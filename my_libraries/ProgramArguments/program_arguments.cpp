#include "program_arguments.h"

#include <cstring>

namespace program_arguments {

missed_argument_exception::missed_argument_exception(std::string short_argument, std::string long_argument) {
    this->message_ = "You missed argument for " + short_argument + " | " + long_argument;
}

const char* missed_argument_exception::what() const noexcept {
    return message_.c_str();
}

Response parse_program_arguments(const int argc, const char* argv[]) {
    Response response;
    strcpy(response.working_directory, argv[0]);
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (i + 1 >= argc || argv[i + 1][0] == '-') {
                if (argv[i][1] == '-') {
                    response.flags.insert(argv[i]);
                } else {
                    for (int j = 1; j < strlen(argv[i]); j++) {
                        char tmp[3];
                        tmp[0] = '-';
                        tmp[1] = argv[i][j];
                        tmp[2] = '\0';
                        response.flags.insert(tmp);
                    }
                }
            } else {
                response.set_argument(argv[i], argv[i + 1]);
                i++;
            }
        } else {
            response.some_values.push_back(argv[i]);
        }
    }

    return response;
}

void Response::set_argument(std::string argument, std::string value) {
    arguments[argument] = value;
}

std::string Response::get_argument(std::string short_argument, std::string long_argument) {
    try {
        return arguments.at(short_argument);
    } catch (const std::exception& exception_1) {
        try {
            return arguments.at(long_argument);
        } catch (const std::exception& exception_2) {
            throw missed_argument_exception(short_argument, long_argument);
        }
    }
}

}// namespace program_arguments
