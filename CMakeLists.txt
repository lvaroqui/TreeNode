cmake_minimum_required(VERSION 3.0)
project(TreeNode VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

### Google Test

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main gmock and gmock_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

### TreeNode lib
add_library(TreeNode INTERFACE)
target_include_directories(TreeNode INTERFACE include/)

### Tests
enable_testing()
add_executable(TreeNodeTest tests/TreeNodeTest.cpp)
target_link_libraries(TreeNodeTest TreeNode gmock_main)
add_test(NAME TreeNodeTest COMMAND TreeNodeTest)

### Examples
add_executable(TreeNodeExample examples/main.cpp)
target_link_libraries(TreeNodeExample TreeNode)
