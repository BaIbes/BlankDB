// src/cli/Parser.cpp

#include "../../include/blankdb/cli/Parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace blankdb {
namespace cli {

std::unique_ptr<Command> Parser::parse(const std::string& input) {
    auto type = determine_command_type(input);
    switch (type) {
        case Command::Type::CREATE_TABLE:
            return parse_create_table(input);
        case Command::Type::INSERT:
            return parse_insert(input);
        case Command::Type::SELECT:
            return parse_select(input);
        default:
            return std::make_unique<Command>();
    }
}

Command::Type Parser::determine_command_type(const std::string& input) {
    std::istringstream iss(input);
    std::string command;

    iss >> command;
    if (command == "CREATE") return Command::Type::CREATE_TABLE;
    if (command == "INSERT") return Command::Type::INSERT;
    if (command == "SELECT") return Command::Type::SELECT;
    return Command::Type::UNKNOWN;
}

std::unique_ptr<Command> Parser::parse_create_table(const std::string& input) {
    std::istringstream iss(input);
    std::string token;
    Command cmd;
    cmd.type = Command::Type::CREATE_TABLE;

    // Пропускаем "CREATE TABLE"
    iss >> token >> token >> cmd.table_name;

    // Собираем схему таблицы
    std::unordered_map<std::string, std::string> schema;
    while (iss >> token) {
        size_t pos = token.find(':');
        if (pos != std::string::npos) {
            std::string column_name = token.substr(0, pos);
            std::string column_type = token.substr(pos + 1);
            schema[column_name] = column_type;
        }
    }

    cmd.data = schema;
    return std::make_unique<Command>(cmd);
}

std::unique_ptr<Command> Parser::parse_insert(const std::string& input) {
    std::istringstream iss(input);
    std::string token;
    Command cmd;
    cmd.type = Command::Type::INSERT;

    // Пропускаем "INSERT INTO"
    iss >> token >> token >> cmd.table_name;

    // Собираем данные для вставки
    std::unordered_map<std::string, std::string> data;
    while (iss >> token) {
        size_t pos = token.find('=');
        if (pos != std::string::npos) {
            std::string key = token.substr(0, pos);
            std::string value = token.substr(pos + 1);
            data[key] = value;
        }
    }

    cmd.data = data;
    return std::make_unique<Command>(cmd);
}

std::unique_ptr<Command> Parser::parse_select(const std::string& input) {
    std::istringstream iss(input);
    std::string token;
    Command cmd;
    cmd.type = Command::Type::SELECT;

    // Пропускаем "SELECT"
    iss >> token;

    // Собираем список колонок
    std::vector<std::string> columns;
    while (iss >> token && token != "FROM") {
        columns.push_back(token);
    }

    // Пропускаем "FROM"
    iss >> token;

    // Получаем имя таблицы
    iss >> cmd.table_name;

    cmd.columns = columns;
    return std::make_unique<Command>(cmd);
}

} // namespace cli
} // namespace blankdb