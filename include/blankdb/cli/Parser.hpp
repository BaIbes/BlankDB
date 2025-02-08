#ifndef BLANKDB_PARSER_HPP
#define BLANKDB_PARSER_HPP

#include <string>
#include <unordered_map>
#include <vector>

namespace blankdb {
namespace cli {

struct Command {
    enum class Type { CREATE_TABLE, INSERT, SELECT, DROP_TABLE, CREATE_INDEX, DROP_INDEX, UNKNOWN };

    Type type = Type::UNKNOWN;
    std::string table_name;
    std::unordered_map<std::string, std::string> data; // Для CREATE TABLE и INSERT
    std::vector<std::string> columns;                 // Для SELECT и CREATE INDEX

    // Преобразование команды в строку запроса
    std::string to_query_string() const;
};

class Parser {
public:
    static Command parse(const std::string& input);
};

} // namespace cli
} // namespace blankdb

#endif // BLANKDB_PARSER_HPP