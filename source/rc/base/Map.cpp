/**
 * @file Map.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Map.h"
#include "Engine.h"
#include "graphics/Quad2.h"

namespace rc::base {

Map::Map(const Map::DataType& data, uint8_t cube) :
    cubeSize{cube}, mapArray{data} { updateSize(); }

Map::Map(Map::DataType&& data, uint8_t cube) :
    cubeSize{cube},
    mapArray{std::move(data)} { updateSize(); }

void Map::reset(uint8_t w, uint8_t h) {
    // bad size
    if (w * h == 0) return;
    mapArray.resize(h);
    for (LineType& line : mapArray) {
        line.resize(w);
        std::fill(line.begin(), line.end(), 0);
    }
    updateSize();
}

void Map::setMap(const DataType& data) {
    mapArray = data;
    updateSize();
}

void Map::setMap(DataType&& data) {
    mapArray = std::move(data);
    updateSize();
}

void Map::draw() {
    auto& engine = Engine::get();
    double x    = 0;
    double y    = 0;
    for (LineType& line : mapArray) {
        for (uint8_t cell : line) {
            engine.drawQuad(
                    {{x + 1, y + 1},
                     {x + 1, y - 1 + cubeSize},
                     {x - 1 + cubeSize, y - 1 + cubeSize},
                     {x - 1 + cubeSize, y + 1}},
                     cell > 0 ? graphics::Color{255, 255, 255, 255} : graphics::Color{0, 0, 0, 255});
            x += cubeSize;
        }
        x = 0;
        y += cubeSize;
    }
}

bool Map::isValid() const {
    if (mapArray.empty()) return false;
    size_t height = mapArray.front().size();
    if (height == 0) return false;
    for (const auto& line : mapArray) {
        if (line.size() != height) return false;
    }
    return true;
}

Map::BaseType& Map::at(const gridCoordinate& location) {
    return mapArray[location[1]][location[0]];
}

const Map::BaseType& Map::at(const gridCoordinate& location) const {
    return mapArray[location[1]][location[0]];
}


Map::rayCastResult Map::castRay(const math::Vector2<double>& from, const math::Vector2<double>& direction) const {
    gridCoordinate playerCell = whichCell(from);
    // check for vertical line
    double verticalDistance = 100000000;
    math::Vector2<double> verticalPoint{from};
    {
        // Intersection with verticalsq
        math::Vector2<double> verticalOffset{};
        double progression = 0;
        if (direction[0] > 0.001) {// look to the right
            verticalOffset[0] = cubeSize;
            progression       = direction[1] / direction[0];
            verticalPoint[0]  = cubeSize * (playerCell[0] + 1) + 0.001;
        } else if (direction[0] < -0.001) {// look to the left
            verticalOffset[0] = -cubeSize;
            progression       = direction[1] / direction[0];
            verticalPoint[0]  = cubeSize * playerCell[0] - 0.001;// just a little inside the left cube
        } else {// ray is almost vertical: no projection
            verticalDistance = -1;
        }
        if (verticalDistance > 0) {
            verticalOffset[1] = verticalOffset[0] * progression;
            verticalPoint[1]  = from[1] + progression * (verticalPoint[0] - from[0]);
            while (isIn(verticalPoint) && at(whichCell(verticalPoint)) == 0) {
                verticalPoint += verticalOffset;
            }
            if (isIn(verticalPoint)) {
                verticalDistance = (verticalPoint - from).lengthSQ();
            }
        }
    }
    // check for horizontal line
    double horizontalDistance = 100000000;
    math::Vector2<double> horizontalPoint{from};
    {
        // Intersection with verticals
        math::Vector2<double> horizontalOffset{};
        double progression = 0;
        if (direction[1] > 0.001) {// look to the bottom
            horizontalOffset[1] = cubeSize;
            progression         = direction[0] / direction[1];
            horizontalPoint[1]  = cubeSize * (playerCell[1] + 1) + 0.001;
        } else if (direction[1] < -0.001) {// look to the top
            horizontalOffset[1] = -cubeSize;
            progression         = direction[0] / direction[1];
            horizontalPoint[1]  = cubeSize * playerCell[1] - 0.001;// just a little inside the left cube
        } else {// ray is almost horizontal: no projection
            horizontalDistance = -1;
        }
        if (horizontalDistance > 0) {
            horizontalOffset[0] = horizontalOffset[1] * progression;
            horizontalPoint[0]  = from[0] + progression * (horizontalPoint[1] - from[1]);
            while (isIn(horizontalPoint) && at(whichCell(horizontalPoint)) == 0) {
                horizontalPoint += horizontalOffset;
            }
            if (isIn(horizontalPoint)) {
                horizontalDistance = (horizontalPoint - from).lengthSQ();
            }
        }
    }
    if (verticalDistance < 0)
        return {std::sqrt(horizontalDistance), horizontalPoint, false};
    if (horizontalDistance < 0)
        return {std::sqrt(verticalDistance), verticalPoint, true};
    if (horizontalDistance < verticalDistance)
        return {std::sqrt(horizontalDistance), horizontalPoint, false};
    return {std::sqrt(verticalDistance), verticalPoint, true};
}

Map::gridCoordinate Map::whichCell(const math::Vector2<double>& from) const {
    gridCoordinate result;
    result[0] = static_cast<uint64_t>(from[0]) / cubeSize;
    result[1] = static_cast<uint64_t>(from[1]) / cubeSize;
    return result;
}

bool Map::isIn(const math::Vector2<double>& from) const {
    return from[0] >= 0 && from[0] <= maxWidth && from[1] >= 0 && from[1] <= maxHeight;
}

bool Map::isIn(const gridCoordinate& from) const {
    return from[0] < width() && from[1] < height();
}

void Map::updateSize() {
    maxWidth  = width() * cubeSize;
    maxHeight = height() * cubeSize;
}

}// namespace rc::base
