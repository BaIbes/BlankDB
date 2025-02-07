// In Error.hpp
#include <exception>
#include <string>

namespace blankdb {
class StorageException : public std::exception {
public:
    explicit StorageException(const std::string& message) : msg_(message) {}
    const char* what() const noexcept override {
        return msg_.c_str();
    }
private:
    std::string msg_;
};
class IndexException : public std::exception {
public:
    explicit IndexException(const std::string& message) : msg_(message) {}
    const char* what() const noexcept override { return msg_.c_str(); }
private:
    std::string msg_;
};
} // namespace blankdb