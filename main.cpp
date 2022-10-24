#include "my_libraries/ProgramArguments/program_arguments.h"
#include "my_libraries/BmpFile/bmp_file.h"

#include <iostream>
#include <vector>
#include <fstream>

const std::string kLArgument("-l");
const std::string kLengthArgument("--length");
const std::string kWArgument("-w");
const std::string kWidthArgument("--width");
const std::string kIArgument("-i");
const std::string kInputArgument("--input");
const std::string kOArgument("-o");
const std::string kOutputArgument("--output");
const std::string kMArgument("-m");
const std::string kMaxIterationsArgument("--max-iter");
const std::string kFArgument("-f");
const std::string kFreqArgument("--freq");

const bmp_writer::Pixel kWhite(255, 255, 255, 255);
const bmp_writer::Pixel kGreen(0, 128, 0, 255);
const bmp_writer::Pixel kPurple(140, 0, 255, 255);
const bmp_writer::Pixel kYellow(255, 255, 0, 255);
const bmp_writer::Pixel kBlack(0, 0, 0, 255);

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

void GetDataFromFile(const std::string& file_path, std::vector<std::vector<uint64_t>>& grains) {
    std::ifstream input(file_path);
    if (!input) {
        std::cerr << "Invalid file path" << std::endl;
        exit(-1);
    }

    int16_t x;
    int16_t y;
    int64_t number_of_grains;

    while ((input >> x)) {
        if (!(input >> y)) {
            std::cerr << "Missed y value" << std::endl;
            exit(-1);
        }
        if (!(input >> number_of_grains)) {
            std::cerr << "Missed number of grains value" << std::endl;
            exit(-1);
        }

        grains[y][x] = number_of_grains;
    }

    input.close();
}

void DrawSandPileModel(const std::vector<std::vector<uint64_t>>& data, const std::string& output_file_path) {
    std::vector<std::vector<bmp_writer::Pixel>> pixels(data.size(), std::vector<bmp_writer::Pixel>(data[0].size()));
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == 0) {
                pixels[i][j] = kWhite;
            } else if (data[i][j] == 1) {
                pixels[i][j] = kGreen;
            } else if (data[i][j] == 2) {
                pixels[i][j] = kPurple;
            } else if (data[i][j] == 3) {
                pixels[i][j] = kYellow;
            } else {
                pixels[i][j] = kBlack;
            }
        }
    }

    bmp_writer::WriteBmpFile(output_file_path, pixels);
}

bool SandPileModelIteration(std::vector<std::vector<uint64_t>>& data) { // returns true if sand pile changed
    bool changed = false;
    bool resized_left = false;
    bool resized_right = false;
    bool resized_top = false;
    bool resized_bottom = false;

    std::vector<std::vector<uint64_t>> result(data.size() + 2, std::vector<uint64_t>(data[0].size() + 2, 0));
    for (int i = 1; i <= data.size(); i++) {
        for (int j = 1; j <= data[i - 1].size(); j++) {
            if (data[i - 1][j - 1] < 4) {
                result[i][j] += data[i - 1][j - 1];
            } else {
                resized_left = (resized_left || j == 1);
                resized_top = (resized_top || i == 1);
                resized_right = (resized_right || j == data[i - 1].size());
                resized_bottom = (resized_bottom || i == data.size());

                result[i][j] += data[i - 1][j - 1] - 4;
                result[i - 1][j] += 1;
                result[i + 1][j] += 1;
                result[i][j - 1] += 1;
                result[i][j + 1] += 1;
                changed = true;
            }
        }
    }
    data = std::vector<std::vector<uint64_t>>(result.size() - !resized_bottom - !resized_top,
                                              std::vector<uint64_t>(result[0].size() - !resized_left - !resized_right,
                                                                    0));
    for (int i = !resized_top; i < result.size() - !resized_bottom; i++) {
        for (int j = !resized_left; j < result[i].size() - !resized_right; j++) {
            data[i - !resized_top][j - !resized_left] = result[i][j];
        }
    }

    result.clear();
    return changed;
}

void SandPileModelAlgorithm(const uint16_t height,
                            const uint16_t width,
                            const uint64_t max_iterations,
                            const uint64_t frequency,
                            const std::string& input_file_path,
                            const std::string& out_directory) {
    std::vector<std::vector<uint64_t>> grains(height, std::vector<uint64_t>(width, 0));
    GetDataFromFile(input_file_path, grains);

    bool work = true;
    uint64_t i = 1;
    while (work && (i++ <= max_iterations || max_iterations == 0)) {
        work = SandPileModelIteration(grains);
        if (frequency != 0 && i % frequency == 0) {
            DrawSandPileModel(grains, out_directory + '\\' + std::to_string(i / frequency) + ".bmp");
        }
    }
    if (frequency == 0) {
        DrawSandPileModel(grains, out_directory + "\\1.bmp");
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
