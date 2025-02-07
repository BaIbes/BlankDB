#include "../include/blankdb/Database.hpp"
#include "../include/blankdb/StorageManager.hpp"
#include "../include/blankdb/IndexManager.hpp"
#include "../include/blankdb/utils/Logger.hpp"

namespace blankdb {
class Database::Impl {
public:
    Impl(const std::string& path)
        : storage(path),
          indexer(storage),
          logger(Logger::get_instance()),
          transaction_active(false) {}

    StorageManager storage;
    IndexManager indexer;
    Logger& logger;
    bool transaction_active = false;

    // Ensure proper cleanup
    ~Impl() {
        if (storage.is_connected()) {
            storage.disconnect();
        }
    }
};

Database::Database(const std::string& db_path)
    : impl_(std::make_unique<Impl>(db_path)) {}

Database::~Database() = default;

void Database::open() {
    if (!impl_->storage.is_connected()) {
        impl_->storage.connect();
        impl_->logger.log("Database opened: " + impl_->storage.get_path());
    }
}

void Database::close() {
    if (impl_->transaction_active) {
        rollback();
    }
    impl_->storage.disconnect();
    impl_->logger.log("Database closed");
}

bool Database::is_open() const noexcept {
    return impl_->storage.is_connected();
}

void Database::create_table(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& schema) {
    begin_transaction();
    try {
        impl_->storage.create_table(table_name, schema);
        impl_->indexer.register_table(table_name);
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
} // namespace blankdb