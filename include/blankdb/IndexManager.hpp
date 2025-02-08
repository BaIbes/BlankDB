#ifndef BLANKDB_INDEX_MANAGER_HPP
#define BLANKDB_INDEX_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector> // Для std::vector
#include <memory>
#include <stdexcept>

namespace blankdb {

// Forward declaration of StorageManager
class StorageManager;

class IndexManager {
public:
    explicit IndexManager(StorageManager& storage);

    // Регистрация таблицы для индексирования
    void register_table(const std::string& table_name);

    // Создание индекса на столбце таблицы
    void create_index(const std::string& table_name, const std::string& column_name);

    // Удаление индекса на столбце таблицы
    void drop_index(const std::string& table_name, const std::string& column_name);

    // Обновление индексов при вставке записи
    void update_indexes(
        const std::string& table_name,
        uint64_t record_id,
        const std::unordered_map<std::string, std::string>& record_data);

    // Запрос записей по индексу
    std::vector<uint64_t> query_by_index(
        const std::string& table_name,
        const std::string& column_name,
        const std::string& value) const;

private:
    // Структура для хранения индекса
    struct Index {
        std::unordered_map<std::string, std::vector<uint64_t>> data; // Ключ: значение столбца, Значение: список ID записей
    };

    StorageManager& storage_; // Ссылка на StorageManager
    std::unordered_map<std::string, std::unordered_map<std::string, Index>> indexes_; // Индексы для таблиц

    // Проверка существования индекса
    bool has_index(const std::string& table_name, const std::string& column_name) const;
};

} // namespace blankdb

#endif // BLANKDB_INDEX_MANAGER_HPP