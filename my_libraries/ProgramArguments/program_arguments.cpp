#include "program_arguments.h"

#include <cstring>

namespace program_arguments {

    MissedArgumentException::MissedArgumentException(std::string short_argument, std::string long_argument) {
        this->message_ = "You missed argument for " + short_argument + " | " + long_argument;
    }

    const char* MissedArgumentException::what() const noexcept {
        return message_.c_str();
    }

    Response ParseProgramArguments(int argc, const char* const argv[]) {
        Response response;
        strcpy(response.working_directory_, argv[0]);
        for (int i = 1; i < argc; i++) {
            if (argv[i][0] == '-') {
                if (i + 1 >= argc || argv[i + 1][0] == '-') {
                    if (argv[i][1] == '-') {
                        response.flags_.insert(argv[i]);
                    } else {
                        for (int j = 1; j < strlen(argv[i]); j++) {
                            char tmp[3];
                            tmp[0] = '-';
                            tmp[1] = argv[i][j];
                            tmp[2] = '\0';
                            response.flags_.insert(tmp);
                        }
                    }
                } else {
                    response.SetArgument(argv[i], argv[i + 1]);
                    i++;
                }
            } else {
                response.some_values_.emplace_back(argv[i]);
            }
        }

        return response;
    }

    inline void Response::SetArgument(const std::string& argument, const std::string& value) {
        arguments_[argument] = value;
    }

    const std::string& Response::GetArgument(const std::string& short_argument,
                                             const std::string& long_argument) const {
        try {
            return arguments_.at(short_argument);
        } catch (const std::exception& exception_1) {
            try {
                return arguments_.at(long_argument);
            } catch (const std::exception& exception_2) {
                throw MissedArgumentException(short_argument, long_argument);
            }
        }
    }
}// namespace program_arguments
