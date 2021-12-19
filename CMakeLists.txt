# CMake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Project
project(cpuusage)

# Utilities
configure_file(src/cpuusage ${CMAKE_CURRENT_BINARY_DIR}/bin/cpuusage COPYONLY)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bin/cpuusage DESTINATION bin)
configure_file(src/cugenwrapper ${CMAKE_CURRENT_BINARY_DIR}/bin/cugenwrapper COPYONLY)
install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/bin/cugenwrapper DESTINATION bin)

# Manual
configure_file(src/cpuusage.1 ${CMAKE_CURRENT_BINARY_DIR}/share/man/man1/cpuusage.1 COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/man/man1/cpuusage.1 DESTINATION share/man/man1)

# Catapult
if (NOT IS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/catapult)
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xzvf ${CMAKE_SOURCE_DIR}/ext/catapult.tar.gz)
endif ()
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/catapult DESTINATION share/cpuusage)

# POSIX Wrappers - Apple
if (APPLE)
  file(COPY posixwrap/apple/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/posixwrap)
endif ()

# POSIX Wrappers - Linux
if (UNIX AND NOT APPLE)
  file(COPY posixwrap/linux/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/posixwrap)
endif ()

# POSIX Wrappers - Common
file(COPY posixwrap/common/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/posixwrap)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/posixwrap DESTINATION share/cpuusage)

# Cpuusage library source
configure_file(src/cumain.cpp ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/cumain.cpp COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/cumain.cpp DESTINATION share/cpuusage/src)

# Cpuusage library CMakeLists
configure_file(src/libcpuusage_CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/libcpuusage_CMakeLists.txt COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/libcpuusage_CMakeLists.txt DESTINATION share/cpuusage/src)

# Cpuusage library header
configure_file(src/cpuusage.h ${CMAKE_CURRENT_BINARY_DIR}/include/cpuusage.h COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/include/cpuusage.h DESTINATION include)

include_directories("src")

# Cpuusage library build
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID MATCHES "(Clang|GNU)")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -pedantic")
endif ()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_library(${PROJECT_NAME} SHARED src/cumain.cpp src/cpuusage.h)
add_library(${PROJECT_NAME}static STATIC src/cumain.cpp src/cpuusage.h)
set_property(TARGET ${PROJECT_NAME}static PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries(${PROJECT_NAME}static dl)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Cpuusageproc library build
add_library(${PROJECT_NAME}proc SHARED src/cupmain.cpp)
target_link_libraries(${PROJECT_NAME}proc dl)
install(TARGETS ${PROJECT_NAME}proc DESTINATION lib)

# Example - ex001 - regular program / non-instrumented
add_executable(ex001 tests/ex001.c)
target_link_libraries(ex001 pthread)

# Example - ex002 - instrumented
add_executable(ex002 tests/ex002.c)
set_target_properties(ex002 PROPERTIES COMPILE_FLAGS "-rdynamic -finstrument-functions")
target_link_libraries(ex002 pthread cpuusage)

# Example - ex003 - manually instrumented
add_executable(ex003 tests/ex003.cpp src/cpuusage.h)
target_link_libraries(ex003 pthread cpuusage)

# Example - ex004 - simple program (sleep)
add_executable(ex004 tests/ex004.cpp)
target_link_libraries(ex004)

# Example - ex005 - simple scripts
configure_file(tests/ex005.sh ${CMAKE_CURRENT_BINARY_DIR}/ex005.sh COPYONLY)
configure_file(tests/ex005b.sh ${CMAKE_CURRENT_BINARY_DIR}/ex005b.sh COPYONLY)

# Tests
enable_testing()

configure_file(tests/test001 ${CMAKE_CURRENT_BINARY_DIR}/test001 COPYONLY)
add_test(test001 "${PROJECT_BINARY_DIR}/test001")

configure_file(tests/test002 ${CMAKE_CURRENT_BINARY_DIR}/test002 COPYONLY)
add_test(test002 "${PROJECT_BINARY_DIR}/test002")

configure_file(tests/test003 ${CMAKE_CURRENT_BINARY_DIR}/test003 COPYONLY)
add_test(test003 "${PROJECT_BINARY_DIR}/test003")

configure_file(tests/test004 ${CMAKE_CURRENT_BINARY_DIR}/test004 COPYONLY)
add_test(test004 "${PROJECT_BINARY_DIR}/test004")

if(UNIX AND NOT APPLE)
  configure_file(tests/test005 ${CMAKE_CURRENT_BINARY_DIR}/test005 COPYONLY)
  add_test(test005 "${PROJECT_BINARY_DIR}/test005")
endif()

