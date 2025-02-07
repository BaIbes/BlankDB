#include "../include/blankdb/IndexManager.hpp"
#include "../include/blankdb/StorageManager.hpp"

namespace blankdb {
IndexManager::IndexManager(StorageManager& storage)
    : storage_(storage) {}

void IndexManager::register_table(const std::string& table_name) {
    if (indexes_.count(table_name) == 0) {
        indexes_[table_name] = {}; // Initialize an empty map for indexes
    }
}

void IndexManager::create_index(const std::string& table_name, const std::string& column_name) {
    if (!storage_.table_exists(table_name)) {
        throw std::runtime_error("Table does not exist: " + table_name);
    }

    if (has_index(table_name, column_name)) {
        throw std::runtime_error("Index already exists for column: " + column_name + " in table: " + table_name);
    }

    indexes_[table_name][column_name] = Index();
}

void IndexManager::drop_index(const std::string& table_name, const std::string& column_name) {
    if (!has_index(table_name, column_name)) {
        throw std::runtime_error("Index does not exist for column: " + column_name + " in table: " + table_name);
    }

    indexes_[table_name].erase(column_name);

    if (indexes_[table_name].empty()) {
        indexes_.erase(table_name);
    }
}

void IndexManager::update_indexes(
    const std::string& table_name,
    uint64_t record_id,
    const std::unordered_map<std::string, std::string>& record_data) {
    if (!indexes_.count(table_name)) {
        return; // No indexes for this table, nothing to update
    }

    for (const auto& [column_name, index] : indexes_[table_name]) {
        auto it = record_data.find(column_name);
        if (it != record_data.end()) {
            const std::string& value = it->second;
            indexes_[table_name][column_name].data[value].push_back(record_id);
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

    const auto& index = indexes_.at(table_name).at(column_name).data;
    auto it = index.find(value);

    if (it == index.end()) {
        return {};
    }

    return it->second;
}

bool IndexManager::has_index(const std::string& table_name, const std::string& column_name) const {
    return indexes_.count(table_name) && indexes_.at(table_name).count(column_name);
}
} // namespace blankdb