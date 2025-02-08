#include "../include/blankdb/Database.hpp"
#include "../include/blankdb/utils/Error.hpp"
#include "../include/blankdb/QueryEngine.hpp"

namespace blankdb {

// Конструктор Database
Database::Database(const std::string& db_path)
    : impl_(std::make_unique<Impl>(db_path)) {}

// Деструктор Database
Database::~Database() = default;

// Метод для открытия базы данных
void Database::open() {
    impl_->storage.connect();
    impl_->logger.info("Database opened: " + impl_->storage.get_path());
}

// Метод для закрытия базы данных
void Database::close() {
    if (impl_->transaction_active) {
        rollback();
    }
    impl_->storage.disconnect();
    impl_->logger.info("Database closed");
}

// Проверка состояния подключения
bool Database::is_open() const noexcept {
    return impl_->storage.is_connected();
}

// Создание таблицы
void Database::create_table(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& schema) {
    begin_transaction();
    try {
        impl_->storage.create_table(table_name, schema);
        impl_->indexer.register_table(table_name); // Регистрация таблицы для индексирования
        commit();
    } catch (const StorageException& e) {
        impl_->logger.error("Table creation failed: " + std::string(e.what()));
        rollback();
        throw;
    }
}

// Вставка записи
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

// Выполнение запроса
std::vector<std::unordered_map<std::string, std::string>> Database::execute_query(const std::string& query) {
    return impl_->query_engine.execute(query);
}

// Начало транзакции
void Database::begin_transaction() {
    if (!impl_->transaction_active) {
        impl_->storage.start_transaction();
        impl_->transaction_active = true;
    }
}

// Фиксация транзакции
void Database::commit() {
    if (impl_->transaction_active) {
        impl_->storage.commit();
        impl_->transaction_active = false;
    }
}

// Откат транзакции
void Database::rollback() {
    if (impl_->transaction_active) {
        impl_->storage.rollback();
        impl_->transaction_active = false;
    }
}

} // namespace blankdb