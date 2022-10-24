#include "bmp_file.h"

#include <iostream>
#include <fstream>

namespace bmp_writer {
    inline void Pixel::SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
        this->color = alpha;
        this->color = (this->color << 8) + red;
        this->color = (this->color << 8) + green;
        this->color = (this->color << 8) + blue;
    }

    Pixel::Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
        this->SetColor(red, green, blue, alpha);
    }

    void WriteBmpFile(const std::string& filepath, const std::vector<std::vector<Pixel>>& pixels) {
        std::ofstream output(filepath, std::ios_base::out | std::ios_base::binary);

        if (output) {
            BitmapFileHeader file_header{
                BitmapFileHeader::kBmpFileSignature,
                static_cast<uint32_t>(sizeof(BitmapFileHeader) + sizeof(BitmapInformationHeader)
                    + pixels.size() * pixels[0].size() * sizeof(Pixel)),
                0,
                sizeof(BitmapFileHeader) + sizeof(BitmapInformationHeader)
            };

            BitmapInformationHeader information_header{
                sizeof(BitmapInformationHeader),
                static_cast<uint32_t>(pixels[0].size()),
                static_cast<uint32_t>(pixels.size()),
                1,
                sizeof(Pixel) * 8,
                0,
                0,
                40, // 300dpi
                40, //300dpi
                0,
                0
            };

            output.write((char*) &file_header, sizeof(file_header));
            output.write((char*) &information_header, sizeof(information_header));

            for (int64_t i = information_header.height - 1; i >= 0; i--) {
                for (int64_t j = 0; j < information_header.width; j++) {
                    output.write((char*) &pixels[i][j], sizeof(Pixel));
                }
            }

            output.close();
        } else {
            std::cerr << "Directory '" << filepath << "' doesn't exist!" << std::endl;
            throw std::runtime_error("Wrong directory");
        }
    }
}

