#include "../include/blankdb/Database.hpp"
#include "../include/blankdb/StorageManager.hpp"
#include "../include/blankdb/IndexManager.hpp"
#include "../include/blankdb/utils/Logger.hpp"


namespace blankdb {

class Database::Impl {
public:
    Impl(const std::string& path) 
        : storage(path), 
          logger(Logger::get_instance()) {}
    
    StorageManager storage;
    IndexManager indexer;
    Logger& logger;
    bool transaction_active = false;
};

Database::Database(const std::string& db_path) 
    : impl_(std::make_unique<Impl>(db_path)) {}

Database::~Database() {
    if(impl_->storage.is_connected()) {
        close();
    }
}

void Database::open() {
    impl_->storage.connect();
    impl_->logger.log("Database opened: " + impl_->storage.get_path());
}

void Database::close() {
    if(impl_->transaction_active) {
        rollback();
    }
    impl_->storage.flush();
    impl_->logger.log("Database closed");
}

bool Database::is_open() const noexcept {
    return impl_->storage.is_connected();
}

void Database::create_table(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& schema)
{
    if(!impl_->transaction_active) {
        begin_transaction();
    }
    
    try {
        impl_->storage.create_table(table_name, schema);
        impl_->indexer.register_table(table_name);
    } catch(const StorageException& e) {
        impl_->logger.error("Table creation failed: " + std::string(e.what()));
        rollback();
        throw;
    }
}

uint64_t Database::insert(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& record)
{
    if(!impl_->transaction_active) {
        begin_transaction();
    }
    
    try {
        auto id = impl_->storage.write_record(table_name, record);
        impl_->indexer.update_indexes(table_name, id, record);
        return id;
    } catch(const IndexException& e) {
        impl_->logger.error("Index update failed: " + std::string(e.what()));
        rollback();
        throw;
    }
}

void Database::begin_transaction() {
    impl_->storage.start_transaction();
    impl_->transaction_active = true;
}

void Database::commit() {
    impl_->storage.commit();
    impl_->transaction_active = false;
}

void Database::rollback() {
    impl_->storage.rollback();
    impl_->transaction_active = false;
}

// Реализации остальных методов...

}