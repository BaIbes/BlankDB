// include/blankdb/cli/Parser.hpp

#ifndef BLANKDB_PARSER_HPP
#define BLANKDB_PARSER_HPP

#include <string>
#include <unordered_map>
#include <variant>
#include <memory>
#include <vector>

namespace blankdb {
namespace cli {

struct Command {
    enum class Type { CREATE_TABLE, INSERT, SELECT, DROP_TABLE, UNKNOWN };

    Type type;
    std::string table_name;
    std::unordered_map<std::string, std::string> data; // Для INSERT
    std::vector<std::string> columns;                 // Для SELECT
};

class Parser {
public:
    static std::unique_ptr<Command> parse(const std::string& input);

private:
    static Command::Type determine_command_type(const std::string& input);
    static std::unique_ptr<Command> parse_create_table(const std::string& input);
    static std::unique_ptr<Command> parse_insert(const std::string& input);
    static std::unique_ptr<Command> parse_select(const std::string& input);
};

} // namespace cli
} // namespace blankdb

#endif // BLANKDB_PARSER_HPP