/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Texture.h"
#include <iostream>
#include <png.h>

namespace rc::graphics::image {


void Texture::loadFromFile(const std::string& textureName) {
    DataFile file("textures/" + textureName);
    if (!file.exists()) return;
    readPNG(file);
}

void Texture::saveToFile(const std::string& textureName) {
    DataFile file("textures/" + textureName);
    savePNG(file);
}

void Texture::readPNG(const DataFile& file) {
    png_FILE_p pngFile = fopen(file.getFullPath().string().c_str(), "rb");
    png_byte header[8];
    fread(header, 1, 8, pngFile);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr  = png_create_info_struct(png_ptr);
    // set error handling to not abort the entire program
    setjmp(png_jmpbuf(png_ptr));
    // initialize png reading
    png_init_io(png_ptr, pngFile);
    // let it know we've already read the first 8 bytes
    png_set_sig_bytes(png_ptr, 8);
    // read in the basic image info
    png_read_info(png_ptr, info_ptr);
    // convert to 8 bits
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    if (bit_depth == 16)
        png_set_strip_16(png_ptr);
    // verify this is in RGBA format, and if not, convert it to RGBA
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    if (color_type != PNG_COLOR_TYPE_RGBA && color_type != PNG_COLOR_TYPE_RGB) {
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            if (bit_depth < 8)
                png_set_expand(png_ptr);
            png_set_gray_to_rgb(png_ptr);
        }
        if (color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_palette_to_rgb(png_ptr);
    }
    // convert tRNS to alpha channel
    if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) != 0U)
        png_set_tRNS_to_alpha(png_ptr);
    m_width  = png_get_image_width(png_ptr, info_ptr);
    m_height = png_get_image_height(png_ptr, info_ptr);
    png_read_update_info(png_ptr, info_ptr);
    // begin reading in the image
    setjmp(png_jmpbuf(png_ptr));
    size_t bpr          = png_get_rowbytes(png_ptr, info_ptr);// number of bytes in a row
    uint8_t numchannels = png_get_channels(png_ptr, info_ptr);
    // initialie our image storage
    m_pixels.resize(m_height * m_width);
    std::vector<png_byte> row(bpr);
    for (uint32_t irow = 0; irow < m_height; ++irow) {
        png_read_row(png_ptr, row.data(), nullptr);
        for (uint32_t icol = 0; icol < m_width; ++icol) {
            auto& pixel = getPixel(icol, irow);
            if (numchannels == 1) {
                // monochrome
                pixel = Color(row[icol], row[icol], row[icol], 255);
            } else if (numchannels == 2) {
                // monochrome + alpha
                pixel = Color(row[2 * icol], row[2 * icol], row[2 * icol], row[2 * icol + 1]);
            } else if (numchannels == 3) {
                // RGB
                pixel = Color(row[3 * icol], row[3 * icol + 1], row[3 * icol + 2], 255);
            } else if (numchannels == 4) {
                // RGBA
                pixel = Color(row[4 * icol], row[4 * icol + 1], row[4 * icol + 2], row[4 * icol + 3]);
            }
        }
    }
    // cleanup
    png_read_end(png_ptr, nullptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(pngFile);
}

void Texture::savePNG(const DataFile& file) {
    png_FILE_p pngFile = fopen(file.getFullPath().string().c_str(), "wb");
    png_structp png    = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info     = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) abort();
    png_init_io(png, pngFile);
    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
            png,
            info,
            m_width, m_height,
            8,
            PNG_COLOR_TYPE_RGBA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    std::vector<png_byte> row(4 * m_width);
    for (uint32_t irow = 0; irow < m_height; ++irow) {
        for (uint32_t icol = 0; icol < m_width; ++icol) {
            const auto& pixel = getPixel(icol, irow);
            row[4 * icol]     = pixel.red();
            row[4 * icol + 1] = pixel.green();
            row[4 * icol + 2] = pixel.blue();
            row[4 * icol + 3] = pixel.alpha();
        }
        png_write_row(png, row.data());
    }
    png_write_end(png, nullptr);
    fclose(pngFile);
    png_destroy_write_struct(&png, &info);
}

/// just a dummy color
static Color dummyColor{0, 0, 0, 0};

const Color& Texture::getPixel(uint16_t u, uint16_t v) const {
    if (u > width() || v > height())
        return dummyColor;
    return m_pixels[u * m_height + v];
    //return m_pixels[u + v * m_width];
}

Color& Texture::getPixel(uint16_t u, uint16_t v) {
    if (u > width() || v > height())
        return dummyColor;
    return m_pixels[u * m_height + v];
    //return m_pixels[u + v * m_width];
}

Color Texture::getPixel(uint16_t u, uint16_t v, uint16_t radius) const {
    if (u > width() || v > height())
        return Color{0, 0, 0, 0};
    int count = 0;
    double R{0};
    double G{0};
    double B{0};
    double A{0};
    for (int32_t i = -radius; i <= radius; ++i) {
        for (int32_t j = -radius; j <= radius; ++j) {
            if (u + i < 0 || v + j < 0) continue;
            auto uu = static_cast<uint16_t>(u + i);
            auto vv = static_cast<uint16_t>(v + j);
            if (uu > width() || vv > height()) continue;
            const auto& Pixel = getPixel(uu,vv);
            R += Pixel.redf();
            G += Pixel.greenf();
            B += Pixel.bluef();
            A += Pixel.alphaf();
            count++;
        }
    }
    A /= count;
    if (A < 0.5) return {0, 0, 0, 0};
    A = 1.0;
    R /= count;
    G /= count;
    B /= count;
    return Color::fromDouble(R, G, B, A);
}

}// namespace rc::graphics::image
