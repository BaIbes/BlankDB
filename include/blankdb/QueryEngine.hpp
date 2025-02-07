#ifndef BLANKDB_QUERY_ENGINE_HPP
#define BLANKDB_QUERY_ENGINE_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "StorageManager.hpp"
#include "IndexManager.hpp"

namespace blankdb {
class QueryEngine {
public:
    explicit QueryEngine(StorageManager& storage, IndexManager& indexer);

    // Execute a query and return the result
    std::vector<std::unordered_map<std::string, std::string>> execute(const std::string& query);

private:
    StorageManager& storage_;
    IndexManager& indexer_;

    // Helper methods for parsing and execution
    void create_table(const std::string& query);
    uint64_t insert_record(const std::string& query);
    std::vector<std::unordered_map<std::string, std::string>> select_records(const std::string& query);

    // Utility method to split strings
    static std::vector<std::string> split(const std::string& str, char delimiter);
};
} // namespace blankdb

#endif