#include "my_libraries/ProgramArguments/program_arguments.h"
#include "my_libraries/BmpFile/bmp_file.h"

#include <iostream>
#include <vector>
#include <fstream>

const std::string L_ARGUMENT = std::string("-l");
const std::string LENGTH_ARGUMENT = std::string("--length");
const std::string W_ARGUMENT = std::string("-w");
const std::string WIDTH_ARGUMENT = std::string("--width");
const std::string I_ARGUMENT = std::string("-i");
const std::string INPUT_ARGUMENT = std::string("--input");
const std::string O_ARGUMENT = std::string("-o");
const std::string OUTPUT_ARGUMENT = std::string("--output");
const std::string M_ARGUMENT = std::string("-m");
const std::string MAX_ITERATIONS_ARGUMENT = std::string("--max-iter");
const std::string F_ARGUMENT = std::string("-f");
const std::string FREQ_ARGUMENT = std::string("--freq");

const bmp_writer::Pixel WHITE(255, 255, 255, 255);
const bmp_writer::Pixel GREEN(0, 128, 0, 255);
const bmp_writer::Pixel PURPLE(140, 0, 255, 255);
const bmp_writer::Pixel YELLOW(255, 255, 0, 255);
const bmp_writer::Pixel BLACK(0, 0, 0, 255);

void prepare_arguments(uint16_t& length,
                       uint16_t& width,
                       std::string& input_file_path,
                       std::string& output_directory,
                       uint64_t& max_iterations,
                       uint64_t& frequency,
                       const int argc, const char* argv[]) {
    program_arguments::Response response = program_arguments::parse_program_arguments(argc, argv);

    try {
        length = std::stoi(response.get_argument(L_ARGUMENT, LENGTH_ARGUMENT));
        width = std::stoi(response.get_argument(W_ARGUMENT, WIDTH_ARGUMENT));
        input_file_path = response.get_argument(I_ARGUMENT, INPUT_ARGUMENT);
        output_directory = response.get_argument(O_ARGUMENT, OUTPUT_ARGUMENT);
        max_iterations = std::stoull(response.get_argument(M_ARGUMENT, MAX_ITERATIONS_ARGUMENT));
        frequency = std::stoull(response.get_argument(F_ARGUMENT, FREQ_ARGUMENT));
    } catch (program_arguments::missed_argument_exception& exception) {
        std::cerr << exception.what() << std::endl;
        exit(-1);
    }
}

void get_data_from_file(const std::string& file_path, std::vector<std::vector<uint64_t>>& grains) {
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

void draw_sand_pile_model(const std::vector<std::vector<uint64_t>>& data, const std::string& output_file_path) {
    std::vector<std::vector<bmp_writer::Pixel>> pixels(data.size(), std::vector<bmp_writer::Pixel>(data[0].size()));
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == 0) {
                pixels[i][j] = WHITE;
            } else if (data[i][j] == 1) {
                pixels[i][j] = GREEN;
            } else if (data[i][j] == 2) {
                pixels[i][j] = PURPLE;
            } else if (data[i][j] == 3) {
                pixels[i][j] = YELLOW;
            } else {
                pixels[i][j] = BLACK;
            }
        }
    }

    bmp_writer::WriteBmpFile(output_file_path.c_str(), pixels);
}

bool sand_pile_model_iteration(std::vector<std::vector<uint64_t>>& data) { // returns true if sand pile changed
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

void sand_pile_model_algorithm(const uint16_t height,
                               const uint16_t width,
                               const uint64_t max_iterations,
                               const uint64_t frequency,
                               const std::string& input_file_path,
                               const std::string& out_directory) {
    std::vector<std::vector<uint64_t>> grains(height, std::vector<uint64_t>(width, 0));
    get_data_from_file(input_file_path, grains);

    bool work = true;
    uint64_t i = 1;
    while (work && (i++ <= max_iterations || max_iterations == 0)) {
        work = sand_pile_model_iteration(grains);
        if (frequency != 0 && i % frequency == 0) {
            draw_sand_pile_model(grains, out_directory + '\\' + std::to_string(i / frequency) + ".bmp");
        }
    }
    if (frequency == 0) {
        draw_sand_pile_model(grains, out_directory + "\\1.bmp");
    }
}

int main(int argc, const char** argv) {
    uint16_t height;
    uint16_t width;
    std::string input_file_path;
    std::string output_directory;
    uint64_t max_iterations;
    uint64_t frequency;
    prepare_arguments(height, width, input_file_path, output_directory, max_iterations, frequency, argc, argv);

    sand_pile_model_algorithm(height, width, max_iterations, frequency, input_file_path, output_directory);


    return 0;
}
