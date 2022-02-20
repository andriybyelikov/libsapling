cmake_minimum_required(VERSION 3.10)

project(
  libsapling
  VERSION 0.8.0
  HOMEPAGE_URL "https://github.com/andriybyelikov/libsapling"
  LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

file(
  GLOB_RECURSE LIBRARY_SOURCES
  RELATIVE ${CMAKE_SOURCE_DIR}
  "src/*.c")
add_library(sapling STATIC ${LIBRARY_SOURCES})
target_include_directories(sapling PUBLIC include)
target_include_directories(sapling PRIVATE src)
target_link_libraries(sapling m)

include(GNUInstallDirs)
install(TARGETS sapling LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(DIRECTORY include/${PROJECT_NAME}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# tests
file(
  GLOB_RECURSE TEST_SOURCES
  RELATIVE ${CMAKE_SOURCE_DIR}
  "test/*.c")
enable_testing()
foreach(source ${TEST_SOURCES})
  # skip parser_test_utils.c
  if(${source} STREQUAL "test/cc/parser/parser_test_utils.c")
    continue()
  endif()

  get_filename_component(basename ${source} NAME_WE)
  string(CONCAT executable ${basename} ".out")
  get_filename_component(directory ${source} DIRECTORY)

  # add parser_test_utils.c to parser tests
  if(${directory} STREQUAL "test/cc/parser")
    add_executable(${executable} ${source} "test/cc/parser/parser_test_utils.c")
  else()
    add_executable(${executable} ${source})
  endif()
  target_include_directories(${executable} PRIVATE .)
  target_link_libraries(${executable} sapling)
  add_test(${basename} ${executable})
endforeach(source ${TEST_SOURCES})
