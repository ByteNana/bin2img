cmake_minimum_required(VERSION 3.15...3.31)

message(STATUS "Starting building makefiles...")

project(
  BINIMG32
  VERSION 0.1.0
  LANGUAGES CXX
)

set(EXECUTABLE_OUTPUT_PATH "../")

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

add_executable(
  BINIMG32
  ${SOURCE_FILES}
)

target_include_directories(
  BINIMG32 PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

set_target_properties(
  BINIMG32 PROPERTIES
  CXX_STANDARD 17
)

message(STATUS "Ok!")
