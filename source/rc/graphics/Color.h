/**
 * @file Color.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <cstdint>

/**
 * @brief Namespace for graphical object
 */
namespace rc::graphics {

/**
 * @brief Class Color
 */
class Color {
public:
    /**
     * @brief Default constructor.
     */
    Color()= default;
    /**
     * @brief Default copy constructor
     */
    Color(const Color&)= default;
    /**
     * @brief Default move constructor
     */
    Color(Color&&)= default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Color& operator=(const Color&)= default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Color& operator=(Color&&)= default;
    /**
     * @brief Destructor.
     */
    ~Color()= default;
    /**
     * @brief Define color by its channels
     * @param red Red channel
     * @param green Green channel
     * @param blue Blue channel
     * @param alpha Alpha channel
     */
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha=255):R{red},G{green},B{blue},A{alpha}{}

    /**
     * @brief Get the red channel
     * @return The red channel
     */
    uint8_t& red(){return R;}
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    [[nodiscard]] const uint8_t& red()const{return R;}
    /**
     * @brief Get the green channel
     * @return The green channel
     */
    uint8_t& green(){return G;}
    /**
     * @brief Get the green channel
     * @return The green channel
     */
    [[nodiscard]] const uint8_t& green()const {return G;}
    /**
     * @brief Get the blue channel
     * @return The blue channel
     */
    uint8_t& blue(){return B;}
    /**
     * @brief Get the blue channel
     * @return The blue channel
     */
    [[nodiscard]] const uint8_t& blue()const {return B;}
    /**
     * @brief Get the alpha channel
     * @return The alpha channel
     */
    uint8_t& alpha(){return A;}
    /**
     * @brief Get the alpha channel
     * @return The alpha channel
     */
    [[nodiscard]] const uint8_t& alpha()const{return A;}
private:
    uint8_t R=0,G=0,B=0,A=255;
};

}// namespace rc::graphics
