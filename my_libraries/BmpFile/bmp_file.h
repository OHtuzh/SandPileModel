#pragma once
#pragma pack(push, 1)

#include <cstdint>
#include <vector>
#include <string>

namespace bmp_writer {

    struct BitmapFileHeader {
        static const uint16_t kBmpFileSignature = 0x4D42;

        uint16_t file_signature;    // must be 0x4d42
        uint32_t file_size;
        uint32_t reserved;
        uint32_t data_offset;
    };

    struct BitmapInformationHeader {
        static const uint32_t kBmpSizeOfHeader = 40;
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
    };

    struct Pixel {
     private:
        uint32_t color{};
     public:
        void SetColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);

        Pixel() = default;
        Pixel(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
    };

    void WriteBmpFile(const std::string& filepath, const std::vector<std::vector<Pixel>>& pixels);

    static_assert(sizeof(BitmapFileHeader) == 14, "Bitmap file header must consist 14 bytes");
    static_assert(sizeof(BitmapInformationHeader) == 40, "Bitmap information header must consist 40 bytes");
    static_assert(sizeof(Pixel) == 4, "Pixel must consist 4 bytes");
}

#pragma pack(pop)
