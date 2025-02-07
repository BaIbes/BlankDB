// src/cli/Interface.cpp

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
        // Здесь можно добавить обработку команд
        std::cout << "Command received: " << command << std::endl;
    }
}

std::string Interface::get_command() {
    std::string command;
    std::getline(std::cin, command);
    return command;
}

void Interface::display_prompt() {
    std::cout << "> ";
}

} // namespace cli
} // namespace blankdb