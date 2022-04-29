#+++
# 
# Copyright (c) 2021 JulesIMF, MIPT
#
# Module name:
#
#		CMakeLists.txt
#
# Abstract:
#
#		My default CMake. Allows to compile the project
#		changing only FILES (and other flags if needed).
#		
#		Build this project with "make" or "make jimp".#		
#
#---

#
# Settings
#

cmake_minimum_required(VERSION 3.16)
project(containers LANGUAGES CXX)
# set(CMAKE_CXX_COMPILER )
set(CMAKE_CXX_STANDARD 17)
set(WARNING_FLAGS "-Wno-multichar -Wno-unused-result -Wno-narrowing")
set (CMAKE_EXPORT_COMPILE_COMMANDS 1)

#
# Definitions
#

# add_definitions(NDEBUG)

set(OPT "-O2")
set(SANITIZE 1)

if(SANITIZE)
    set(ASAN "-fsanitize=address")
else()
    set(ASAN "")
endif()

set(CMAKE_CXX_FLAGS "${OPT} ${WARNING_FLAGS} ${ASAN} ${NOELIDE} -g")
	
#
# Building
#

add_subdirectory(debug)

include_directories(debug/includes)
find_library(dbg libdbg.a bin)

add_executable(array_dbg
        array_dbg.cpp
)

target_link_libraries(array_dbg dbg)

add_executable(play
        play.cpp
)

target_link_libraries(play dbg)