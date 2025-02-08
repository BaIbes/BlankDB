#include "../include/blankdb/IndexManager.hpp"
#include "../include/blankdb/StorageManager.hpp" // Включение StorageManager
#include <stdexcept> // Для std::runtime_error

namespace blankdb {

IndexManager::IndexManager(StorageManager& storage)
    : storage_(storage) {}

void IndexManager::register_table(const std::string& table_name) {
    if (indexes_.count(table_name) == 0) {
        indexes_[table_name] = {}; // Инициализация пустой карты индексов
    }
}

void IndexManager::create_index(const std::string& table_name, const std::string& column_name) {
    if (!storage_.table_exists(table_name)) {
        throw std::runtime_error("Table does not exist: " + table_name);
    }

    if (has_index(table_name, column_name)) {
        throw std::runtime_error("Index already exists for column: " + column_name + " in table: " + table_name);
    }

    indexes_[table_name][column_name] = Index(); // Создание нового индекса
}

void IndexManager::drop_index(const std::string& table_name, const std::string& column_name) {
    if (!has_index(table_name, column_name)) {
        throw std::runtime_error("Index does not exist for column: " + column_name + " in table: " + table_name);
    }

    indexes_[table_name].erase(column_name); // Удаление индекса

    if (indexes_[table_name].empty()) {
        indexes_.erase(table_name); // Если больше нет индексов для таблицы, удаляем ее
    }
}

void IndexManager::update_indexes(
    const std::string& table_name,
    uint64_t record_id,
    const std::unordered_map<std::string, std::string>& record_data) {
    if (!indexes_.count(table_name)) {
        return; // Нет индексов для этой таблицы
    }

    for (const auto& [column_name, index] : indexes_[table_name]) {
        auto it = record_data.find(column_name);
        if (it != record_data.end()) {
            const std::string& value = it->second;
            indexes_[table_name][column_name].data[value].push_back(record_id); // Обновление индекса
        }
    }
}

std::vector<uint64_t> IndexManager::query_by_index(
    const std::string& table_name,
    const std::string& column_name,
    const std::string& value) const {
    if (!has_index(table_name, column_name)) {
        throw std::runtime_error("Index does not exist for column: " + column_name + " in table: " + table_name);
    }

    const auto& index = indexes_.at(table_name).at(column_name).data; // Получаем данные индекса
    auto it = index.find(value);

    if (it == index.end()) {
        return {}; // Записей с таким значением нет
    }

    return it->second; // Возвращаем список ID записей
}

bool IndexManager::has_index(const std::string& table_name, const std::string& column_name) const {
    return indexes_.count(table_name) && indexes_.at(table_name).count(column_name);
}

} // namespace blankdb