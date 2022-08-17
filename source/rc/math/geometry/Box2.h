/**
 * @file Box2.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <utility>

#include "Quad2.h"

namespace rc::math::geometry {
/**
 * @brief Class Box2
 */
class Box2 {
public:
    /// Line vertex's type
    using VertexType = math::geometry::Vector2<int32_t>;
    /**
     * @brief Default constructor.
     */
    Box2() = default;
    /**
     * @brief Default copy constructor
     */
    Box2(const Box2&) = default;
    /**
     * @brief Default move constructor
     */
    Box2(Box2&&) = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Box2& operator=(const Box2&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Box2& operator=(Box2&&) = default;
    /**
     * @brief Destructor.
     */
    ~Box2() = default;

    /**
     * @brief Construct by value
     * @param topLeft_ Lower point
     * @param bottomRight_ Upper point
     */
    Box2(VertexType&& topLeft_, VertexType&& bottomRight_) :
        topLeft{std::move(topLeft_)}, bottomRight{std::move(bottomRight_)} {}
    /**
     * @brief Comparison operator
     * @param other Other box to compare
     * @return True if equal
     */
    bool operator==(const Box2& other) const { return topLeft == other.topLeft && bottomRight == other.bottomRight; }
    /**
     * @brief Comparison operator
     * @param other Other box to compare
     * @return True if not equal
     */
    bool operator!=(const Box2& other) const { return topLeft != other.topLeft || bottomRight != other.bottomRight; }
    /**
     * @brief Data access
     * @param idx component index
     * @return data
     */
    VertexType& operator[](uint8_t idx) { return idx == 0 ? topLeft : bottomRight; }
    /**
     * @brief Data access
     * @param idx component index
     * @return data
     */
    const VertexType& operator[](uint8_t idx) const { return idx == 0 ? topLeft : bottomRight; }
    /**
     * @brief Check box validity
     * @return True if corners at the right place
     */
    [[nodiscard]] bool isValid() const { return topLeft[0] < bottomRight[0] && topLeft[1] < bottomRight[1]; }
    /**
     * @brief Get the area
     * @return The area
     */
    [[nodiscard]] int32_t surface() const { return isValid() ? 1 : -1 * width() * (bottomRight[1] - topLeft[1]); }

    /**
     * @brief Check if a pixel is in the box
     * @param vtx Pixel to check
     * @return True if in the box.
     */
    [[nodiscard]] bool isIn(const VertexType& vtx) const { return topLeft[0] <= vtx[0] && vtx[0] <= bottomRight[0] && topLeft[1] <= vtx[1] && vtx[1] <= bottomRight[1]; }
    /**
     * @brief Check if a box is in the box
     * @param box Box to check
     * @return True if in the box.
     */
    [[nodiscard]] bool isIn(const Box2& box) const { return isIn(box.topLeft) && isIn(box.bottomRight); }

    /**
     * @brief Check if a box intersect the box
     * @param box Box to check
     * @return True if intersect the box.
     */
    [[nodiscard]] bool intersect(const Box2& box) const { return isIn(box.topLeft) || isIn(box.bottomRight); }

    /**
     * @brief Get the box width
     * @return Width
     */
    [[nodiscard]] int32_t width() const { return bottomRight[0] - topLeft[0]; }
    /**
     * @brief Get the box height
     * @return Height
     */
    [[nodiscard]] int32_t height() const { return bottomRight[1] - topLeft[1]; }
    /**
     * @brief Get the center of the box
     * @return The center of the box
     */
    [[nodiscard]] VertexType center() const { return {(bottomRight[0] + topLeft[0]) / 2, (bottomRight[1] + topLeft[1]) / 2}; }

    /**
     * @brief Get the quad associated with the box
     * @return A quad
     */
    [[nodiscard]] Quad2<double> getQuad() const {
        return {{static_cast<double>(topLeft[0]), static_cast<double>(topLeft[1])},
                {static_cast<double>(topLeft[0]), static_cast<double>(bottomRight[1])},
                {static_cast<double>(bottomRight[0]), static_cast<double>(bottomRight[1])},
                {static_cast<double>(bottomRight[0]), static_cast<double>(topLeft[1])}};
    }

    /**
     * @brief Get the right side
     * @return Right coordinate
     */
    [[nodiscard]] const int32_t& right() const {
        return bottomRight[0];
    }

    /**
     * @brief Get the bottom side
     * @return Bottom coordinate
     */
    [[nodiscard]] const int32_t& bottom() const {
        return bottomRight[1];
    }

    /**
     * @brief Get the left side
     * @return Left coordinate
     */
    [[nodiscard]] const int32_t& left() const {
        return topLeft[0];
    }

    /**
     * @brief Get the top side
     * @return Top coordinate
     */
    [[nodiscard]] const int32_t& top() const {
        return topLeft[1];
    }

private:
    /// Top Left Corner
    VertexType topLeft;
    /// Bottom right corner
    VertexType bottomRight;
};

/**
 * @brief Serialize this objet to json
 * @param jso The json output
 * @param box The vector to serialize
 */
inline void to_json(nlohmann::json& jso, const Box2& box) {
    jso = nlohmann::json{box[0], box[1]};
}
/**
 * @brief Deserialize this object from json
 * @param jso Json source
 * @param box Destination vector
 */
inline void from_json(const nlohmann::json& jso, Box2& box) {
    box[0] = jso.at(0);
    box[1] = jso.at(1);
}


}// namespace rc::math::geometry
