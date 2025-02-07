#ifndef BLANKDB_STORAGE_MANAGER_HPP
#define BLANKDB_STORAGE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include "./utils/Error.hpp" // Include the custom exception definition

namespace blankdb {
class StorageManager {
public:
    struct TableSchema {
        std::string name;
        std::unordered_map<std::string, std::string> columns;
    };

    explicit StorageManager(const std::string& db_path);
    ~StorageManager();

    void connect();
    void disconnect();
    bool is_connected() const noexcept;

    void create_table(
        const std::string& name,
        const std::unordered_map<std::string, std::string>& schema);

    void drop_table(const std::string& name);
    bool table_exists(const std::string& name) const;
    TableSchema get_table_schema(const std::string& name) const;

    uint64_t write_record(
        const std::string& table_name,
        const std::unordered_map<std::string, std::string>& data);

    std::vector<std::unordered_map<std::string, std::string>> read_records(
        const std::string& table_name,
        const std::vector<uint64_t>& record_ids) const;

    void start_transaction();
    void commit();
    void rollback();
    void flush_to_disk();
    size_t get_table_size(const std::string& name) const;

    std::string get_path() const; // Added method to get the database path

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
} // namespace blankdb

#endif