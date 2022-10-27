#include "bmp_file.h"

#include <iostream>
#include <fstream>

namespace bmp_writer {
    inline uint8_t* BitmapFileHeader::Serialize() const {
        return new uint8_t[BitmapFileHeader::kBitmapFileHeaderSize]{
            static_cast<uint8_t>(this->file_signature & 0x00FF),
            static_cast<uint8_t>(this->file_signature >> 8),
            // File kSize | 32 bits
            static_cast<uint8_t>(this->file_size & 0xFF),
            static_cast<uint8_t>(this->file_size >> 8 & 0xFF),
            static_cast<uint8_t>(this->file_size >> 16 & 0xFF),
            static_cast<uint8_t>(this->file_size >> 24),
            // Reserved | 32 bits
            static_cast<uint8_t>(this->reserved & 0xFF),
            static_cast<uint8_t>(this->reserved >> 8 & 0xFF),
            static_cast<uint8_t>(this->reserved >> 16 & 0xFF),
            static_cast<uint8_t>(this->reserved >> 24),
            // Data offset | 32 bits
            static_cast<uint8_t>(this->data_offset & 0xFF),
            static_cast<uint8_t>(this->data_offset >> 8 & 0xFF),
            static_cast<uint8_t>(this->data_offset >> 16 & 0xFF),
            static_cast<uint8_t>(this->data_offset >> 24)
        };
    }

    uint8_t* BitmapInformationHeader::Serialize() const {
        return new uint8_t[BitmapInformationHeader::kBitmapInformationHeaderSize]{
            // Size of header | 32 bits
            static_cast<uint8_t>(this->size_of_header & 0xFF),
            static_cast<uint8_t>(this->size_of_header >> 8 & 0xFF),
            static_cast<uint8_t>(this->size_of_header >> 16 & 0xFF),
            static_cast<uint8_t>(this->size_of_header >> 24),
            // Width | 32 bits
            static_cast<uint8_t>(this->width & 0xFF),
            static_cast<uint8_t>(this->width >> 8 & 0xFF),
            static_cast<uint8_t>(this->width >> 16 & 0xFF),
            static_cast<uint8_t>(this->width >> 24),
            // Size of header | 32 bits
            static_cast<uint8_t>(this->height & 0xFF),
            static_cast<uint8_t>(this->height >> 8 & 0xFF),
            static_cast<uint8_t>(this->height >> 16 & 0xFF),
            static_cast<uint8_t>(this->height >> 24),
            // Planes | 16 bits
            static_cast<uint8_t>(this->planes & 0xFF),
            static_cast<uint8_t>(this->planes >> 8),
            // Bits per pixel | 16 bit
            static_cast<uint8_t>(this->bits_per_pixel & 0xFF),
            static_cast<uint8_t>(this->bits_per_pixel >> 8),
            // Compression | 32 bit
            static_cast<uint8_t>(this->compression & 0xFF),
            static_cast<uint8_t>(this->compression >> 8 & 0xFF),
            static_cast<uint8_t>(this->compression >> 16 & 0xFF),
            static_cast<uint8_t>(this->compression >> 24),
            // Image kSize | 32 bit
            static_cast<uint8_t>(this->image_size & 0xFF),
            static_cast<uint8_t>(this->image_size >> 8 & 0xFF),
            static_cast<uint8_t>(this->image_size >> 16 & 0xFF),
            static_cast<uint8_t>(this->image_size >> 24),
            // Pixels per meter in x axis | 32 bit
            static_cast<uint8_t>(this->pixels_per_meter_in_X_axis & 0xFF),
            static_cast<uint8_t>(this->pixels_per_meter_in_X_axis >> 8 & 0xFF),
            static_cast<uint8_t>(this->pixels_per_meter_in_X_axis >> 16 & 0xFF),
            static_cast<uint8_t>(this->pixels_per_meter_in_X_axis >> 24),
            // Pixels per meter in y axis | 32 bit
            static_cast<uint8_t>(this->pixels_per_meter_in_Y_axis & 0xFF),
            static_cast<uint8_t>(this->pixels_per_meter_in_Y_axis >> 8 & 0xFF),
            static_cast<uint8_t>(this->pixels_per_meter_in_Y_axis >> 16 & 0xFF),
            static_cast<uint8_t>(this->pixels_per_meter_in_Y_axis >> 24),
            // Color used | 32 bit
            static_cast<uint8_t>(this->color_used & 0xFF),
            static_cast<uint8_t>(this->color_used >> 8 & 0xFF),
            static_cast<uint8_t>(this->color_used >> 16 & 0xFF),
            static_cast<uint8_t>(this->color_used >> 24),
            // Important colors | 32 bit
            static_cast<uint8_t>(this->important_colors & 0xFF),
            static_cast<uint8_t>(this->important_colors >> 8 & 0xFF),
            static_cast<uint8_t>(this->important_colors >> 16 & 0xFF),
            static_cast<uint8_t>(this->important_colors >> 24)
        };
    }

    inline void Pixel32_t::SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
        this->color = alpha;
        this->color = (this->color << 8) + red;
        this->color = (this->color << 8) + green;
        this->color = (this->color << 8) + blue;
    }

    Pixel32_t::Pixel32_t(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha) {
        this->SetColor(red, green, blue, alpha);
    }

    ColorTable::ColorTable() {
        this->colors = {
            {255, 255, 255, 0},
            {0, 128, 0, 0},
            {140, 0, 255, 0},
            {255, 255, 0, 0},
            {0, 0, 0, 0}
        };
    }

    inline uint8_t* ColorTable::Serialize() const {
        auto* result = new uint8_t[this->colors.size() * Pixel32_t::kSize];
        int32_t i = 0;
        for (auto color : colors) {
            result[i++] = color.color & 0xFF; // 0xaarrggbb
            result[i++] = color.color >> 8 & 0xFF;
            result[i++] = color.color >> 16 & 0xFF;
            result[i++] = color.color >> 24;
        }

        return result;
    }

    inline int64_t ColorTable::GetSize() const {
        return static_cast<int64_t>(this->colors.size() * 4);
    }

    uint8_t* SerializePixels4t(const std::vector<Pixel4_t>& pixels, const std::streamsize& buff_size) {
        auto* result = new uint8_t[buff_size]{};
        for (int i = 0, j = 0; i < pixels.size() - 1; i += 2, j++) {
            result[j] = static_cast<uint8_t>((pixels[i].color << 4) + pixels[i + 1].color);
        }
        if (pixels.size() % 2) {
            result[pixels.size() / 2] = static_cast<uint8_t>(pixels[pixels.size() - 1].color << 4);
        }

        return result;
    }

    void WriteBmpFile(const std::string& filepath, const std::vector<std::vector<Pixel4_t>>& pixels) {
        std::ofstream output(filepath, std::ios_base::out | std::ios_base::binary);

        if (output) {
            ColorTable color_table;

            BitmapFileHeader file_header{
                BitmapFileHeader::kBmpFileSignature,
                static_cast<uint32_t>(BitmapFileHeader::kBitmapFileHeaderSize
                    + BitmapInformationHeader::kBitmapInformationHeaderSize
                    + color_table.GetSize()
                    + (pixels[0].size() + 1) / 2 * pixels.size()),
                0,
                static_cast<uint32_t>(BitmapFileHeader::kBitmapFileHeaderSize
                    + BitmapInformationHeader::kBitmapInformationHeaderSize + color_table.GetSize())
            };

            BitmapInformationHeader information_header{
                BitmapInformationHeader::kBitmapInformationHeaderSize,
                static_cast<uint32_t>(pixels[0].size()),
                static_cast<uint32_t>(pixels.size()),
                1,
                4,
                0,
                0,
                40, // 300dpi
                40, //300dpi
                5,
                5
            };

            output.write((char*) file_header.Serialize(), BitmapFileHeader::kBitmapFileHeaderSize);
            output.write((char*) information_header.Serialize(), BitmapInformationHeader::kBitmapInformationHeaderSize);
            output.write((char*) color_table.Serialize(), color_table.GetSize());

            const auto buff_size = static_cast<std::streamsize>(((pixels[0].size() + 1) / 2 + 3) / 4 * 4);
            for (int64_t i = information_header.height - 1; i >= 0; i--) {
                output.write((char*) SerializePixels4t(pixels[i], buff_size), buff_size);
            }

            output.close();
        } else {
            std::cerr << "Directory '" << filepath << "' doesn't exist!" << std::endl;
            throw std::runtime_error("Wrong directory");
        }
    }
}

