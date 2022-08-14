/**
 * @file Texture.h
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "core/fs/DataFile.h"
#include "graphics/Color.h"
#include <string>

/**
 * @brief Namespace for the images
 */
namespace rc::graphics::image {

/**
 * @brief Class Texture
 */
class Texture {
public:
    /**
     * @brief Default constructor.
     */
    Texture() = default;
    /**
     * @brief Default copy constructor
     */
    Texture(const Texture&) = default;
    /**
     * @brief Default move constructor
     */
    Texture(Texture&&) = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Texture& operator=(const Texture&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Texture& operator=(Texture&&) = default;
    /**
     * @brief Destructor.
     */
    virtual ~Texture() = default;

    /**
     * @brief Load texture from the file in data
     * @param textureName Texture's name to load
     */
    void loadFromFile(const std::string& textureName);
    /**
     * @brief Save texture to the file in data
     * @param textureName Texture's name to save
     */
    void saveToFile(const std::string& textureName);

    /**
     * @brief Get texture's width
     * @return Texture's width
     */
    [[nodiscard]] const size_t& width() const { return _width; }
    /**
     * @brief Get texture's height
     * @return Texture's height
     */
    [[nodiscard]] const size_t& height() const { return _height; }

    /**
     * @brief Get texture pixel at coordinate
     * @param u Horizontal coordinate
     * @param v Vertical coordinate
     * @return Color value
     */
    [[nodiscard]] const Color& getPixel(uint16_t u, uint16_t v) const;

    /**
     * @brief Get mean color value at coordinate
     * @param u Horizontal coordinate
     * @param v Vertical coordinate
     * @param radius Radius on the average
     * @return Color value
     */
    Color getPixel(uint16_t u, uint16_t v, uint16_t radius) const;

private:
    using DataFile = core::fs::DataFile;
    size_t _width  = 0;
    size_t _height = 0;
    std::vector<Color> _pixels;

    void readPNG(const DataFile& file);
    void savePNG(const DataFile& file);
};

}// namespace rc::graphics::image
