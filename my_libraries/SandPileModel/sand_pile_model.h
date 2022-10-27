#pragma once

#include <cinttypes>

#include <vector>
#include <string>

void GetDataFromFile(const std::string& file_path, std::vector<std::vector<uint64_t>>& grains);

void DrawSandPileModel(const std::vector<std::vector<uint64_t>>& data, const std::string& output_file_path);

bool SandPileModelIteration(std::vector<std::vector<uint64_t>>& data);

void SandPileModelAlgorithm(const uint16_t height,
                            const uint16_t width,
                            const uint64_t max_iterations,
                            const uint64_t frequency,
                            const std::string& input_file_path,
                            const std::string& out_directory);

