/**
 * @file Texture.cpp
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Texture.h"
#include <fstream>
#include <iostream>
#include <png.h>

namespace rc::graphics::image {


void Texture::loadFromFile(const std::string& textureName) {
    DataFile file("textures/" + textureName);
    if (!file.exists()) return;
    readPNG(file);
}

void Texture::saveToFile(const std::string& textureName){
    DataFile file("textures/" + textureName);
    savePNG(file);
}

void Texture::readPNG(const DataFile& file){
    png_FILE_p pngFile = fopen(file.getFullPath().string().c_str(), "rb");
    png_byte header[8];
    fread(header, 1, 8, pngFile);
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info_ptr = png_create_info_struct(png_ptr);
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
    _width  = png_get_image_width(png_ptr, info_ptr);
    _height = png_get_image_height(png_ptr, info_ptr);
    png_read_update_info(png_ptr, info_ptr);
    // begin reading in the image
    setjmp(png_jmpbuf(png_ptr));
    size_t bpr          = png_get_rowbytes(png_ptr, info_ptr);// number of bytes in a row
    uint8_t numchannels = png_get_channels(png_ptr, info_ptr);
    // initialie our image storage
    _pixels.resize(_height * _width);
    std::vector<png_byte> row(bpr);
    for (auto& pixel : _pixels) {
        auto GlobalIndex = static_cast<size_t>(&pixel - _pixels.data());
        size_t RowIndex    = (GlobalIndex % _width) * numchannels;
        if (RowIndex == 0)
            png_read_row(png_ptr, row.data(), nullptr);
        if (numchannels == 1) {
            // monochrome
            pixel = Color(row[RowIndex], row[RowIndex], row[RowIndex], 255);
        } else if (numchannels == 2) {
            // monochrome + alpha
            pixel = Color(row[RowIndex], row[RowIndex], row[RowIndex], row[RowIndex + 1]);
        } else if (numchannels == 3) {
            // RGB
            pixel = Color(row[RowIndex], row[RowIndex + 1], row[RowIndex + 2], 255);
        } else if (numchannels == 4) {
            // RGBA
            pixel = Color(row[RowIndex], row[RowIndex + 1], row[RowIndex + 2], row[RowIndex + 3]);
        }
    }
    // cleanup
    png_read_end(png_ptr, nullptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(pngFile);
}

void Texture::savePNG(const DataFile& file){
    png_FILE_p pngFile = fopen(file.getFullPath().string().c_str(), "wb");
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) abort();
    png_init_io(png, pngFile);
    // Output is 8bit depth, RGBA format.
    png_set_IHDR(
            png,
            info,
            _width, _height,
            8,
            PNG_COLOR_TYPE_RGBA,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    std::vector<png_byte> row(4*_width);
    uint16_t counter = 0;
    for(auto& pixel:_pixels){
        row[counter] = pixel.red();
        row[counter+1] = pixel.green();
        row[counter+2] = pixel.blue();
        row[counter+3] = pixel.alpha();
        counter+=4;
        if (counter>=row.size()) {
            png_write_row(png, row.data());
            counter = 0;
        }
    }
    //png_write_image(png, reinterpret_cast<png_bytepp>(_pixels.data()));
    png_write_end(png, nullptr);
    fclose(pngFile);
    png_destroy_write_struct(&png, &info);

}

/// just a dummy color
static Color dummyColor{0,0,0,0};

const Color& Texture::getPixel(uint16_t u, uint16_t v)const{
    if (u>width() || v > height())
        return dummyColor;
    return _pixels[u + _width * v];
}

Color Texture::getPixel(uint16_t u, uint16_t v, uint16_t radius)const{
    if (u>width() || v > height())
        return Color{0,0,0,0};
    int count = 0;
    double R{0};
    double G{0};
    double B{0};
    double A{0};
    for (int32_t i=-radius;i<=radius;++i){
        for (int32_t j=-radius;j<=radius;++j){
            if (u+i < 0 || v+j<0 ) continue;
            uint16_t uu = static_cast<uint16_t>(u + i);
            uint16_t vv = static_cast<uint16_t>(v + j);
            if (uu > width() || vv > height()) continue;
            size_t idx = uu + _width * vv;
            Color Pixel = _pixels[idx];
            R += Pixel.redf();
            G += Pixel.greenf();
            B += Pixel.bluef();
            A += Pixel.alphaf();
            count++;
        }
    }
    A /= count;
    if (A<0.5) return {0,0,0,0};
    A = 1.0;
    R/=count;
    G/=count;
    B/=count;
    return Color::fromDouble(R,G,B,A);
}

}// namespace rc::graphics
