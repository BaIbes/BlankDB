#include "../include/blankdb/QueryEngine.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace blankdb {
QueryEngine::QueryEngine(StorageManager& storage, IndexManager& indexer)
    : storage_(storage), indexer_(indexer) {}

std::vector<std::unordered_map<std::string, std::string>> QueryEngine::execute(const std::string& query) {
    std::string trimmed_query = query;
    trimmed_query.erase(std::remove_if(trimmed_query.begin(), trimmed_query.end(), ::isspace), trimmed_query.end());

    if (trimmed_query.substr(0, 12) == "CREATETABLE") {
        create_table(query);
        return {};
    } else if (trimmed_query.substr(0, 6) == "INSERT") {
        insert_record(query);
        return {};
    } else if (trimmed_query.substr(0, 6) == "SELECT") {
        return select_records(query);
    } else {
        throw std::runtime_error("Unsupported query: " + query);
    }
}

void QueryEngine::create_table(const std::string& query) {
    size_t pos = query.find('(');
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid CREATE TABLE query: " + query);
    }

    std::string table_name = query.substr(12, pos - 12);
    std::string schema_str = query.substr(pos + 1, query.size() - pos - 2);

    std::unordered_map<std::string, std::string> schema;
    std::vector<std::string> columns = split(schema_str, ',');
    for (const auto& col : columns) {
        size_t colon_pos = col.find(':');
        if (colon_pos == std::string::npos) {
            throw std::runtime_error("Invalid column definition: " + col);
        }
        std::string col_name = col.substr(0, colon_pos);
        std::string col_type = col.substr(colon_pos + 1);
        schema[col_name] = col_type;
    }

    storage_.create_table(table_name, schema);
    indexer_.register_table(table_name);
}

uint64_t QueryEngine::insert_record(const std::string& query) {
    size_t pos = query.find('(');
    if (pos == std::string::npos) {
        throw std::runtime_error("Invalid INSERT query: " + query);
    }

    std::string table_name = query.substr(6, pos - 6);
    std::string values_str = query.substr(pos + 1, query.size() - pos - 2);

    std::unordered_map<std::string, std::string> record_data;
    std::vector<std::string> values = split(values_str, ',');
    for (const auto& value_pair : values) {
        size_t equals_pos = value_pair.find('=');
        if (equals_pos == std::string::npos) {
            throw std::runtime_error("Invalid value pair: " + value_pair);
        }
        std::string key = value_pair.substr(0, equals_pos);
        std::string value = value_pair.substr(equals_pos + 1);
        record_data[key] = value;
    }

    uint64_t record_id = storage_.write_record(table_name, record_data);
    indexer_.update_indexes(table_name, record_id, record_data);
    return record_id;
}

std::vector<std::unordered_map<std::string, std::string>> QueryEngine::select_records(const std::string& query) {
    size_t from_pos = query.find("FROM");
    if (from_pos == std::string::npos) {
        throw std::runtime_error("Invalid SELECT query: " + query);
    }

    std::string table_name = query.substr(from_pos + 4);
    table_name.erase(std::remove_if(table_name.begin(), table_name.end(), ::isspace), table_name.end());

    // For simplicity, assume all records are selected
    std::vector<uint64_t> record_ids = {0}; // Replace with actual logic
    return storage_.read_records(table_name, record_ids);
}

std::vector<std::string> QueryEngine::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
} // namespace blankdb