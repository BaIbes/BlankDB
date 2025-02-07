#include "../include/blankdb/Database.hpp"
#include "../include/blankdb/StorageManager.hpp"
#include "../include/blankdb/IndexManager.hpp"
#include "../include/blankdb/utils/Logger.hpp"
#include "../include/blankdb/utils/Error.hpp"

namespace blankdb {
class Database::Impl {
public:
    Impl(const std::string& path)
        : storage(path),
          indexer(storage),
          logger(Logger::get_instance()),
          transaction_active(false) {}

    ~Impl() {
        if (storage.is_connected()) {
            storage.disconnect();
        }
    }

    StorageManager storage;
    IndexManager indexer;
    Logger& logger;
    bool transaction_active = false;
};

Database::Database(const std::string& db_path)
    : impl_(std::make_unique<Impl>(db_path)) {}

Database::~Database() = default;

void Database::create_table(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& schema) {
    begin_transaction();
    try {
        impl_->storage.create_table(table_name, schema);
        impl_->indexer.register_table(table_name); // Register the table for indexing
        commit();
    } catch (const StorageException& e) {
        impl_->logger.error("Table creation failed: " + std::string(e.what()));
        rollback();
        throw;
    }
}

uint64_t Database::insert(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& record) {
    begin_transaction();
    try {
        auto id = impl_->storage.write_record(table_name, record);
        impl_->indexer.update_indexes(table_name, id, record);
        commit();
        return id;
    } catch (const IndexException& e) {
        impl_->logger.error("Index update failed: " + std::string(e.what()));
        rollback();
        throw;
    }
}

void Database::begin_transaction() {
    if (!impl_->transaction_active) {
        impl_->storage.start_transaction();
        impl_->transaction_active = true;
    }
}

void Database::commit() {
    if (impl_->transaction_active) {
        impl_->storage.commit();
        impl_->transaction_active = false;
    }
}

void Database::rollback() {
    if (impl_->transaction_active) {
        impl_->storage.rollback();
        impl_->transaction_active = false;
    }
}
}