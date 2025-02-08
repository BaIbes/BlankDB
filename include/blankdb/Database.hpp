#ifndef BLANKDB_DATABASE_HPP
#define BLANKDB_DATABASE_HPP

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "StorageManager.hpp"
#include "IndexManager.hpp"
#include "QueryEngine.hpp" // Включение QueryEngine
#include "utils/Logger.hpp"

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
        const std::unordered_map<std::string, std::string>& schema);

    void drop_table(const std::string& table_name);
    std::vector<std::string> list_tables() const;

    uint64_t insert(
        const std::string& table_name,
        const std::unordered_map<std::string, std::string>& record);

    std::vector<std::unordered_map<std::string, std::string>> execute_query(const std::string& query);

    void begin_transaction();
    void commit();
    void rollback();

    void create_index(const std::string& table_name, const std::string& column_name);
    void drop_index(const std::string& table_name, const std::string& column_name);

private:
    class Impl {
    public:
        Impl(const std::string& path)
            : storage(path),
              indexer(storage),
              query_engine(storage, indexer),
              logger(Logger::get_instance()),
              transaction_active(false) {}

        ~Impl() {
            if (storage.is_connected()) {
                storage.disconnect();
            }
        }

        StorageManager storage;
        IndexManager indexer;
        QueryEngine query_engine; // Добавляем QueryEngine
        Logger& logger;
        bool transaction_active = false;
    };

    std::unique_ptr<Impl> impl_;
};

} // namespace blankdb

#endif // BLANKDB_DATABASE_HPP