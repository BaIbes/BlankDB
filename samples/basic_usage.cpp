#include "../include/blankdb/Database.hpp"
#include "../include/blankdb/cli/Interface.hpp"
#include "../include/blankdb/cli/Parser.hpp"
#include "../include/blankdb/utils/Logger.hpp"

#include <iostream>
#include <memory>
#include <stdexcept>

using namespace blankdb;

int main() {
    std::cout << "Hello, BlankDB!" << std::endl;
    Database db("test.db");
    return 0;
}