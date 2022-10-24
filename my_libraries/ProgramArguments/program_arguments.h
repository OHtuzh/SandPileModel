#pragma once

#include <vector>
#include <map>
#include <unordered_set>
#include <string>

namespace program_arguments {

    class MissedArgumentException : public std::exception {
     public:
        MissedArgumentException(std::string short_argument, std::string long_argument);
        const char* what() const noexcept override;
     private:
        std::string message_;
    };

    class Response {
     public:
        char working_directory_[256]{};
        std::unordered_set<std::string> flags_;
        std::vector<std::string> some_values_;


        void SetArgument(const std::string& argument, const std::string& value);
        const std::string& GetArgument(const std::string& short_argument, const std::string& long_argument) const;
     private:
        std::map<std::string, std::string> arguments_;
    };

    Response ParseProgramArguments(int argc, const char* const argv[]);

}// namespace program_arguments
