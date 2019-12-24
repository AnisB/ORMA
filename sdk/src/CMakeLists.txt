cmake_minimum_required(VERSION 3.2)

# The defines we need for the library
set(ORMA_SDK_INCLUDE ${ORMA_SDK_ROOT}/include)
set(ORMA_SDK_SOURCE ${ORMA_SDK_ROOT}/src)

sub_directory_list(sub_projects_headers "${ORMA_SDK_INCLUDE}")
foreach(header_dir ${sub_projects_headers})
	bento_headers(tmp_header_list "${ORMA_SDK_INCLUDE}/${header_dir}" "${header_dir}")
	list(APPEND header_files "${tmp_header_list}")
endforeach()

sub_directory_list(sub_projects_sources "${ORMA_SDK_SOURCE}")
foreach(source_dir ${sub_projects_sources})
	bento_sources(tmp_source_list "${ORMA_SDK_SOURCE}/${source_dir}" "${source_dir}")
	list(APPEND source_files "${tmp_source_list}")
endforeach()

# Generate the static library
bento_static_lib("orma_sdk" "sdk" "${header_files};${source_files};" "${ORMA_SDK_INCLUDE};${ORMA_3RD_INCLUDE}/include;${BENTO_SDK_ROOT}/include")
