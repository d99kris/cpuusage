# CMake
cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

# Extract resources
execute_process(COMMAND ${CMAKE_COMMAND} -E tar -xzvf ${CMAKE_SOURCE_DIR}/catapult.tar.gz)

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
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/catapult DESTINATION share/cpuusage)

# POSIX Wrappers
file(COPY posixwrap DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/posixwrap DESTINATION share/cpuusage)

# Cpuusage library source
configure_file(src/cumain.cpp ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/cumain.cpp COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/cumain.cpp DESTINATION share/cpuusage/src)

# Cpuusage library CMakeLists
configure_file(src/libcpuusage_CMakeLists.txt ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/libcpuusage_CMakeLists.txt COPYONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/cpuusage/src/libcpuusage_CMakeLists.txt DESTINATION share/cpuusage/src)

# Cpuusage library build
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID MATCHES "(Clang|GNU)")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -pedantic")
endif ()
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
add_library(${PROJECT_NAME} SHARED src/cumain.cpp)
add_library(${PROJECT_NAME}static STATIC src/cumain.cpp)
set_property(TARGET ${PROJECT_NAME}static PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries(${PROJECT_NAME}static dl)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# Example - cutest1 - regular program / non-instrumented
add_executable(cutest1 tests/cutest.c)
target_link_libraries(cutest1 pthread)

# Example - cutest2 - instrumented
add_executable(cutest2 tests/cutest.c)
set_target_properties(cutest2 PROPERTIES COMPILE_FLAGS "-rdynamic -finstrument-functions")
target_link_libraries(cutest2 pthread)

