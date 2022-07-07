# Quickstart [info_quickstart]

この `API (Application Programming Interface)`
は、おおむね安定していますが、一部の分野はまだ拡張・改良中です。

**Blender Python API の特徴:**

-   ユーザーインターフェイスで利用できる全てのデータを編集できる。
    (Scenes, Meshes, Particles etc.)
-   ユーザープリファレンスやキーマップ、テーマを変更できる。
-   ツールを独自の設定で起動できる。
-   メニューやヘッダ、パネルのようなユーザーインターフェイスの要素を作成できる。
-   新しいツールを作成できる。
-   インタラクティブツールを作成できる。
-   Blenderと統合可能な新しいレンダリングエンジンを作成できる。
-   データやプロパティの変更を購読できる。
-   既存のBlenderデータに新しい設定を定義できる。
-   Pythonを使って、3Dビューポートに描画できる。

**まだ、存在しない特徴:**

-   新しいスペースタイプを作成できる。
-   全てのタイプにカスタムプロパティを割り当てることができる。

この文書は、Blender Python API に習熟できることを目的としていますが、
全てのトピックをカバーしているわけではありません。

はじめる前に知っておくと便利なことを簡単にまとめました:

-   `Developer Extra <blender_manual:prefs-interface-dev-extras>` と
    `Python Tooltips <blender_manual:prefs-interface-tooltips-python>`
    の設定を有効化

-   `Python Console <blender_manual:bpy.types.SpaceConsole>`
    はワンライナーをテストするのに大変便利です。
    自動補完機能があるのでAPIを素早く調べることができます。

-   上述の設定を有効にしていれば、ツールチップボタンがPythonの属性とオペレーターの名前を表示します。

-   上述の設定を有効にしていれば、ボタンのコンテキストメニューがAPIドキュメントに直接リンクされます。

-   たくさんのPythonのサンプルがテキストエディタのテンプレートメニューでみつけることができます。

-   Blenderとともに配布されている更なるスクリプトを調べるには以下を参照してください:

    -   `scripts/startup/bl_ui` ユーザーインターフェイス用
    -   `scripts/startup/bl_operators` オペレーター用.

    正確な位置はプラットフォームに依存しますので、以下を参照してください:
    `directory layout docs <blender_manual:blender-directory-layout>`.

## スクリプトを実行する

Pythonスクリプトを実行する、最も一般的な2つ方法が、組み込みのテキストエディタを使う方法、とPythonコンソールにコマンド入力する方法です。
*テキストエディタ* と *Pythonコンソール*
は共に、ヘッダから選択できるスペースのタイプにです。
手動で設定したPython開発用のスペースではなく、トップバータブからアクセスできる
*スクリプティング* ワークスペースを使いましょう。

テキストエディタで、 `.py`
ファイルを開くか、クリップボードから貼り付けて、 *Run Script*
を使って試すことができます。 Python
コンソールは、一般的に、スニペットの入力や、すぐにフィードバックを得るために使用しますが、
スクリプト全体を貼り付けることも可能です。
スクリプトはコマンドラインからBlenderとともに実行することも可能ですが、Blenderでのスクリプティングの学習において必須ではありません。

### キーコンセプト

## データアクセス

#### データブロックへのアクセス

Python API
を使って、アニメーションシステムやユーザーインターフェイスと同じやり方でBlenderのデータにアクセスできます。
これは、ボタンで変更できるようなどのような設定も、Pythonで変更できることを意味します。
現在読み込まれているblendファイルのデータへのアクセスは、 `bpy.data`
モジュールで行います。
このモジュールは、ライブラリデータへのアクセスを提供します。例えば:

> \>\>\> bpy.data.objects \<bpy_collection\[3\], BlendDataObjects>
>
> \>\>\> bpy.data.scenes \<bpy_collection\[1\], BlendDataScenes>
>
> \>\>\> bpy.data.materials \<bpy_collection\[1\], BlendDataMaterials>

#### コレクションへのアクセス

インデックスと同じく文字列でコレクションのメンバーへのアクセスに使用できることにお気付きでしょう。
Pythonのディクショナリーと違い、どちらの方法も利用できます;
しかしながら、メンバーのインデックスは、Blenderの実行中に変わることがあります。

> \>\>\> list(bpy.data.objects) \[bpy.data.objects\["Cube"\],
> bpy.data.objects\["Plane"\]\]
>
> \>\>\> bpy.data.objects\['Cube'\] bpy.data.objects\["Cube"\]
>
> \>\>\> bpy.data.objects\[0\] bpy.data.objects\["Cube"\]

#### 属性へのアクセス

マテリアルやオブジェクト、コレクションといったデータブロックを一度取得したら、
まるでGUIで設定を変更するかのように、その属性にアクセスできます。
実際、各ボタンのツールチップは、Pythonの属性を表示します。これはスクリプトでどこの設定を変更するべきか調べるさいに便利です。

> \>\>\> bpy.data.objects\[0\].name 'Camera'
>
> \>\>\> bpy.data.scenes\["Scene"\] bpy.data.scenes\['Scene'\]
>
> \>\>\> bpy.data.materials.new("MyMaterial")
> bpy.data.materials\['MyMaterial'\]

どのデータにアクセスするべきか試す場合は、Python
コンソールの利用が便利です。Python
コンソールは独自のスペースタイプです。
これは、オートコンプリートをサポートし、あなたのコード内のデータを素早く調査する手段を提供します。

コンソールですぐに確認できるデータパスの例:

> \>\>\> bpy.data.scenes\[0\].render.resolution_percentage 100 \>\>\>
> bpy.data.scenes\[0\].objects\["Torus"\].data.vertices\[0\].co.x 1.0

#### データ作成/削除

他のPython APIに慣れている場合、 bpy
APIで、該当クラスの呼び出しにより新しいデータブロックを作成できないことに驚くかもしれません:

> \>\>\> bpy.types.Mesh() Traceback (most recent call last): File
> "\<blender_console>", line 1, in \<module> TypeError:
> bpy_struct.\_\_new\_\_(type): expected a single argument

これはAPI設計の意図的な部分です。 Blender Python API は
メインのBlenderデータベース(`bpy.data`
を通してアクセスされる)の外で、Blenderデータを作成できません。
このデータはBlenderによって管理されます。(保存、読み込み、アンドゥ、追加など)

`bpy.data`
内のコレクションのメソッドを経由して、データは追加、削除されます。例えば:

> \>\>\> mesh = bpy.data.meshes.new(name="MyMesh") \>\>\> print(mesh)
> \<bpy_struct, Mesh("MyMesh.001")>
>
> \>\>\> bpy.data.meshes.remove(mesh)

#### カスタムプロパティ

IDを持ついずれのデータブロック(:mod:\`bpy.data\`にリンクして、アクセスできるデータ)のプロパティに、Pythonはアクセスできます。
プロパティを割り当てる際に、独自の名前を選択できます。
これらは必要な時に作成され、すでに存在する場合は上書きされます。

このデータはblendファイルに保存され、オブジェクトとしてコピーされます。例えば:

``` python
bpy.context.object["MyOwnProperty"] = 42

if "SomeProp" in bpy.context.object:
    print("Property found")

# Use the get function like a Python dictionary
# which can have a fallback value.
value = bpy.data.scenes["Scene"].get("test_prop", "fallback value")

# dictionaries can be assigned as long as they only use basic types.
collection = bpy.data.collections.new("MyTestCollection")
collection["MySettings"] = {"foo": 10, "bar": "spam", "baz": {}}

del collection["MySettings"]
```

これらのプロパティには、Pythonの基本型しか割り当てられないことに注意してください。

-   int, float, string
-   array of ints or floats
-   dictionary (only string keys are supported, values must be basic
    types too)

これらのプロパティはPythonの外でも有効です。
これらは曲線アニメーションやドライバーパスで使用することができます。

## Context

名前やリストで直接データにアクセスできることは便利ですが、ユーザーが選択しているデータを操作することは、より一般的です。
コンテキストは `bpy.context` で、いつでも利用でき、
アクティブオブジェクト、シーンや他の多くの属性を持つツール設定の取得に使用されます。

いくつかの一般的なユースケースは以下になります:

> \>\>\> bpy.context.object \>\>\> bpy.context.selected_objects \>\>\>
> bpy.context.visible_bones

コンテキストは読取専用です。これは、直接その値を変更できないことを意味します。
ですが、API関数の実行やデータAPIの使用により、値を変更可能です。

そのため、`bpy.context.active_object = obj` はエラーになりますが、
`bpy.context.view_layer.objects.active = obj` は期待通りに動きます。

コンテキストの属性は、アクセスしている場所によって変わります。
3Dビューポートは、Pythonコンソールと比べて、コンテキストのメンバーが異なります。
そのため、コンテキストの属性にアクセスするときは、ユーザーの状態を知るように注意してください。

参照 `bpy.context` API リファレンス

## Operators (Tools)

オペレーターは一般的に、ボタンやメニュー、キーショートカットによりユーザーがアクセスするツールのことです。
ユーザーの観点ではツールですが、Pythonは `bpy.ops`
を通して、これらを独自の設定で実行することができます。

例:

> \>\>\> bpy.ops.mesh.flip_normals() {'FINISHED'} \>\>\>
> bpy.ops.mesh.hide(unselected=False) {'FINISHED'} \>\>\>
> bpy.ops.object.transform_apply() {'FINISHED'}

<div class="tip" markdown="1">

<div class="title" markdown="1">

Tip

</div>

</div>

`Operator Cheat Sheet <blender_manual:bpy.ops.wm.operator_cheat_sheet>`
に全てのオペレーターとPython上のデフォルト値のリストと、生成されたドキュメントがあります。
これは、全てのBlenderオペレーターの概要を知る良い方法です。

#### Operator Poll()

多くのオペレーターは"poll"関数を持ちます。
これは、カーソルが正しい場所にあるかどうか、オブジェクトが正しいモード(Editモード、ウェイトペイントモードなど)かどうかをチェックします。
Pythonでオペレーターのpoll関数が失敗した場合、例外が発生します。

例えば、 コンソールから`bpy.ops.view3d.render_border()`
を呼ぶと、以下の例外が発生します。

``` python
RuntimeError: Operator bpy.ops.view3d.render_border.poll() failed, context is incorrect
```

このケースでは、コンテキストはアクティブカメラを持つ3Dビューポートである必要があります。

try-except節の使用を避けるためには、オペレーターを呼ぶ時はいつでも、
現在のコンテキストでオペレーターが実行できるかどうかチェックするためにオペレーター自身の
`poll()` 関数を呼びます。

``` python
if bpy.ops.view3d.render_border.poll():
    bpy.ops.view3d.render_border()
```

### インテグレーション

Pythonスクリプトは、以下の方法でBlenderと統合できます。

-   レンダリングエンジンを定義
-   オペレーターを定義
-   メニュー、ヘッダやパネルを定義
-   新しいボタンを既存のメニュー、ヘッダやパネルに追加

Pythonでは、統合は既存型のサブクラスを定義することで実行します。

## オペレーターの例

``` python
import bpy


def main(context):
    for ob in context.scene.objects:
        print(ob)


class SimpleOperator(bpy.types.Operator):
    """Tooltip"""
    bl_idname = "object.simple_operator"
    bl_label = "Simple Object Operator"

    @classmethod
    def poll(cls, context):
        return context.active_object is not None

    def execute(self, context):
        main(context)
        return {'FINISHED'}


def menu_func(self, context):
    self.layout.operator(SimpleOperator.bl_idname, text=SimpleOperator.bl_label)


# Register and add to the "object" menu (required to also use F3 search "Simple Object Operator" for quick access).
def register():
    bpy.utils.register_class(SimpleOperator)
    bpy.types.VIEW3D_MT_object.append(menu_func)


def unregister():
    bpy.utils.unregister_class(SimpleOperator)
    bpy.types.VIEW3D_MT_object.remove(menu_func)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.object.simple_operator()
```

一度、このスクリプトを実行すると、 `SimpleOperator`
がBlenderに登録されます。
そして、オペレーターの検索やツールバーに追加することで、利用できるようになります。

スクリプトを実行するには:

1.  Blenderを起動し、スクリプティングワークスペースに切り替えます
2.  新しいテキスト欄を作成するために、テキストエディタで *New*
    ボタンをクリックします。
3.  上述のコードをコピーし、テキストエディタにペーストします。
4.  *Run Script* ボタンをクリックします。
5.  カーソルを3Dビューポートに移動し、`Operator Search menu <blender_manual:bpy.ops.wm.search_menu>`
    を開き、"Simple"と入力します。
6.  検索結果の" Simple Operator"をクリックします。

<div class="seealso" markdown="1">

</div>

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

</div>

## Panelの例

パネルは、オペレーターと同じく、クラスとして登録されます。 追加の `bl_`
変数は、パネルが表示しているコンテキストを設定するために使用されることに注目してください。

``` python
import bpy


class HelloWorldPanel(bpy.types.Panel):
    """Creates a Panel in the Object properties window"""
    bl_label = "Hello World Panel"
    bl_idname = "OBJECT_PT_hello"
    bl_space_type = 'PROPERTIES'
    bl_region_type = 'WINDOW'
    bl_context = "object"

    def draw(self, context):
        layout = self.layout

        obj = context.object

        row = layout.row()
        row.label(text="Hello world!", icon='WORLD_DATA')

        row = layout.row()
        row.label(text="Active object is: " + obj.name)
        row = layout.row()
        row.prop(obj, "name")

        row = layout.row()
        row.operator("mesh.primitive_cube_add")


def register():
    bpy.utils.register_class(HelloWorldPanel)


def unregister():
    bpy.utils.unregister_class(HelloWorldPanel)


if __name__ == "__main__":
    register()
```

スクリプトを実行するには:

1.  Blenderを起動し、スクリプティングワークスペースに切り替えます
2.  新しいテキスト欄を作成するために、テキストエディタで *New*
    ボタンをクリックします。
3.  上述のコードをコピーし、テキストエディタにペーストします。
4.  *Run Script* ボタンをクリックします。

結果を見るには:

1.  デフォルトのキューブを選択します
2.  ボタンパネルのオブジェクトプロパティアイコンをクリックする。(右端の小さなキューブです)
3.  スクロールダウンし、 "Hello World Panel"
    という名前のパネルを見つけます。
4.  オブジェクト名を変更すると、\*Hello World
    Panel\*の名前フィールドも更新されます。

行の配置と、ラベルとプロパティは、コードで定義されることに注意してください。

<div class="seealso" markdown="1">

`bpy.types.Panel`

</div>

### Types

Blenderは多くのPython型を定義していますが、Pythonのネイティブ型も使用しています。
BlenderのPython APIは、3つのカテゴリに分けることができます。

## Native Types

単純なケースでは、数値や文字列をカスタム型としてリターンするのはやりすぎです。
なので、普通のPython型を使用します。

-   Blender float, int, boolean -> float, int, boolean

-   Blender enumerator -> string

    > \>\>\> C.object.rotation_mode = 'AXIS_ANGLE'

-   Blender enumerator (multiple) -> set of strings

    ``` python
    # setting multiple camera overlay guides
    bpy.context.scene.camera.data.show_guide = {'GOLDEN', 'CENTER'}

    # passing as an operator argument for report types
    self.report({'WARNING', 'INFO'}, "Some message!")
    ```

## Internal Types

`bpy.types.bpy_struct`
はBlenderのデータブロックとコレクションに使われます。 collections,
meshes, bones, scenesのような自分の属性自身を含むデータにも使われます。

2つのBlenderのデータをラップした主要な型があります。
1つはデータブロック(内部表現では
`bpy_struct`)です。もう1つがプロパティです。

> \>\>\> bpy.context.object bpy.data.objects\['Cube'\]
>
> \>\>\> C.scene.objects bpy.data.scenes\['Scene'\].objects

これらの型は、Blenderのデータを参照しているので、これらを変更するとすぐに見た目に反映されることに注意してください。

## Mathutils Types

`` mathutils`から利用できるのは、ベクターや、クォータニオン、オイラー角、マトリックス、そしてカラータイプです。
:class:`bpy.types.Object.location `` や
`bpy.types.PoseBone.rotation_euler`、 `bpy.types.Scene.cursor_location`
のような属性は、
一緒に使ったり、様々な便利な方法で操作できる特殊な数学型としてアクセスできます。

マトリックスとベクターの乗算の例です:

``` python
bpy.context.object.matrix_world @ bpy.context.object.data.verts[0].co
```

<div class="note" markdown="1">

<div class="title" markdown="1">

Note

</div>

mathutils型は、Blenderの内部データを参照しているので、変更を元に戻すことができます。

Example:

``` python
# modifies the Z axis in place.
bpy.context.object.location.z += 2.0

# location variable holds a reference to the object too.
location = bpy.context.object.location
location *= 2.0

# Copying the value drops the reference so the value can be passed to
# functions and modified without unwanted side effects.
location = bpy.context.object.location.copy()
```

</div>

### Animation

Pythonでキーフレームを追加するには、2つの方法があります。

最初の方法は、キープロパティを直接経由する方法です。ユーザーがボタンからキーフレームを追加することと似ています。
カーブとキーフレームデータを手動で作成してから、そのパスをプロパティに設定する方法もあります。
ここに、両方の方法の例があります。両方ともアクティブオブジェクトのZ軸にキーフレームを追加しています。

Simple example:

``` python
obj = bpy.context.object
obj.location[2] = 0.0
obj.keyframe_insert(data_path="location", frame=10.0, index=2)
obj.location[2] = 1.0
obj.keyframe_insert(data_path="location", frame=20.0, index=2)
```

Using low-level functions:

``` python
obj = bpy.context.object
obj.animation_data_create()
obj.animation_data.action = bpy.data.actions.new(name="MyAction")
fcu_z = obj.animation_data.action.fcurves.new(data_path="location", index=2)
fcu_z.keyframe_points.add(2)
fcu_z.keyframe_points[0].co = 10.0, 0.0
fcu_z.keyframe_points[1].co = 20.0, 1.0
```
