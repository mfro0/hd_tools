cmake_minimum_required(VERSION 3.5)

project(partinit LANGUAGES C)

add_executable(partinit partinit.c atari_rootsect.h)

install(TARGETS partinit
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
