#ifndef BLANKDB_INDEX_MANAGER_HPP
#define BLANKDB_INDEX_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <memory>
#include <stdexcept>

namespace blankdb {
class StorageManager;

class IndexManager {
public:
    explicit IndexManager(StorageManager& storage);

    // Register a table for potential indexing
    void register_table(const std::string& table_name);

    // Create an index on a column of a table
    void create_index(const std::string& table_name, const std::string& column_name);

    // Drop an index on a column of a table
    void drop_index(const std::string& table_name, const std::string& column_name);

    // Update indexes when a record is inserted
    void update_indexes(
        const std::string& table_name,
        uint64_t record_id,
        const std::unordered_map<std::string, std::string>& record_data);

    // Query records using an index
    std::vector<uint64_t> query_by_index(
        const std::string& table_name,
        const std::string& column_name,
        const std::string& value) const;

private:
    struct Index {
        std::unordered_map<std::string, std::vector<uint64_t>> data;
    };

    StorageManager& storage_;
    std::unordered_map<std::string, std::unordered_map<std::string, Index>> indexes_;

    bool has_index(const std::string& table_name, const std::string& column_name) const;
};
} // namespace blankdb

#endif