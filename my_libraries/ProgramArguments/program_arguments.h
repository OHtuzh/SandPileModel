#pragma once

#include <vector>
#include <map>
#include <unordered_set>
#include <string>

namespace program_arguments {

class missed_argument_exception : public std::exception {
 public:
    missed_argument_exception(std::string short_argument, std::string long_argument);
    const char* what() const noexcept override;
 private:
    std::string message_;
};

struct Response {
 private:
    std::map<std::string, std::string> arguments;
 public:
    char working_directory[256]{};
    std::unordered_set<std::string> flags;
    std::vector<std::string> some_values;

    void set_argument(std::string argument, std::string value);

    std::string get_argument(std::string short_argument, std::string long_argument);
};

Response parse_program_arguments(const int argc, const char* argv[]);

}// namespace program_arguments