CMAKE_MINIMUM_REQUIRED( VERSION 2.8.5 )

# Project Declaration
project(ngengine-network)

# enable c++11
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-m64 -Wall -Wno-unused-function)
link_directories(../../externals/jsoncpp/src/lib_json/)
link_directories(../../externals/yaml-cpp/build/)

# Get orm source files

file(
  GLOB_RECURSE orm_source_files 
  # ngengine-network
  ../../src/ngengine/*
)

include_directories(
  ../../include/
  /usr/dev/include/
)

link_directories(
  /usr/dev/lib
)

add_library(ngengine-network STATIC ${orm_source_files})

install(TARGETS ngengine-network
        RUNTIME DESTINATION /usr/dev/bin
        LIBRARY DESTINATION /usr/dev/lib
        ARCHIVE DESTINATION /usr/dev/lib)

install(DIRECTORY ../../include/ngengine DESTINATION /usr/dev/include)

install(DIRECTORY ../../include/ngengine DESTINATION /usr/dev/include)

add_custom_command(TARGET ngengine-network POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy libngengine-network.a ../../bin/libngengine-network.a
)

file(MAKE_DIRECTORY ../../bin)

# TESTS AND TEST RUNNER

MACRO( TEST _target _target_folder )
  file(
    GLOB_RECURSE _sources
    ../../tests/tests/${_target_folder}/*.cpp
  )
  add_executable(${_target} ${_sources})
  target_include_directories(${_target} PUBLIC ../../tests/tests/${_target_folder})
  
  target_link_libraries(${_target} ngengine-network)
  target_link_libraries(${_target} ngengine-entity)
  target_link_libraries(${_target} sqlite3)
  target_link_libraries(${_target} pthread)

  add_custom_command(TARGET ${_target} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy ${_target} ../../bin/tests/${_target_folder}/test
  )

ENDMACRO( TEST )

TEST(entity entity)

# TEST RUNNER

file(
    GLOB_RECURSE _sources_runner
    ../../tests/runner/*.cpp
  )
add_executable( test_runner ${_sources_runner})

add_custom_command(TARGET test_runner POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy test_runner ../../bin/test-runner
)

# nge-orm-utils

#file(
#  GLOB_RECURSE nge_orm_utils_source_files 
#  # ngengine-network
#  ../../src/nge-orm-utils/*
#)

#add_executable( nge-orm-utils ${nge_orm_utils_source_files})

# jsoncpp
#target_include_directories(nge-orm-utils PUBLIC ../../externals/jsoncpp/include/)
#target_link_libraries(nge-orm-utils jsoncpp)

# yaml
#target_include_directories(nge-orm-utils PUBLIC ../../externals/yaml-cpp/include/)
#target_link_libraries(nge-orm-utils yaml-cpp)

#add_custom_command(TARGET nge-orm-utils POST_BUILD 
#  COMMAND ${CMAKE_COMMAND} -E copy nge-orm-utils ../../bin/nge-orm-utils
#)

