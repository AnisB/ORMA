cmake_minimum_required(VERSION 3.2)

# The defines we need for the library
set(ORMA_SDK_INCLUDE ${ORMA_SDK_ROOT}/include)
set(ORMA_CAPI_INCLUDE ${ORMA_CAPI_ROOT}/include)
set(ORMA_CAPI_SRC ${ORMA_CAPI_ROOT}/src)

bento_headers(header_files "${ORMA_CAPI_INCLUDE}" "c_api")
bento_sources(source_files "${ORMA_CAPI_SRC}" "c_api")

# Generate the static library
bento_dynamic_lib("orma_dylib" "c_api" "${header_files};${source_files};" "${ORMA_SDK_INCLUDE};${ORMA_3RD_LIBRARIES};${ORMA_CAPI_INCLUDE}")
target_link_libraries("orma_dylib" "orma_sdk" "bento_sdk")