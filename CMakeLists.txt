cmake_minimum_required(VERSION 3.14)
project(workshop CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

add_library(workshop_lib STATIC src/Parser.cpp src/Simulator.cpp)
target_include_directories(workshop_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(workshop src/main.cpp)
target_link_libraries(workshop PRIVATE workshop_lib)

option(WORKSHOP_BUILD_TESTS "Build GoogleTest unit tests" ON)

if(WORKSHOP_BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  set(WORKSHOP_EXAMPLES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/examples")
  string(REPLACE "\\" "/" WORKSHOP_EXAMPLES_PATH "${WORKSHOP_EXAMPLES_PATH}")
  configure_file(tests/test_paths.hpp.in "${CMAKE_BINARY_DIR}/generated/test_paths.hpp" @ONLY)

  add_executable(workshop_tests tests/test_parser.cpp tests/test_simulator.cpp)
  target_include_directories(workshop_tests PRIVATE "${CMAKE_BINARY_DIR}/generated")
  target_link_libraries(workshop_tests PRIVATE workshop_lib GTest::gtest_main)

  enable_testing()
  include(GoogleTest)
  gtest_discover_tests(workshop_tests)
endif()
