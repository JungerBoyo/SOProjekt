cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)

project(CpSync

  VERSION 0.0.1
  DESCRIPTION "simple daemon program for dir synchronization"
  HOMEPAGE_URL ""
  LANGUAGES C
)

set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(config)
add_subdirectory(src)
