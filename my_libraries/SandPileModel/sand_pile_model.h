#pragma once

#include <cinttypes>

#include <vector>
#include <string>

struct SandPileInputData {
    uint16_t height;
    uint16_t width;
    std::string input_file_path;
    std::string output_directory;
    uint64_t max_iterations;
    uint64_t frequency;
};


void GetDataFromFile(const std::string& file_path, std::vector<std::vector<uint64_t>>& grains);

void DrawSandPileModel(const std::vector<std::vector<uint64_t>>& data, const std::string& output_file_path);

bool SandPileModelIteration(std::vector<std::vector<uint64_t>>& data);

void SandPileModelAlgorithm(const SandPileInputData& input_data);

