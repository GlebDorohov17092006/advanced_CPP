cmake_minimum_required(VERSION 3.14)
project(TypeMapProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(type_map_tests tests.cpp)

target_include_directories(type_map_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

if(MSVC)
    target_compile_options(type_map_tests PRIVATE /W4 /WX)
else()
    target_compile_options(type_map_tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

add_library(typemap INTERFACE)
target_include_directories(typemap INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(type_map_tests typemap)

enable_testing()
add_test(NAME TypeMapTests COMMAND type_map_tests)