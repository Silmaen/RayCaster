/**
 * @file Quad2.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "Color.h"
#include "math/Vector2.h"
#include <algorithm>

namespace rc::graphics {

/**
 * @brief Class Quad2
 * @tparam DataType Data's type
 */
template<class DataType>
class Quad2 {
public:
    /// Quad's vertex type
    using VertexType= math::Vector2<DataType>;
    /**
     * @brief Default constructor.
     */
    Quad2()= default;
    /**
     * @brief Default copy constructor
     */
    Quad2(const Quad2&)= default;
    /**
     * @brief Default move constructor
     */
    Quad2(Quad2&&) noexcept= default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Quad2& operator=(const Quad2&)= default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Quad2& operator=(Quad2&&) noexcept= default;
    /**
     * @brief Destructor.
     */
    virtual ~Quad2()= default;

    /**
     * @brief Define a quad with its points and color
     * @param p1 First Vertex
     * @param p2 Second Vertex
     * @param p3 Third Vertex
     * @param p4 Fourth Vertex
     */
    Quad2(const VertexType& p1, const VertexType& p2, const VertexType& p3, const VertexType& p4):
        P1{p1}, P2{p2}, P3{p3}, P4{p4} {}

    /**
     * @brief Define a quad with its points and color
     * @param p1 First Vertex
     * @param p2 Second Vertex
     * @param p3 Third Vertex
     * @param p4 Fourth Vertex
     */
    Quad2(VertexType&& p1, VertexType&& p2, VertexType&& p3, VertexType&& p4):
        P1{std::move(p1)}, P2{std::move(p2)}, P3{std::move(p3)}, P4{std::move(p4)} {}

    /**
     * @brief Get the vertex
     * @param idx Vertex's id
     * @return Reference to the vertex
     */
    VertexType& getPoint(uint8_t idx) { return idx == 0 ? P1 : idx == 1 ? P2 :
                                                       idx == 2         ? P3 :
                                                                          P4; }
    /**
     * @brief Get the vertex
     * @param idx Vertex's id
     * @return Reference to the vertex
     */
    const VertexType& getPoint(uint8_t idx)const { return idx == 0 ? P1 : idx == 1 ? P2 :
                                                       idx == 2         ? P3 :
                                                                          P4; }
private:
    /// First vertex
    VertexType P1;
    /// Second vertex
    VertexType P2;
    /// Third vertex
    VertexType P3;
    /// Fourth vertex
    VertexType P4;
};

}// namespace rc::graphics
