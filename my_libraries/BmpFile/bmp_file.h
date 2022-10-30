#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace bmp_writer {

    struct BitmapFileHeader {
        static const uint32_t kBitmapFileHeaderSize = 14;
        static const uint16_t kBmpFileSignature = 0x4D42;

        uint16_t file_signature;    // must be 0x4d42
        uint32_t file_size;
        uint32_t reserved;
        uint32_t data_offset;

        inline uint8_t* Serialize() const;
    };

    struct BitmapInformationHeader {
        static const uint32_t kBitmapInformationHeaderSize = 40;
        static const uint16_t kBmpPlanes = 1;

        uint32_t size_of_header;    // must be 40
        uint32_t width;
        uint32_t height;
        uint16_t planes;            // must be 1
        uint16_t bits_per_pixel;
        uint32_t compression;
        uint32_t image_size;
        uint32_t pixels_per_meter_in_X_axis;
        uint32_t pixels_per_meter_in_Y_axis;
        uint32_t color_used;
        uint32_t important_colors;

        inline uint8_t* Serialize() const;
    };

    struct Pixel32_t {
        uint32_t color{};
        static const uint32_t kSize = 4;

        Pixel32_t() = default;
        Pixel32_t(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

        void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    };

    struct ColorTable {
     private:
        std::vector<Pixel32_t> colors;
     public:
        ColorTable();

        inline int64_t GetSize() const;
        inline uint8_t* Serialize() const;
    };

    struct Pixel4_t {
        static const uint8_t kWhite = 0;
        static const uint8_t kGreen = 1;
        static const uint8_t kPurple = 2;
        static const uint8_t kYellow = 3;
        static const uint8_t kBlack = 4;

        uint8_t color;
    };

    inline uint8_t* SerializePixels4t(const std::vector<Pixel4_t>& pixels, const std::streamsize& buff_size);

    inline BitmapFileHeader CreateFileHeader(const std::vector<std::vector<Pixel4_t>>& pixels,
                                             uint32_t color_table_size);

    inline BitmapInformationHeader CreateInformationHeader(const std::vector<std::vector<Pixel4_t>>& pixels);

    inline void WriteHeaders(std::ostream& out,
                             const BitmapFileHeader& file_header,
                             const BitmapInformationHeader& bitmap_information_header,
                             const ColorTable& color_table);

    void WriteBmpFile(const std::string& filepath, const std::vector<std::vector<Pixel4_t>>& pixels);
}
