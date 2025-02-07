# Создание интерфейсной библиотеки для индекса
add_library(Index INTERFACE)

# Включение директории заголовочных файлов
target_include_directories(Index INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})