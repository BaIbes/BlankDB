#include "../../include/blankdb/cli/Interface.hpp"
#include <iostream>

namespace blankdb {
namespace cli {

void Interface::run() {
    while (true) {
        display_prompt();
        std::string command = get_command();
        if (command == "exit" || command == "quit") {
            std::cout << "Exiting..." << std::endl;
            break;
        }
        process_input(command);
    }
}

std::string Interface::get_command() {
    std::string command;
    std::getline(std::cin, command);
    return command;
}

// Метод теперь доступен публично
void Interface::display_prompt() {
    std::cout << "> ";
}

void Interface::process_input(const std::string& input) {
    // Здесь можно добавить обработку команд
    std::cout << "Command received: " << input << std::endl;
}

} // namespace cli
} // namespace blankdb