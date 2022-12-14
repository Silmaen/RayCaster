/**
 * @file Color.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "math/functions.h"
#include <nlohmann/json.hpp>

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
    Color() = default;
    /**
     * @brief Default copy constructor
     */
    Color(const Color&) = default;
    /**
     * @brief Default move constructor
     */
    Color(Color&&) = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Color& operator=(const Color&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Color& operator=(Color&&) = default;
    /**
     * @brief Destructor.
     */
    ~Color() = default;
    /**
     * @brief Define color by its channels
     * @param red Red channel
     * @param green Green channel
     * @param blue Blue channel
     * @param alpha Alpha channel
     */
    constexpr Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255) :
        R{red}, G{green}, B{blue}, A{alpha} {}

    /**
     * @brief Define color by its channels
     * @param red Red channel
     * @param green Green channel
     * @param blue Blue channel
     * @param alpha Alpha channel
     * @return A color
     */
    static constexpr Color fromDouble(double red, double green, double blue, double alpha = 1.0) {
        return {static_cast<uint8_t>(math::clamp(red, 0.0, 1.0) * 255),
                static_cast<uint8_t>(math::clamp(green, 0.0, 1.0) * 255),
                static_cast<uint8_t>(math::clamp(blue, 0.0, 1.0) * 255),
                static_cast<uint8_t>(math::clamp(alpha, 0.0, 1.0) * 255)};
    }
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    uint8_t& red() { return R; }
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    [[nodiscard]] const uint8_t& red() const { return R; }
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    [[nodiscard]] double redf() const { return R / 255.0; }
    /**
     * @brief Get the green channel
     * @return The green channel
     */
    uint8_t& green() { return G; }
    /**
     * @brief Get the green channel
     * @return The green channel
     */
    [[nodiscard]] const uint8_t& green() const { return G; }
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    [[nodiscard]] double greenf() const { return G / 255.0; }
    /**
     * @brief Get the blue channel
     * @return The blue channel
     */
    uint8_t& blue() { return B; }
    /**
     * @brief Get the blue channel
     * @return The blue channel
     */
    [[nodiscard]] const uint8_t& blue() const { return B; }
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    [[nodiscard]] double bluef() const { return B / 255.0; }
    /**
     * @brief Get the alpha channel
     * @return The alpha channel
     */
    uint8_t& alpha() { return A; }
    /**
     * @brief Get the alpha channel
     * @return The alpha channel
     */
    [[nodiscard]] const uint8_t& alpha() const { return A; }
    /**
     * @brief Get the red channel
     * @return The red channel
     */
    [[nodiscard]] double alphaf() const { return A / 255.0; }

    /**
     * @brief Get the RGBA color code
     * @return The RGBA color code
     */
    [[nodiscard]] uint32_t code() const { return static_cast<uint32_t>(R) << 24 | static_cast<uint32_t>(G) << 16 | static_cast<uint32_t>(B) << 8 | A; }

    /**
     * @brief Comparison operator
     * @param other Other color to compare
     * @return True if equal
     */
    [[nodiscard]] bool operator==(const Color& other)const { return code() == other.code(); }

    /**
     * @brief Comparison operator
     * @param other Other color to compare
     * @return True if not equal
     */
    [[nodiscard]] bool operator!=(const Color& other)const { return code() != other.code(); }

    /**
     * @brief Make this color a bit darker
     * @return This
     */
    Color& darken(){
        R=static_cast<uint8_t>(0.9 * R);
        G=static_cast<uint8_t>(0.9 * G);
        B=static_cast<uint8_t>(0.9 * B);
        return *this;
    }

    /**
     * @brief Make this color a bit lighter
     * @return This
     */
    Color& lighten(){
        R=static_cast<uint8_t>(1.1 * R);
        G=static_cast<uint8_t>(1.1 * G);
        B=static_cast<uint8_t>(1.1 * B);
        return *this;
    }

    /**
     * @brief Get a darker version of this color
     * @return Darker color
     */
    [[nodiscard]] Color darker()const{
        Color temp{*this};
        return temp.darken();
    }

    /**
     * @brief Get a lighter version of this color
     * @return Lighter color
     */
    [[nodiscard]] Color lighter()const{
        Color temp{*this};
        return temp.lighten();
    }

private:
    uint8_t R = 0, G = 0, B = 0, A = 255;
};

/**
 * @brief Serialize this objet to json
 * @param jso The json output
 * @param color The vector to serialize
 */
inline void to_json(nlohmann::json& jso, const Color& color){
    jso = nlohmann::json{color.red(), color.green(), color.blue(), color.alpha()};
}
/**
 * @brief Deserialize this object from json
 * @param jso Json source
 * @param color Destination vector
 */
inline void from_json(const nlohmann::json& jso, Color& color){
    color.red() = jso.at(0);
    color.green() = jso.at(1);
    color.blue() = jso.at(2);
    color.alpha() = jso.at(3);
}

}// namespace rc::graphics
