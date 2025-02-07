# Создание интерфейсной библиотеки для утилит
add_library(Utils INTERFACE)

# Включение директории заголовочных файлов
target_include_directories(Utils INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})