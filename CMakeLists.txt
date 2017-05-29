cmake_minimum_required(VERSION 3.1)

# general build setup
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
if (CMAKE_CXX_COMPILER_ID MATCHES "(Clang|GNU)")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -pedantic")
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -pedantic")
endif ()

# cpuusage library
project (cpuusage)
file(GLOB SOURCES src/cumain.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_library(${PROJECT_NAME}static STATIC ${SOURCES})
set_property(TARGET ${PROJECT_NAME}static PROPERTY POSITION_INDEPENDENT_CODE 1)
target_link_libraries(${PROJECT_NAME} dl)
target_link_libraries(${PROJECT_NAME}static dl)
install(TARGETS ${PROJECT_NAME} DESTINATION lib)

# cpuusage example - autogenerate instrumented wrapper for exwrapper.h functions
add_custom_command(OUTPUT ${CMAKE_SOURCE_DIR}/examples/exwrapper.c COMMAND "/usr/bin/python"
  ${CMAKE_SOURCE_DIR}/src/genwrapper.py ${CMAKE_SOURCE_DIR}/examples/exwrapper.h
  ${CMAKE_SOURCE_DIR}/examples/exwrapper.c DEPENDS ${CMAKE_SOURCE_DIR}/examples/exwrapper.h)
add_library(exwrapper SHARED examples/exwrapper.c)
set_source_files_properties(examples/exwrapper.c PROPERTIES GENERATED TRUE)
target_link_libraries(exwrapper cpuusagestatic dl)
set_target_properties(exwrapper PROPERTIES COMPILE_FLAGS "-finstrument-functions")

# cpuusage example cutest1 - regular program / non-instrumented
add_executable(cutest1 examples/cutest.c)
target_link_libraries(cutest1 pthread)

# cpuusage example cutest2 - instrumented, statically linking cpuusage library 
add_executable(cutest2 examples/cutest.c)
set_target_properties(cutest2 PROPERTIES COMPILE_FLAGS "-rdynamic -finstrument-functions")
target_link_libraries(cutest2 pthread cpuusage)

# cpuusage example cutest3 - instrumented, not linking cpuusage library (needs library preload)
add_executable(cutest3 examples/cutest.c)
set_target_properties(cutest3 PROPERTIES COMPILE_FLAGS "-rdynamic -finstrument-functions")
target_link_libraries(cutest3 pthread cpuusage)

