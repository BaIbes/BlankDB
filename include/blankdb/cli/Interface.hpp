// include/blankdb/cli/Interface.hpp

#ifndef BLANKDB_INTERFACE_HPP
#define BLANKDB_INTERFACE_HPP

#include <string>
#include <vector>
#include <memory>

namespace blankdb {
namespace cli {

class Interface {
public:
    Interface() = default;

    // Запуск интерфейса
    void run();

    // Получение команды от пользователя
    std::string get_command();

private:
    void display_prompt();
};

} // namespace cli
} // namespace blankdb

#endif // BLANKDB_INTERFACE_HPP