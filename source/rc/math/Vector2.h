/**
 * @file Vector2.h
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include <cmath>
#include <cstdint>
#include "Angle.h"

namespace rc::math {

/**
 * @brief Class Vector2
 */
template<class DataType>
class Vector2 {
public:
    /**
     * @brief Default copy constructor
     */
    Vector2(const Vector2&)= default;
    /**
     * @brief Default move constructor
     */
    Vector2(Vector2&&) noexcept = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Vector2& operator=(const Vector2&)= default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Vector2& operator=(Vector2&&) noexcept = default;
    /**
     * @brief Default constructor.
     */
    Vector2()= default;
    /**
     * @brief Destructor.
     */
    virtual ~Vector2()= default;//---UNCOVER---
    /**
     * @brief Constructor with coordinates
     * @param x First coordinates
     * @param y Second coordinates
     */
    Vector2(DataType x, DataType y):X(x),Y(y) {}
    /**
     * @brief Data access
     * @param idx component index
     * @return data
     */
    DataType& operator[](uint8_t idx){return idx == 0 ? X : Y;}
    /**
     * @brief Data access
     * @param idx component index
     * @return data
     */
    const DataType& operator[](uint8_t idx)const{return idx == 0 ? X : Y;}

    /**
     * @brief Addition operator
     * @param other Other vector to add
     * @return This actualized vector
     */
    Vector2& operator+=(const Vector2& other){X+=other.X;Y+=other.Y;return *this;}

    /**
     * @brief Sub operator
     * @param other Other vector to sub
     * @return This actualized vector
     */
    Vector2& operator-=(const Vector2& other){X-=other.X;Y-=other.Y;return *this;}

    /**
     * @brief product operator
     * @param other Scalar to multiply
     * @return This actualized vector
     */
    Vector2& operator*=(const DataType& other){X*=other;Y*=other;return *this;}

    /**
     * @brief Division operator
     * @param other Scalar to divide
     * @return This actualized vector
     */
    Vector2& operator/=(const DataType& other){X/=other;Y/=other;return *this;}

    /**
     * @brief Addition operator
     * @param other Other vector to add
     * @return Result vector
     */
    Vector2 operator+(const Vector2& other)const{Vector2 res{*this};res+=other;return res;}
    /**
     * @brief Sub operator
     * @param other Other vector to sub
     * @return Result vector
     */
    Vector2 operator-(const Vector2& other)const{Vector2 res{*this};res-=other;return res;}
    /**
     * @brief Product operator
     * @param other Scalar to multiply
     * @return Result vector
     */
    Vector2 operator*(const DataType& other)const{Vector2 res{*this};res*=other;return res;}
    /**
     * @brief Division operator
     * @param other Scalar to divide
     * @return Result vector
     */
    Vector2 operator/(const DataType& other)const{Vector2 res{*this};res/=other;return res;}

    /**
     * @brief Dot product
     * @param other Other vector to multiply
     * @return The dot product
     */
    DataType dot(const Vector2& other)const {return X*other.X+Y*other.Y;}
    /**
     * @brief Get the square length
     * @return Square length
     */
    DataType lengthSQ()const{return X*X+Y*Y;}
    /**
     * @brief Get the length
     * @return Length
     */
    DataType length()const{return std::sqrt(lengthSQ());}

    /**
     * @brief Rotate the vector by the given angle
     * @param angle Rotation's angle
     * @return This actualized vector
     */
    Vector2& rotate(const Angle& angle){
        double ca = std::cos(angle.get());
        double sa = std::sin(angle.get());
        auto Tx = static_cast<DataType>(ca * X - sa * Y);
        Y = static_cast<DataType>(sa * X + ca * Y);
        std::swap(X, Tx);
        return *this;
    }
    /**
     * @brief Rotate the vector by the given angle
     * @param angle Rotation's angle
     * @return Rotated vector
     */
    Vector2 rotated(const Angle& angle)const{
        Vector2 tmp{*this};
        tmp.rotate(angle);
        return tmp;
    }
    /**
     * @brief Get the vector angle with X axis
     * @return Vector Angle
     */
    [[nodiscard]] Angle getAngle()const{
        double len =length();
        return Angle{std::atan2(Y/len,X/len),Angle::Unit::Radian};
    }

private:
    /// First coordinate
    DataType X{};
    /// Second coordinate
    DataType Y{};
};

}// namespace rc::math
