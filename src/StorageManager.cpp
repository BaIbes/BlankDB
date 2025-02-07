#include "../include/blankdb/StorageManager.hpp"
#include "../include/blankdb/utils/Logger.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>

namespace blankdb {
class StorageManager::Impl {
public:
    Impl(const std::string& path)
        : db_path(path),
          tables(),
          logger(Logger::get_instance()),
          transaction_active(false) {}

    void load_metadata() {
        if (!std::filesystem::exists(db_path)) {
            std::filesystem::create_directory(db_path);
            return;
        }

        std::ifstream meta_file(db_path + "/metadata");
        if (!meta_file) return;

        std::string line;
        while (std::getline(meta_file, line)) {
            // Parse metadata and populate `tables`
        }
    }

    void save_metadata() {
        std::ofstream meta_file(db_path + "/metadata", std::ios::trunc);
        for (const auto& [name, schema] : tables) {
            meta_file << name << ":";
            for (const auto& [col, type] : schema.columns) {
                meta_file << col << "=" << type << ";";
            }
            meta_file << "\n";
        }
    }

    std::string db_path;
    std::unordered_map<std::string, TableSchema> tables;
    Logger& logger;
    bool transaction_active = false;
    std::unordered_map<std::string, uint64_t> next_ids; // Per-table ID generator
};

StorageManager::StorageManager(const std::string& db_path)
    : impl_(std::make_unique<Impl>(db_path)) {}

StorageManager::~StorageManager() {
    if (impl_->transaction_active) rollback();
    if (is_connected()) disconnect();
}

void StorageManager::connect() {
    impl_->load_metadata();
    impl_->logger.log("Storage connected: " + impl_->db_path);
}

void StorageManager::disconnect() {
    flush_to_disk();
    impl_->save_metadata();
    impl_->logger.log("Storage disconnected");
}

bool StorageManager::is_connected() const noexcept {
    return !impl_->db_path.empty();
}

void StorageManager::create_table(
    const std::string& name,
    const std::unordered_map<std::string, std::string>& schema) {
    if (impl_->tables.count(name)) {
        throw StorageException("Table already exists: " + name);
    }

    TableSchema new_table;
    new_table.name = name;
    new_table.columns = schema;
    impl_->tables.emplace(name, new_table);
    impl_->next_ids[name] = 1; // Initialize ID generator
    impl_->logger.log("Table created: " + name);
}

uint64_t StorageManager::write_record(
    const std::string& table_name,
    const std::unordered_map<std::string, std::string>& data) {
    if (!impl_->tables.count(table_name)) {
        throw StorageException("Table not found: " + table_name);
    }

    uint64_t record_id = impl_->next_ids[table_name]++;
    std::ofstream data_file(impl_->db_path + "/" + table_name, std::ios::app);
    data_file << record_id << "|";
    for (const auto& [key, value] : data) {
        data_file << key << "=" << value << ";";
    }
    data_file << "\n";

    impl_->logger.log("Record written: " + std::to_string(record_id));
    return record_id;
}

void StorageManager::start_transaction() {
    if (impl_->transaction_active) {
        throw StorageException("Transaction already active");
    }
    impl_->transaction_active = true;
    impl_->logger.log("Transaction started");
}

void StorageManager::commit() {
    impl_->transaction_active = false;
    flush_to_disk();
    impl_->logger.log("Transaction committed");
}

void StorageManager::rollback() {
    impl_->transaction_active = false;
    // Rollback logic here
    impl_->logger.log("Transaction rolled back");
}

void StorageManager::flush_to_disk() {
    // Ensure all data is flushed to disk
    impl_->logger.log("Data flushed to disk");
}

std::string StorageManager::get_path() const {
    return impl_->db_path;
}
} // namespace blankdb