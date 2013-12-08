add_library(StringAlgos_lib SHARED StringAlgos.cpp)
set_target_properties(StringAlgos_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
set_target_properties(StringAlgos_lib PROPERTIES OUTPUT_NAME StringAlgos_lib${BUILD_POSTFIX})
