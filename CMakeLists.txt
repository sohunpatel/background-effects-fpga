cmake_minimum_required(VERSION 3.25)

project(background-effects 
    LANGUAGES C
    VERSION 0.1.0
    DESCRIPTION "Implementation of background image processing at the webcam")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(x86)
