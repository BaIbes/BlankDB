#include "../include/blankdb/Database.hpp"
#include "../include/blankdb/cli/Interface.hpp"
#include "../include/blankdb/cli/Parser.hpp"
#include "../include/blankdb/utils/Logger.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>

int main() {
    try {
        // Инициализация базы данных
        blankdb::Database db("test_db");
        db.open();

        // Инициализация CLI
        blankdb::cli::Interface interface;

        // Логирование начала работы приложения
        blankdb::Logger::get_instance().info("BlankDB application started.");

        // Главный цикл CLI
        while (true) {
            interface.display_prompt(); // Вывод приглашения
            std::string command = interface.get_command(); // Получение команды

            if (command == "exit" || command == "quit") {
                blankdb::Logger::get_instance().info("Exiting application.");
                break;
            }

            try {
                // Парсинг команды
                auto parsed_command = blankdb::cli::Parser::parse(command);

                // Выполнение команды
                if (parsed_command.type == blankdb::cli::Command::Type::CREATE_TABLE) {
                    db.create_table(parsed_command.table_name, parsed_command.data);
                    blankdb::Logger::get_instance().info("Table created: " + parsed_command.table_name);
                } else if (parsed_command.type == blankdb::cli::Command::Type::INSERT) {
                    db.insert(parsed_command.table_name, parsed_command.data);
                    blankdb::Logger::get_instance().info("Record inserted into table: " + parsed_command.table_name);
                } else if (parsed_command.type == blankdb::cli::Command::Type::SELECT) {
                    std::string query = parsed_command.to_query_string(); // Преобразуем команду в строку запроса
                    auto result = db.execute_query(query);
                    for (const auto& record : result) {
                        for (const auto& [key, value] : record) {
                            std::cout << key << ": " << value << ", ";
                        }
                        std::cout << std::endl;
                    }
                } else if (parsed_command.type == blankdb::cli::Command::Type::DROP_TABLE) {
                    db.drop_table(parsed_command.table_name);
                    blankdb::Logger::get_instance().info("Table dropped: " + parsed_command.table_name);
                } else if (parsed_command.type == blankdb::cli::Command::Type::CREATE_INDEX) {
                    db.create_index(parsed_command.table_name, parsed_command.columns[0]);
                    blankdb::Logger::get_instance().info("Index created on column: " + parsed_command.columns[0] + " in table: " + parsed_command.table_name);
                } else if (parsed_command.type == blankdb::cli::Command::Type::DROP_INDEX) {
                    db.drop_index(parsed_command.table_name, parsed_command.columns[0]);
                    blankdb::Logger::get_instance().info("Index dropped on column: " + parsed_command.columns[0] + " in table: " + parsed_command.table_name);
                } else {
                    std::cerr << "Unknown command type." << std::endl;
                }
            } catch (const std::exception& e) {
                blankdb::Logger::get_instance().error("Error processing command: " + std::string(e.what()));
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        // Закрытие базы данных
        db.close();
        blankdb::Logger::get_instance().info("Database closed.");

    } catch (const std::exception& e) {
        blankdb::Logger::get_instance().error("Fatal error: " + std::string(e.what()));
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}