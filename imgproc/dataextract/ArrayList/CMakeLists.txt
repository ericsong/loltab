add_library(ArrayList_lib SHARED ArrayList.cpp)
set_target_properties(ArrayList_lib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/libs)
set_target_properties(ArrayList_lib PROPERTIES OUTPUT_NAME ArrayList_lib${BUILD_POSTFIX})
