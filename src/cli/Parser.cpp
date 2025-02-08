#include "../../include/blankdb/cli/Parser.hpp"
#include <sstream>

namespace blankdb {
namespace cli {

std::string Command::to_query_string() const {
    switch (type) {
        case Type::CREATE_TABLE: {
            std::ostringstream oss;
            oss << "CREATE TABLE " << table_name << " (";
            for (const auto& [key, value] : data) {
                oss << key << ":" << value << ", ";
            }
            oss.seekp(-2, std::ios_base::end); // Убираем запятую в конце
            oss << ")";
            return oss.str();
        }
        case Type::INSERT: {
            std::ostringstream oss;
            oss << "INSERT INTO " << table_name << " (";
            for (const auto& [key, value] : data) {
                oss << key << "=" << value << ", ";
            }
            oss.seekp(-2, std::ios_base::end); // Убираем запятую в конце
            oss << ")";
            return oss.str();
        }
        case Type::SELECT: {
            std::ostringstream oss;
            oss << "SELECT ";
            if (!columns.empty()) {
                for (const auto& col : columns) {
                    oss << col << ", ";
                }
                oss.seekp(-2, std::ios_base::end); // Убираем запятую в конце
            } else {
                oss << "*";
            }
            oss << " FROM " << table_name;
            return oss.str();
        }
        default:
            return "";
    }
}

Command Parser::parse(const std::string& input) {
    Command cmd;
    std::istringstream iss(input);
    std::string token;

    iss >> token;
    if (token == "CREATE") {
        iss >> token;
        if (token == "TABLE") {
            cmd.type = Command::Type::CREATE_TABLE;
            iss >> cmd.table_name;
            while (iss >> token) {
                size_t pos = token.find(':');
                if (pos != std::string::npos) {
                    std::string column_name = token.substr(0, pos);
                    std::string column_type = token.substr(pos + 1);
                    cmd.data[column_name] = column_type;
                }
            }
        }
    } else if (token == "INSERT") {
        cmd.type = Command::Type::INSERT;
        iss.ignore(); // Пропускаем "INTO"
        iss >> cmd.table_name;
        while (iss >> token) {
            size_t pos = token.find('=');
            if (pos != std::string::npos) {
                std::string key = token.substr(0, pos);
                std::string value = token.substr(pos + 1);
                cmd.data[key] = value;
            }
        }
    } else if (token == "SELECT") {
        cmd.type = Command::Type::SELECT;
        while (iss >> token && token != "FROM") {
            cmd.columns.push_back(token);
        }
        if (token == "FROM") {
            iss >> cmd.table_name;
        }
    }

    return cmd;
}

} // namespace cli
} // namespace blankdb