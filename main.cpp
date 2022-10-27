#include "my_libraries/SandPileModel/sand_pile_model.h"
#include "my_libraries/ProgramArguments/program_arguments.h"

#include <iostream>
#include <vector>
#include <fstream>

const char* kLArgument = "-l";
const char* kLengthArgument = "--length";
const char* kWArgument = "-w";
const char* kWidthArgument = "--width";
const char* kIArgument = "-i";
const char* kInputArgument = "--input";
const char* kOArgument = "-o";
const char* kOutputArgument = "--output";
const char* kMArgument = "-m";
const char* kMaxIterationsArgument = "--max-iter";
const char* kFArgument = "-f";
const char* kFreqArgument = "--freq";

void PrepareArguments(uint16_t& length,
                      uint16_t& width,
                      std::string& input_file_path,
                      std::string& output_directory,
                      uint64_t& max_iterations,
                      uint64_t& frequency,
                      int argc, const char* const argv[]) {
    program_arguments::Response response = program_arguments::ParseProgramArguments(argc, argv);

    try {
        length = std::stoi(response.GetArgument(kLArgument, kLengthArgument));
        width = std::stoi(response.GetArgument(kWArgument, kWidthArgument));
        input_file_path = response.GetArgument(kIArgument, kInputArgument);
        output_directory = response.GetArgument(kOArgument, kOutputArgument);
        max_iterations = std::stoull(response.GetArgument(kMArgument, kMaxIterationsArgument));
        frequency = std::stoull(response.GetArgument(kFArgument, kFreqArgument));
    } catch (const program_arguments::MissedArgumentException& exception) {
        std::cerr << exception.what() << std::endl;
        exit(-1);
    }
}

int main(int argc, const char** argv) {
    uint16_t height;
    uint16_t width;
    std::string input_file_path;
    std::string output_directory;
    uint64_t max_iterations;
    uint64_t frequency;
    PrepareArguments(height, width, input_file_path, output_directory, max_iterations, frequency, argc, argv);

    SandPileModelAlgorithm(height, width, max_iterations, frequency, input_file_path, output_directory);

    return 0;
}
