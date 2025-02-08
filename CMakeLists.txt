cmake_minimum_required(VERSION 3.15)
project(BlankDB LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(EXISTS "${CMAKE_SOURCE_DIR}/_deps/nlohmann_json-src/single_include/nlohmann/json.hpp")
    set(nlohmann_json_SOURCE_DIR "${CMAKE_SOURCE_DIR}/_deps/nlohmann_json-src")
    set(nlohmann_json_BINARY_DIR "${CMAKE_SOURCE_DIR}/_deps/nlohmann_json-build")
    set(nlohmann_json_POPULATED TRUE CACHE BOOL "nlohmann_json is already populated" FORCE)
else()
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.2
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()

if(NOT TARGET BlankDB)
    add_library(BlankDB STATIC
        src/Database.cpp
        src/StorageManager.cpp
        src/IndexManager.cpp
        src/QueryEngine.cpp
        src/cli/Parser.cpp
        src/cli/Interface.cpp
        src/utils/Serialization.cpp
        src/utils/Logger.cpp
        src/index/BTreeNode.cpp
    )

    target_include_directories(BlankDB PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/single_include>
    )
    target_link_libraries(BlankDB PUBLIC nlohmann_json::nlohmann_json)
endif()

add_executable(BlankDBSample samples/basic_usage.cpp)
target_link_libraries(BlankDBSample PRIVATE BlankDB)