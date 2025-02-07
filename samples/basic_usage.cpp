#include <blankdb/Database.hpp>
#include <iostream>

int main() {
    blankdb::Database db("test_db");
    db.open();

    try {
        db.create_table("users", {{"id", "int"}, {"name", "string"}});
        db.insert("users", {{"id", "1"}, {"name", "Alice"}});
        db.insert("users", {{"id", "2"}, {"name", "Bob"}});

        auto result = db.execute_query("SELECT * FROM users");
        for (const auto& record : result) {
            for (const auto& [key, value] : record) {
                std::cout << key << ": " << value << ", ";
            }
            std::cout << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    db.close();
    return 0;
}