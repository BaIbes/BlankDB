cmake_minimum_required(VERSION 3.10)

# Название проекта
project(BlankDB LANGUAGES CXX)

# Установка языковой версии C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

# Подключение поддиректорий
add_subdirectory(include/blankdb/utils)
add_subdirectory(include/blankdb/index)
add_subdirectory(src)
add_subdirectory(tests)

# FetchContent для внешних библиотек
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.10.5
)
FetchContent_MakeAvailable(nlohmann_json)