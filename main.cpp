#include "my_libraries/SandPileModel/sand_pile_model.h"
#include "my_libraries/ProgramArguments/program_arguments.h"

#include <iostream>

const char* kLArgument = "-l";
const char* kLengthArgument = "--height";
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

void PrepareArguments(SandPileInputData& input_data,
                      int argc, const char* const argv[]) {
    auto response = program_arguments::ParseProgramArguments(argc, argv);

    input_data.height = std::stoi(response.GetArgument(kLArgument, kLengthArgument));
    input_data.width = std::stoi(response.GetArgument(kWArgument, kWidthArgument));
    input_data.input_file_path = response.GetArgument(kIArgument, kInputArgument);
    input_data.output_directory = response.GetArgument(kOArgument, kOutputArgument);
    input_data.max_iterations = std::stoull(response.GetArgument(kMArgument, kMaxIterationsArgument));
    input_data.frequency = std::stoull(response.GetArgument(kFArgument, kFreqArgument));
}

int main(int argc, const char** argv) {
    SandPileInputData input_data;

    try {
        PrepareArguments(input_data, argc, argv);
    } catch (const program_arguments::MissedArgumentException& exception) {
        std::cerr << exception.what() << std::endl;
        return -1;
    }

    SandPileModelAlgorithm(input_data);

    return 0;
}
