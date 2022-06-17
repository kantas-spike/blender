
/* SPDX-License-Identifier: GPL-2.0-or-later */

#pragma once

#include <fstream>
#include <optional>

#include "BLI_array.hh"
#include "BLI_edgehash.h"
#include "BLI_float3x3.hh"
#include "BLI_math.h"
#include "BLI_math_vec_types.hh"
#include "BLI_rect.h"
#include "BLI_vector.hh"

#include "DNA_meshdata_types.h"

namespace blender::bke::uv_islands {
// TODO: primitives can be added twice
// TODO: Joining uv island should check where the borders could be merged.
// TODO: this isn't optimized for performance.

/*
 * When enabled various parts of the code would generate an SVG file to visual see how the
 * algorithm makes decisions.
 */
#define DEBUG_SVG

struct UVIslands;
struct UVIslandsMask;
struct MeshEdge;
struct MeshPrimitive;
struct UVPrimitive;
struct UVPrimitiveEdge;
struct UVBorder;
struct UVVertex;
struct UVEdge;

struct MeshEdge;
struct MeshPrimitive;

struct MeshVertex {
  int64_t v;
  Vector<MeshEdge *> edges;
};

struct MeshUVVert {
  MeshVertex *vertex;
  float2 uv;
  int64_t loop;
};

struct MeshEdge {
  MeshVertex *vert1;
  MeshVertex *vert2;
  Vector<MeshPrimitive *> primitives;
};

/** Represents a triangle in 3d space (MLoopTri) */
struct MeshPrimitive {
  int64_t index;
  int64_t poly;
  Vector<MeshEdge *, 3> edges;
  Vector<MeshUVVert, 3> vertices;

  const MeshUVVert &get_uv_vert(const MeshVertex *vert) const
  {
    for (const MeshUVVert &uv_vert : vertices) {
      if (uv_vert.vertex == vert) {
        return uv_vert;
      }
    }
    BLI_assert_unreachable();
    return vertices[0];
  }

  bool has_vertex(const MeshVertex &v) const
  {
    for (int i = 0; i < 3; i++) {
      if (vertices[i].vertex == &v) {
        return true;
      }
    }
    return false;
  }

  MeshUVVert *get_other_uv_vertex(const MeshVertex *v1, const MeshVertex *v2)
  {
    BLI_assert(vertices[0].vertex == v1 || vertices[1].vertex == v1 || vertices[2].vertex == v1);
    BLI_assert(vertices[0].vertex == v2 || vertices[1].vertex == v2 || vertices[2].vertex == v2);
    for (MeshUVVert &uv_vertex : vertices) {
      if (uv_vertex.vertex != v1 && uv_vertex.vertex != v2) {
        return &uv_vertex;
      }
    }
    return nullptr;
  }

  rctf uv_bounds() const
  {
    rctf result;
    BLI_rctf_init_minmax(&result);
    for (const MeshUVVert &uv_vertex : vertices) {
      BLI_rctf_do_minmax_v(&result, uv_vertex.uv);
    }
    return result;
  }
};

/** Wrapper to contain all required mesh data. */
struct MeshData {
 public:
  const MLoopTri *looptri;
  const int64_t looptri_len;
  const int64_t vert_len;
  const MLoop *mloop;
  const MLoopUV *mloopuv;

 public:
  Vector<MeshPrimitive> primitives;
  Vector<MeshEdge> edges;
  Vector<MeshVertex> vertices;

  explicit MeshData(const MLoopTri *looptri,
                    const int64_t looptri_len,
                    const int64_t vert_len,
                    const MLoop *mloop,
                    const MLoopUV *mloopuv)
      : looptri(looptri),
        looptri_len(looptri_len),
        vert_len(vert_len),
        mloop(mloop),
        mloopuv(mloopuv)
  {
    init_vertices();
    init_primitives();
    init_edges();

    for (const MeshVertex &v : vertices) {
      printf("Vert {v%lld}\n", v.v);
      for (const MeshEdge *e : v.edges) {
        printf("-Edge {v%lld v%lld}\n", e->vert1->v, e->vert2->v);
        for (const MeshPrimitive *p : e->primitives) {
          printf(" -Prim {p%lld, v%lld v%lld v%lld}\n",
                 p->index,
                 p->vertices[0].vertex->v,
                 p->vertices[1].vertex->v,
                 p->vertices[2].vertex->v);
        }
      }
    }
  }
  void init_vertices()
  {
    vertices.reserve(vert_len);
    for (int64_t i = 0; i < vert_len; i++) {
      MeshVertex vert;
      vert.v = i;
      vertices.append(vert);
    }
  }
  void init_primitives()
  {
    primitives.reserve(looptri_len);
    for (int64_t i = 0; i < looptri_len; i++) {
      const MLoopTri &tri = looptri[i];
      MeshPrimitive primitive;
      primitive.index = i;
      primitive.poly = tri.poly;

      for (int j = 0; j < 3; j++) {
        MeshUVVert uv_vert;
        uv_vert.loop = tri.tri[j];
        uv_vert.vertex = &vertices[mloop[uv_vert.loop].v];
        uv_vert.uv = mloopuv[uv_vert.loop].uv;
        primitive.vertices.append(uv_vert);
      }
      primitives.append(primitive);
    }
  }

  void init_edges()
  {
    /* TODO: use actual sized. */
    edges.reserve(looptri_len * 2);
    EdgeHash *eh = BLI_edgehash_new_ex(__func__, looptri_len * 2);
    for (int64_t i = 0; i < looptri_len; i++) {
      const MLoopTri &tri = looptri[i];
      MeshPrimitive &primitive = primitives[i];
      for (int j = 0; j < 3; j++) {
        int v1 = mloop[tri.tri[j]].v;
        int v2 = mloop[tri.tri[(j + 1) % 3]].v;
        /* TODO: Use lookup_ptr to be able to store edge 0. */
        void *v = BLI_edgehash_lookup(eh, v1, v2);
        int64_t edge_index;
        if (v == nullptr) {
          edge_index = edges.size();
          BLI_edgehash_insert(eh, v1, v2, POINTER_FROM_INT(edge_index + 1));
          MeshEdge edge;
          edge.vert1 = &vertices[v1];
          edge.vert2 = &vertices[v2];
          edges.append(edge);
          MeshEdge *edge_ptr = &edges.last();
          vertices[v1].edges.append(edge_ptr);
          vertices[v2].edges.append(edge_ptr);
        }
        else {
          edge_index = POINTER_AS_INT(v) - 1;
        }

        MeshEdge *edge = &edges[edge_index];
        edge->primitives.append(&primitive);
        primitive.edges.append(edge);
      }
    }
    BLI_edgehash_free(eh, nullptr);
  }
};

struct UVVertex {
  MeshVertex *vertex;
  /* Position in uv space. */
  float2 uv;

  /* uv edges that are connected to this uvvertex. */
  Vector<UVEdge *> uv_edges;

  explicit UVVertex()
  {
  }

  explicit UVVertex(const MeshUVVert &vert) : vertex(vert.vertex), uv(vert.uv)
  {
  }
};

struct UVEdge {
  std::array<UVVertex *, 2> vertices;
  Vector<UVPrimitive *, 2> uv_primitives;

  bool has_shared_edge(const MeshUVVert &v1, const MeshUVVert &v2) const
  {
    return (vertices[0]->uv == v1.uv && vertices[1]->uv == v2.uv) ||
           (vertices[0]->uv == v2.uv && vertices[1]->uv == v1.uv);
  }

  bool has_shared_edge(const UVVertex &v1, const UVVertex &v2) const
  {
    return (vertices[0]->uv == v1.uv && vertices[1]->uv == v2.uv) ||
           (vertices[0]->uv == v2.uv && vertices[1]->uv == v1.uv);
  }

  bool has_shared_edge(const UVEdge &other) const
  {
    return has_shared_edge(*other.vertices[0], *other.vertices[1]);
  }

  bool has_same_vertices(const MeshVertex &vert1, const MeshVertex &vert2) const
  {
    return (vertices[0]->vertex == &vert1 && vertices[1]->vertex == &vert2) ||
           (vertices[0]->vertex == &vert2 && vertices[1]->vertex == &vert1);
  }

  bool has_same_uv_vertices(const UVEdge &other) const
  {
    return has_shared_edge(other) &&
           has_same_vertices(*other.vertices[0]->vertex, *other.vertices[1]->vertex);
    ;
  }

  bool has_same_vertices(const MeshEdge &edge) const
  {
    return has_same_vertices(*edge.vert1, *edge.vert2);
  }

  bool is_border_edge() const
  {
    return uv_primitives.size() == 1;
  }

  void append_to_uv_vertices()
  {
    for (UVVertex *vertex : vertices) {
      vertex->uv_edges.append_non_duplicates(this);
    }
  }

  UVVertex *get_other_uv_vertex(const MeshVertex *vertex)
  {
    if (vertices[0]->vertex == vertex) {
      return vertices[1];
    }
    return vertices[0];
  }
};

struct UVPrimitive {
  /**
   * Index of the primitive in the original mesh.
   */
  MeshPrimitive *primitive;
  Vector<UVEdge *, 3> edges;

  explicit UVPrimitive(MeshPrimitive *primitive) : primitive(primitive)
  {
  }

  void append_to_uv_edges()
  {
    for (UVEdge *uv_edge : edges) {
      uv_edge->uv_primitives.append_non_duplicates(this);
    }
  }
  void append_to_uv_vertices()
  {
    for (UVEdge *uv_edge : edges) {
      uv_edge->append_to_uv_vertices();
    }
  }

  Vector<std::pair<UVEdge *, UVEdge *>> shared_edges(UVPrimitive &other)
  {
    Vector<std::pair<UVEdge *, UVEdge *>> result;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (edges[i]->has_shared_edge(*other.edges[j])) {
          result.append(std::pair<UVEdge *, UVEdge *>(edges[i], other.edges[j]));
        }
      }
    }
    return result;
  }

  bool has_shared_edge(const UVPrimitive &other) const
  {
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if (edges[i]->has_shared_edge(*other.edges[j])) {
          return true;
        }
      }
    }
    return false;
  }

  bool has_shared_edge(const MeshPrimitive &primitive) const
  {
    for (const UVEdge *uv_edge : edges) {
      const MeshUVVert *v1 = &primitive.vertices.last();
      for (int i = 0; i < primitive.vertices.size(); i++) {
        const MeshUVVert *v2 = &primitive.vertices[i];
        if (uv_edge->has_shared_edge(*v1, *v2)) {
          return true;
        }
        v1 = v2;
      }
    }
    return false;
  }

  /**
   * Get the UVVertex in the order that the verts are ordered in the MeshPrimitive.
   */
  const UVVertex *get_uv_vertex(const uint8_t mesh_vert_index) const
  {
    const MeshVertex *mesh_vertex = primitive->vertices[mesh_vert_index].vertex;
    for (const UVEdge *uv_edge : edges) {
      for (const UVVertex *uv_vert : uv_edge->vertices) {
        if (uv_vert->vertex == mesh_vertex) {
          return uv_vert;
        }
      }
    }
    BLI_assert_unreachable();
    return nullptr;
  }

  /**
   * Get the UVEdge that share the given uv coordinates.
   * Will assert when no UVEdge found.
   */
  UVEdge *get_uv_edge(const float2 uv1, const float2 uv2) const
  {
    for (UVEdge *uv_edge : edges) {
      const float2 &e1 = uv_edge->vertices[0]->uv;
      const float2 &e2 = uv_edge->vertices[1]->uv;
      if ((e1 == uv1 && e2 == uv2) || (e1 == uv2 && e2 == uv1)) {
        return uv_edge;
      }
    }
    BLI_assert_unreachable();
    return nullptr;
  }

  UVEdge *get_uv_edge(const MeshVertex *v1, const MeshVertex *v2) const
  {
    for (UVEdge *uv_edge : edges) {
      const MeshVertex *e1 = uv_edge->vertices[0]->vertex;
      const MeshVertex *e2 = uv_edge->vertices[1]->vertex;
      if ((e1 == v1 && e2 == v2) || (e1 == v2 && e2 == v1)) {
        return uv_edge;
      }
    }
    BLI_assert_unreachable();
    return nullptr;
  }

  const bool contains_uv_vertex(const UVVertex *uv_vertex) const
  {
    for (UVEdge *edge : edges) {
      if (std::find(edge->vertices.begin(), edge->vertices.end(), uv_vertex) !=
          edge->vertices.end()) {
        return true;
      }
    }
    return false;
  }

  const UVVertex *get_other_uv_vertex(const UVVertex *v1, const UVVertex *v2) const
  {
    BLI_assert(contains_uv_vertex(v1));
    BLI_assert(contains_uv_vertex(v2));

    for (const UVEdge *edge : edges) {
      for (const UVVertex *uv_vertex : edge->vertices) {
        if (uv_vertex != v1 && uv_vertex != v2) {
          return uv_vertex;
        }
      }
    }
    BLI_assert_unreachable();
    return nullptr;
  }

  UVBorder extract_border() const;
};

struct UVBorderEdge {
  UVEdge *edge;
  bool tag = false;
  UVPrimitive *uv_primitive;
  /* Should the vertices of the edge be evaluated in reverse order. */
  bool reverse_order = false;

  int64_t index = -1;
  int64_t prev_index = -1;
  int64_t next_index = -1;
  int64_t border_index = -1;

  struct {
    /* Is possible to extend on the `get_uv_vertex(0)` */
    bool extendable : 1;
  } flags;

  explicit UVBorderEdge(UVEdge *edge, UVPrimitive *uv_primitive)
      : edge(edge), uv_primitive(uv_primitive)
  {
    flags.extendable = true;
  }

  UVVertex *get_uv_vertex(int index)
  {
    int actual_index = reverse_order ? 1 - index : index;
    return edge->vertices[actual_index];
  }

  const UVVertex *get_uv_vertex(int index) const
  {
    int actual_index = reverse_order ? 1 - index : index;
    return edge->vertices[actual_index];
  }

  /**
   * Get the uv vertex from the primitive that is not part of the edge.
   */
  const UVVertex *get_other_uv_vertex() const
  {
    return uv_primitive->get_other_uv_vertex(edge->vertices[0], edge->vertices[1]);
  }

  float length() const
  {
    return len_v2v2(edge->vertices[0]->uv, edge->vertices[1]->uv);
  }
};

struct UVBorderCorner {
  UVBorderEdge *first;
  UVBorderEdge *second;
  float angle;

  UVBorderCorner(UVBorderEdge *first, UVBorderEdge *second, float angle)
      : first(first), second(second), angle(angle)
  {
  }

  float2 uv(float factor)
  {
    float2 origin = first->get_uv_vertex(1)->uv;
    float angle_between = angle * factor;
    float desired_len = second->length() * factor + first->length() * (1.0 - factor);
    float2 v = first->get_uv_vertex(0)->uv - origin;
    normalize_v2(v);

    float3x3 rot_mat = float3x3::from_rotation(angle_between);
    float2 rotated = rot_mat * v;
    float2 result = rotated * desired_len + first->get_uv_vertex(1)->uv;
    return result;
  }
};

struct UVBorder {
  /** Ordered list of UV Verts of the border of this island. */
  // TODO: support multiple rings + order (CW, CCW)
  Vector<UVBorderEdge> edges;

  /**
   * Check if the border is counter clock wise from its island.
   */
  bool is_ccw() const;

  /**
   * Flip the order of the verts, changing the order between CW and CCW.
   */
  void flip();

  /**
   * Calculate the outside angle of the given vert.
   */
  float outside_angle(const UVBorderEdge &vert) const;

  void update_indexes(uint64_t border_index);

  /**
   * Mark edges not extendable when they don't share the same vertex.
   *
   * No solutions exists (yet) when the vertexes are different.
   */
  void update_extendability();

  static std::optional<UVBorder> extract_from_edges(Vector<UVBorderEdge> &edges);

  void validate() const;

  /** Remove edge from the border. updates the indexes. */
  void remove(int64_t index)
  {
    /* Could read the border_index from any border edge as they are consistent. */
    uint64_t border_index = edges[0].border_index;
    edges.remove(index);
    update_indexes(border_index);
  }
};

struct UVIsland {
  Vector<UVVertex> uv_vertices;
  Vector<UVEdge> uv_edges;
  Vector<UVPrimitive> uv_primitives;
  /**
   * List of borders of this island. There can be multiple borders per island as a border could
   * be completely encapsulated by another one.
   */
  Vector<UVBorder> borders;

  UVIsland()
  {
    uv_vertices.reserve(100000);
    uv_edges.reserve(100000);
    uv_primitives.reserve(100000);
  }

  UVPrimitive *add_primitive(MeshPrimitive &primitive)
  {
    UVPrimitive uv_primitive(&primitive);
    uv_primitives.append(uv_primitive);
    UVPrimitive *uv_primitive_ptr = &uv_primitives.last();
    for (MeshEdge *edge : primitive.edges) {
      const MeshUVVert &v1 = primitive.get_uv_vert(edge->vert1);
      const MeshUVVert &v2 = primitive.get_uv_vert(edge->vert2);
      UVEdge uv_edge_template;
      uv_edge_template.vertices[0] = lookup_or_create(UVVertex(v1));
      uv_edge_template.vertices[1] = lookup_or_create(UVVertex(v2));
      UVEdge *uv_edge = lookup_or_create(uv_edge_template);
      uv_primitive_ptr->edges.append(uv_edge);
      uv_edge->append_to_uv_vertices();
      uv_edge->uv_primitives.append(uv_primitive_ptr);
    }
    return uv_primitive_ptr;
  }

  UVVertex *lookup_or_create(const UVVertex &vertex)
  {
    for (UVVertex &uv_vertex : uv_vertices) {
      if (uv_vertex.uv == vertex.uv && uv_vertex.vertex == vertex.vertex) {
        return &uv_vertex;
      }
    }

    uv_vertices.append(vertex);
    UVVertex *result = &uv_vertices.last();
    result->uv_edges.clear();
    return result;
  }

  UVEdge *lookup_or_create(const UVEdge &edge)
  {
    for (UVEdge &uv_edge : uv_edges) {
      if (uv_edge.has_same_uv_vertices(edge)) {
        return &uv_edge;
      }
    }

    uv_edges.append(edge);
    UVEdge *result = &uv_edges.last();
    result->uv_primitives.clear();
    return result;
  }

  /** Initialize the border attribute. */
  void extract_borders();
  /** Iterative extend border to fit the mask. */
  void extend_border(const UVIslandsMask &mask,
                     const short island_index,
                     const MeshData &mesh_data);

 private:
  void append(const UVPrimitive &primitive)
  {
    uv_primitives.append(primitive);
    UVPrimitive *new_prim_ptr = &uv_primitives.last();
    for (int i = 0; i < 3; i++) {
      UVEdge *other_edge = primitive.edges[i];
      UVEdge uv_edge_template;
      uv_edge_template.vertices[0] = lookup_or_create(*other_edge->vertices[0]);
      uv_edge_template.vertices[1] = lookup_or_create(*other_edge->vertices[1]);
      new_prim_ptr->edges[i] = lookup_or_create(uv_edge_template);
      new_prim_ptr->edges[i]->append_to_uv_vertices();
      new_prim_ptr->edges[i]->uv_primitives.append(new_prim_ptr);
    }
  }

 public:
  bool has_shared_edge(const UVPrimitive &primitive) const
  {
    for (const UVPrimitive &prim : uv_primitives) {
      if (prim.has_shared_edge(primitive)) {
        return true;
      }
    }
    return false;
  }
  bool has_shared_edge(const MeshPrimitive &primitive) const
  {
    for (const UVPrimitive &prim : uv_primitives) {
      if (prim.has_shared_edge(primitive)) {
        return true;
      }
    }
    return false;
  }

  const void extend_border(const UVPrimitive &primitive)
  {
    for (UVPrimitive &prim : uv_primitives) {
      if (prim.has_shared_edge(primitive)) {
        append(primitive);
      }
    }
  }

  /**
   * Join 2 uv islands together where the primitive gives the location that joins the two islands
   * together.
   *
   * NOTE: this cannot be used to join two islands that have multiple shared primitives, or
   * connecting via multiple primitives.
   * */
  void join(const UVIsland &other)
  {
    for (const UVPrimitive &other_prim : other.uv_primitives) {
      append(other_prim);
    }
  }

  void validate_primitives() const
  {
    /* Each UVPrimitive that points to the same mesh primitive should contain the same mesh
     * vertices (and can be retrieved in the same order). */
    for (const UVPrimitive &prim_a : uv_primitives) {
      validate_primitive(prim_a);
    }
  }
  void validate_primitive(const UVPrimitive &prim_a) const
  {
    /* Each UVPrimitive that points to the same mesh primitive should contain the same mesh
     * vertices (and can be retrieved in the same order). */
    for (const UVPrimitive &prim_b : uv_primitives) {
      if (prim_a.primitive != prim_b.primitive) {
        continue;
      }
      for (int i = 0; i < 3; i++) {
        const UVVertex *v1 = prim_a.get_uv_vertex(i);
        const UVVertex *v2 = prim_b.get_uv_vertex(i);
        BLI_assert(v1->vertex->v == v2->vertex->v);
      }
    }
  }

  void validate_border() const
  {
    /* Each UVPrimitive that points to the same mesh primitive should contain the same mesh
     * vertices (and can be retrieved in the same order). */
    for (const UVPrimitive &prim_a : uv_primitives) {
      for (const UVPrimitive &prim_b : uv_primitives) {
        if (prim_a.primitive != prim_b.primitive) {
          continue;
        }
        for (int i = 0; i < 3; i++) {
          const UVVertex *v1 = prim_a.get_uv_vertex(i);
          const UVVertex *v2 = prim_b.get_uv_vertex(i);
          BLI_assert(v1->vertex->v == v2->vertex->v);
        }
      }
    }
    for (const UVBorder &border : borders) {
      border.validate();
    }
  }
};

/* Debug functions to export to a SVG file. */
void svg_header(std::ostream &ss);
void svg(std::ostream &ss, const UVIsland &islands, int step);
void svg(std::ostream &ss, const UVIslands &islands, int step);
void svg(std::ostream &ss, const UVPrimitive &primitive);
void svg(std::ostream &ss, const UVPrimitive &primitive, int step);
void svg(std::ostream &ss, const UVIslandsMask &mask, int step);
void svg(std::ostream &ss, const UVBorder &border, int step);
void svg_footer(std::ostream &ss);

struct UVIslands {
  Vector<UVIsland> islands;

  explicit UVIslands(MeshData &mesh_data)
  {
    islands.reserve(1000);

#ifdef DEBUG_SVG
    std::ofstream of;
    of.open("/tmp/islands.svg");
    svg_header(of);
    int step = 0;
    for (MeshPrimitive &primitive : mesh_data.primitives) {
      add(primitive);
      svg(of, *this, step++);
    }
    svg_footer(of);
    of.close();
#else
    for (MeshPrimitive &primitive : mesh_data.primitives) {
      add(primitive);
    }
#endif
  }

  void extract_borders()
  {
    for (UVIsland &island : islands) {
      island.extract_borders();
    }

#ifdef DEBUG_SVG
    std::ofstream of;
    of.open("/tmp/borders.svg");
    svg_header(of);
    for (UVIsland &island : islands) {
      int index = 0;
      for (UVBorder &border : island.borders) {
        border.update_indexes(index);
        index++;
        svg(of, border, 0);
      }
    }
    svg_footer(of);
    of.close();
#endif
  }

  void extend_borders(const UVIslandsMask &islands_mask, const MeshData &mesh_data)
  {
    printf("Extending borders\n");
    printf("=================\n");
    ushort index = 0;
    for (UVIsland &island : islands) {
      printf("Island %d\n", index);
      printf("---------\n");
      island.extend_border(islands_mask, index++, mesh_data);
    }

#ifdef DEBUG_SVG
    std::ofstream of;
    of.open("/tmp/borders.svg");
    svg_header(of);
    for (const UVIsland &island : islands) {
      for (const UVBorder &border : island.borders) {
        svg(of, border, 0);
      }
    }
    svg_footer(of);
    of.close();
#endif
  }

 private:
  void add(MeshPrimitive &primitive)
  {
    Vector<uint64_t> extended_islands;
    for (uint64_t index = 0; index < islands.size(); index++) {
      UVIsland &island = islands[index];
      if (island.has_shared_edge(primitive)) {
        extended_islands.append(index);
      }
    }

    if (extended_islands.size() > 0) {
      UVIsland &island = islands[extended_islands[0]];
      island.add_primitive(primitive);

      /* `extended_islands` can hold upto 3 islands that are connected with the given tri.
       * they can be joined to a single island, using the first as its target. */
      for (uint64_t index = 1; index < extended_islands.size(); index++) {
        island.join(islands[extended_islands[index]]);
      }

      /* remove the islands that have been joined, starting at the end. */
      for (uint64_t index = extended_islands.size() - 1; index > 0; index--) {
        islands.remove(extended_islands[index]);
      }

      return;
    }

    /* if the tri has not been added we can create a new island. */
    UVIsland *island = create_island();
    island->add_primitive(primitive);
  }

  UVIsland *create_island()
  {
    islands.append(UVIsland());
    return &islands.last();
  }

  bool validate() const
  {
    /* After operations it is not allowed that islands share any edges. In that case it should
     * already be merged. */
    for (int i = 0; i < islands.size() - 1; i++) {
      for (int j = i + 1; j < islands.size(); j++) {
        for (const UVPrimitive &prim : islands[j].uv_primitives) {
          if (islands[i].has_shared_edge(prim)) {
            return false;
          }
        }
      }
    }
    return true;
  }
};

/* Bitmask containing the num of the nearest Island. */
// TODO: this is a really quick implementation.
struct UVIslandsMask {
  float2 udim_offset;
  ushort2 resolution;
  Array<uint16_t> mask;

  UVIslandsMask(float2 udim_offset, ushort2 resolution)
      : udim_offset(udim_offset), resolution(resolution), mask(resolution.x * resolution.y)
  {
    clear();
  }

  void clear()
  {
    mask.fill(0xffff);
  }
  /**
   * Is the given uv coordinate part of the given island_index mask.
   *
   * true - part of the island mask.
   * false - not part of the island mask.
   */
  bool is_masked(const short island_index, const float2 uv) const;

  void add(const UVIslands &islands)
  {
    for (int index = 0; index < islands.islands.size(); index++) {
      add(index, islands.islands[index]);
    }
  }

  void add(short island_index, const UVIsland &island)
  {
    for (const UVPrimitive &uv_primitive : island.uv_primitives) {
      const MeshPrimitive *mesh_primitive = uv_primitive.primitive;

      rctf uv_bounds = mesh_primitive->uv_bounds();
      rcti buffer_bounds;
      buffer_bounds.xmin = max_ii(floor((uv_bounds.xmin - udim_offset.x) * resolution.x), 0);
      buffer_bounds.xmax = min_ii(ceil((uv_bounds.xmax - udim_offset.x) * resolution.x),
                                  resolution.x - 1);
      buffer_bounds.ymin = max_ii(floor((uv_bounds.ymin - udim_offset.y) * resolution.y), 0);
      buffer_bounds.ymax = min_ii(ceil((uv_bounds.ymax - udim_offset.y) * resolution.y),
                                  resolution.y - 1);

      for (int y = buffer_bounds.ymin; y < buffer_bounds.ymax + 1; y++) {
        for (int x = buffer_bounds.xmin; x < buffer_bounds.xmax + 1; x++) {
          float2 uv(float(x) / resolution.x, float(y) / resolution.y);
          float3 weights;
          barycentric_weights_v2(mesh_primitive->vertices[0].uv,
                                 mesh_primitive->vertices[1].uv,
                                 mesh_primitive->vertices[2].uv,
                                 uv,
                                 weights);
          if (!barycentric_inside_triangle_v2(weights)) {
            continue;
          }

          uint64_t offset = resolution.x * y + x;
          mask[offset] = island_index;
        }
      }
    }
  }

  void dilate(int max_iterations);

  void print() const
  {
    int offset = 0;
    for (int y = 0; y < resolution.y; y++) {
      for (int x = 0; x < resolution.x; x++) {
        uint16_t value = mask[offset++];
        if (value == 0xffff) {
          printf(" ");
        }
        else if (value == 0) {
          printf("0");
        }
        else if (value == 1) {
          printf("1");
        }
        else if (value == 2) {
          printf("2");
        }
        else if (value == 3) {
          printf("3");
        }
        else if (value == 4) {
          printf("4");
        }
        else if (value == 5) {
          printf("5");
        }
        else if (value == 6) {
          printf("6");
        }
      }
      printf("\n");
    }
  }
};

}  // namespace blender::bke::uv_islands