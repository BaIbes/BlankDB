cmake_minimum_required(VERSION 3.10)
project(BlankDB LANGUAGES CXX)

# Устанавливаем стандарт C++ (например, C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(cmake/CPM.cmake)
# Добавляем каталог с заголовками
include_directories(${CMAKE_SOURCE_DIR}/include)

# Собираем список исходников для библиотеки BlankDB
set(LIB_SOURCES
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

# Создаем статическую библиотеку BlankDB
add_library(BlankDB STATIC ${LIB_SOURCES})
target_include_directories(BlankDB PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    $<BUILD_INTERFACE:${nlohmann_json_SOURCE_DIR}/single_include>
)

CPMAddPackage("gh:nlohmann/json@3.10.5")

target_link_libraries(BlankDB PUBLIC nlohmann_json::nlohmann_json)

# Создаем консольное приложение
add_executable(BlankDBSample samples/basic_usage.cpp)

# 
# Связываем библиотеку BlankDB с приложением
target_link_libraries(BlankDBSample PRIVATE BlankDB)