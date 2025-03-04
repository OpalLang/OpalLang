cmake_minimum_required(VERSION 3.22)

project(
    Opal
    VERSION 0.1.0
    DESCRIPTION "Opal is a programming language"
    LANGUAGES CXX
)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/build)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.hpp
    ${PROJECT_SOURCE_DIR}/src/*.h
)

list(FILTER SOURCES EXCLUDE REGEX ".*Main\\.cpp$")

if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ${PROJECT_SOURCE_DIR}/src/")
endif()

set(INTERFACE_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include")
file(MAKE_DIRECTORY ${INTERFACE_INCLUDE_DIR})
file(MAKE_DIRECTORY "${INTERFACE_INCLUDE_DIR}/opal")

file(GLOB SRC_SUBDIRS RELATIVE "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/*")
foreach(SUBDIR ${SRC_SUBDIRS})
    if(IS_DIRECTORY "${PROJECT_SOURCE_DIR}/src/${SUBDIR}")
        file(CREATE_LINK "${PROJECT_SOURCE_DIR}/src/${SUBDIR}" "${INTERFACE_INCLUDE_DIR}/opal/${SUBDIR}" SYMBOLIC)
    endif()
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${INTERFACE_INCLUDE_DIR})

add_library(opal_lib STATIC ${SOURCES})
target_include_directories(opal_lib PUBLIC 
    ${PROJECT_SOURCE_DIR}/src
    ${INTERFACE_INCLUDE_DIR}
)

target_link_libraries(opal_lib PUBLIC spdlog::spdlog fmt::fmt)

add_executable(opal ${PROJECT_SOURCE_DIR}/src/Main.cpp)
target_link_libraries(opal PRIVATE opal_lib)

file(GLOB_RECURSE TEST_SOURCES
    ${PROJECT_SOURCE_DIR}/tests/*.cpp
    ${PROJECT_SOURCE_DIR}/tests/*.hpp
    ${PROJECT_SOURCE_DIR}/tests/*.h
)

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE opal_lib GTest::gtest_main GTest::gmock)
target_include_directories(tests PRIVATE 
    ${PROJECT_SOURCE_DIR}/src
    ${INTERFACE_INCLUDE_DIR}
)

include(GoogleTest)
gtest_discover_tests(tests)

install(TARGETS opal DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/include)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/src)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Sources found: ${SOURCES}")
message(STATUS "Test sources found: ${TEST_SOURCES}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Binary output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
