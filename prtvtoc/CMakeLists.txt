cmake_minimum_required(VERSION 3.5)

project(prtvtoc LANGUAGES C)

add_executable(prtvtoc prtvtoc.c)

install(TARGETS prtvtoc
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
