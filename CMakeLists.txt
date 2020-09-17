cmake_minimum_required(VERSION 2.8)

project(flogout)

set( APP flogout)

find_package(FLTK REQUIRED)

file( GLOB SOURCES "src/*.c*" "src/*.h*" )

set (CMAKE_CXX_FLAGS "-Ofast -std=c++11 -s" )

if( FLTK_FOUND )
    add_executable( ${APP} ${SOURCES} )
    target_link_libraries( ${APP} ${FLTK_LIBRARIES} )

endif()

