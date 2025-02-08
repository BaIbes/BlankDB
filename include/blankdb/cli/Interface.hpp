#ifndef BLANKDB_INTERFACE_HPP
#define BLANKDB_INTERFACE_HPP

#include <string>
#include <vector>

namespace blankdb {
namespace cli {

class Interface {
public:
    Interface() = default;

    void run();
    std::string get_command();

    // Делаем метод доступным для внешнего использования
    void display_prompt();

private:
    void process_input(const std::string& input);
};

} // namespace cli
} // namespace blankdb

#endif // BLANKDB_INTERFACE_HPP