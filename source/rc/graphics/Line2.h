/**
 * @file Line2.h
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "math/Vector2.h"
#include <algorithm>
#include <cstdint>

namespace rc::graphics {

/**
 * @brief Class Line
 */
template<class DataType>
class Line2 {
public:
    /// Line vertex's type
    using VertexType = math::Vector2<DataType>;
    /**
     * @brief Default copy constructor
     */
    Line2(const Line2&) = default;
    /**
     * @brief Default move constructor
     */
    Line2(Line2&&)  noexcept = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Line2& operator=(const Line2&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Line2& operator=(Line2&&)  noexcept = default;
    /**
     * @brief Default constructor.
     */
    Line2() = default;
    /**
     * @brief Destructor.
     */
    virtual ~Line2() = default;//---UNCOVER---

    /**
     * @brief Construct by point
     * @param p1 First point
     * @param p2 Second point
     */
    Line2(const VertexType& p1, const VertexType& p2) :
        P1{p1}, P2{p2} {}

    /**
     * @brief Construct by point
     * @param p1 First point
     * @param p2 Second point
     */
    Line2(VertexType&& p1, VertexType&& p2) :
        P1{std::move(p1)}, P2{std::move(p2)} {}

    /**
     * @brief Constructor with point direction length
     * @param point Starting point
     * @param direction Line direction
     * @param length Line's length
     */
    Line2(const VertexType& point, const math::Vector2<DataType>& direction, const DataType& length) :
        P1{point}, P2{point + direction * length} {}
    /**
     * @brief Get the vertex
     * @param idx Vertex's id
     * @return Reference to the vertex
     */
    VertexType& getPoint(uint8_t idx) { return idx == 0 ? P1 : P2; }

    /**
     * @brief Get the vertex
     * @param idx Vertex's id
     * @return Reference to the vertex
     */
    const VertexType& getPoint(uint8_t idx) const { return idx == 0 ? P1 : P2; }

private:
    /// First point
    VertexType P1;
    /// Second point
    VertexType P2;
};

}// namespace rc::graphics
