# SPDX-License-Identifier: GPL-2.0-or-later

ExternalProject_Add(external_epoxy
  URL file://${PACKAGE_DIR}/${EPOXY_FILE}
  DOWNLOAD_DIR ${DOWNLOAD_DIR}
  URL_HASH ${EPOXY_HASH_TYPE}=${EPOXY_HASH}
  PREFIX ${BUILD_DIR}/epoxy
  CONFIGURE_COMMAND ${CONFIGURE_ENV} && meson setup --prefix ${LIBDIR}/epoxy --default-library static --libdir lib . ../external_epoxy -Dtests=false
  BUILD_COMMAND ninja
  INSTALL_COMMAND ninja install
)
