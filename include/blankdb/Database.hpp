#ifndef BLANKDB_DATABASE_HPP
#define BLANKDB_DATABASE_HPP

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

namespace blankdb {

class Database {
public:

    explicit Database(const std::string& db_path);
    ~Database();

    void open();
    void close();
    bool is_open() const noexcept;

    void create_table(
        const std::string& table_name,
        const std::unordered_map<std::string, std::string>& schema
    );
    
    void drop_table(const std::string& table_name);
    std::vector<std::string> list_tables() const;

    uint64_t insert(
        const std::string& table_name,
        const std::unordered_map<std::string, std::string>& record
    );
    
    std::vector<std::unordered_map<std::string, std::string>> select(
        const std::string& table_name,
        const std::vector<std::string>& columns = {},
        const std::string& where_clause = ""
    ) const;

    void begin_transaction();
    void commit();
    void rollback();

    void create_index(
        const std::string& table_name,
        const std::string& column_name
    );
    
    void drop_index(const std::string& index_name);

    class QueryResult;
    QueryResult execute(const std::string& query);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}

#endif