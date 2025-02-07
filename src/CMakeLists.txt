# Создание статической библиотеки для BlankDB
add_library(BlankDB STATIC
    Database.cpp
    StorageManager.cpp
    IndexManager.cpp
    QueryEngine.cpp
    cli/Parser.cpp
    cli/Interface.cpp
    utils/Serialization.cpp
    utils/Logger.cpp
    index/BTree.cpp
)

# Включение директории заголовочных файлов
target_include_directories(BlankDB PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Зависимости
target_link_libraries(BlankDB PRIVATE
    nlohmann_json::nlohmann_json
)

# Добавление исполняемого файла для демонстрации
add_executable(BlankDBSample samples/basic_usage.cpp)
target_link_libraries(BlankDBSample PRIVATE BlankDB)