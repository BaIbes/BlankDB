// Error.hpp
#ifndef BLANKDB_ERROR_HPP
#define BLANKDB_ERROR_HPP

#include <exception>
#include <string>

namespace blankdb {

// Исключение для ошибок хранилища
class StorageException : public std::exception {
public:
    explicit StorageException(const std::string& message) : msg_(message) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

// Исключение для ошибок индексации
class IndexException : public std::exception {
public:
    explicit IndexException(const std::string& message) : msg_(message) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
private:
    std::string msg_;
};

} // namespace blankdb

#endif // BLANKDB_ERROR_HPP