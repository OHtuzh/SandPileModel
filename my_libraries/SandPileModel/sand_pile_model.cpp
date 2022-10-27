#include "sand_pile_model.h"
#include "../BmpFile/bmp_file.h"

#include <iostream>
#include <fstream>


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
    std::vector<std::vector<bmp_writer::Pixel4_t>>
        pixels(data.size(), std::vector<bmp_writer::Pixel4_t>(data[0].size()));
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            if (data[i][j] == 0) {
                pixels[i][j].color = bmp_writer::Pixel4_t::kWhite;
            } else if (data[i][j] == 1) {
                pixels[i][j].color = bmp_writer::Pixel4_t::kGreen;
            } else if (data[i][j] == 2) {
                pixels[i][j].color = bmp_writer::Pixel4_t::kPurple;
            } else if (data[i][j] == 3) {
                pixels[i][j].color = bmp_writer::Pixel4_t::kYellow;
            } else {
                pixels[i][j].color = bmp_writer::Pixel4_t::kBlack;
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
