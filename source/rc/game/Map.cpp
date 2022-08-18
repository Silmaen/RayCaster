/**
 * @file Map.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Map.h"
#include "core/fs/DataFile.h"
#include <fstream>

namespace rc::game {

/// Map Color list
static const std::vector<graphics::Color> mapColors{
        graphics::Color{255, 0, 255},
        graphics::Color{255, 0, 255},
        graphics::Color{0x60, 0x60, 0x60},
        graphics::Color{0x40, 0x40, 0x40},
        graphics::Color{0x19, 0x19, 0x8c},
        graphics::Color{0x0a, 0x0a, 0x64},
        graphics::Color{0x80, 0x40, 0x10},
        graphics::Color{0x64, 0x32, 0x08},
};
/// Ray color list
static const std::vector<graphics::Color> rayColors{
        graphics::Color{0, 0, 0},
        graphics::Color{0, 0, 0},
        graphics::Color{0x80, 0x80, 0x80},
        graphics::Color{0x70, 0x70, 0x70},
        graphics::Color{0x39, 0x39, 0xbc},
        graphics::Color{0x3a, 0x3a, 0x94},
        graphics::Color{0xb0, 0x70, 0x40},
        graphics::Color{0x94, 0x62, 0x38},
};
/// Texture list
static const std::vector<std::string> mapTextures{
        "doorpattern.png",
        "doorpattern.png",
        "greystone.png",
        "greystone.png",
        "bluestone.png",
        "bluestone.png",
        "wood.png",
        "wood.png",
};

const graphics::Color& mapCell::getMapColor() const {
    return mapColors[textureId];
}

const graphics::Color& mapCell::getRayColor() const {
    return rayColors[textureId];
}

const std::string& mapCell::getTextureName() const {
    return mapTextures[textureId];
}

Map::Map(const Map::DataType& data, uint8_t cube) :
    cubeSize{cube}, mapArray{data} { updateSize(); }

Map::Map(Map::DataType&& data, uint8_t cube) :
    cubeSize{cube},
    mapArray{std::move(data)} { updateSize(); }

void Map::reset(uint8_t width, uint8_t height) {
    // bad size
    if (width * height == 0) return;
    mapArray.resize(height);
    for (LineType& line : mapArray) {
        line.resize(width);
        std::fill(line.begin(), line.end(), mapCell{false, false, 2});
    }
    PlayerInitialPosition  = {0, 0};
    PlayerInitialDirection = {0, 1};
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

Map::rayCastResult Map::castRay(const worldCoordinates& from, const worldCoordinates& direction) const {
    //gridCoordinate playerCell = whichCell(from);
    // check for vertical line
    double verticalDistance = -1;
    worldCoordinates verticalPoint{from};
    double verticalCellRatio = 0;
    if (std::abs(direction[0]) > 0.001)  {
        // Intersection with verticals
        worldCoordinates verticalOffset{};
        verticalOffset = direction / std::abs(direction[0]);
        verticalPoint  = from + math::geometry::Vectf{math::sign(direction[0]) * 0.001, 0.0} + verticalOffset * std::abs((static_cast<int32_t>(from[0] / cubeSize) + math::heaviside(direction[0])) * cubeSize - from[0]);
        verticalOffset *= cubeSize;
        gridCoordinate verticalCell = whichCell(verticalPoint);
        while (isInVisible(verticalCell)) {
            verticalPoint += verticalOffset;
            verticalCell = whichCell(verticalPoint);
        }
        if (isIn(verticalCell))
            verticalDistance = (verticalPoint - from).lengthSQ();
        verticalCellRatio = std::abs(verticalPoint[1] - (verticalCell[1] + math::heaviside(-direction[0])) * cubeSize);
    }
    // check for horizontal line
    double horizontalDistance = -1;
    worldCoordinates horizontalPoint{from};
    double horizontalCellRatio = 0;
    if (std::abs(direction[1]) > 0.001) {
        // Intersection with verticals
        worldCoordinates horizontalOffset{};
        horizontalOffset = direction / std::abs(direction[1]);
        horizontalPoint    = from + math::geometry::Vectf{0.0, math::sign(direction[1]) * 0.001} + horizontalOffset * std::abs((static_cast<int32_t>(from[1] / cubeSize) + math::heaviside(direction[1])) * cubeSize - from[1]);
        horizontalOffset *= cubeSize;
        gridCoordinate horizontalCell = whichCell(horizontalPoint);
        while (isInVisible(horizontalCell)) {
            horizontalPoint += horizontalOffset;
            horizontalCell = whichCell(horizontalPoint);
        }
        if (isIn(horizontalCell))
            horizontalDistance = (horizontalPoint - from).lengthSQ();
        horizontalCellRatio = std::abs(horizontalPoint[0] - (horizontalCell[0] + math::heaviside(direction[1])) * cubeSize);
    }
    if (verticalDistance < 0)
        return {std::sqrt(horizontalDistance), horizontalPoint, false, horizontalCellRatio};
    if (horizontalDistance < 0)
        return {std::sqrt(verticalDistance), verticalPoint, true, verticalCellRatio};
    if (horizontalDistance < verticalDistance)
        return {std::sqrt(horizontalDistance), horizontalPoint, false, horizontalCellRatio};
    return {std::sqrt(verticalDistance), verticalPoint, true, verticalCellRatio};
}

Map::gridCoordinate Map::whichCell(const worldCoordinates& from) const {
    gridCoordinate result;
    result[0] = static_cast<unsigned char>(static_cast<uint64_t>(from[0]) / cubeSize);
    result[1] = static_cast<unsigned char>(static_cast<uint64_t>(from[1]) / cubeSize);
    return result;
}

bool Map::isIn(const worldCoordinates& from) const {
    return from[0] >= 0 && from[0] <= maxWidth && from[1] >= 0 && from[1] <= maxHeight;
}

bool Map::isIn(const gridCoordinate& from) const {
    return from[0] < width() && from[1] < height();
}

bool Map::isInPassable(const worldCoordinates& from) const {
    return isIn(from) && at(whichCell(from)).passable;
}

bool Map::isInPassable(const Map::gridCoordinate& from) const {
    return isIn(from) && at(from).passable;
}

bool Map::isInVisible(const worldCoordinates& from) const {
    return isIn(from) && at(whichCell(from)).visibility;
}

bool Map::isInVisible(const Map::gridCoordinate& from) const {
    return isIn(from) && at(from).visibility;
}

Map::worldCoordinates Map::possibleMove(const worldCoordinates& Start, const worldCoordinates& Expected) const {
    if (isInPassable(Start + Expected))
        return Expected;
    if (isInPassable(Start + worldCoordinates{Expected[0], 0.0}))
        return {Expected[0], 0.0};
    if (isInPassable(Start + worldCoordinates{0.0, Expected[1]}))
        return {0.0, Expected[1]};
    return worldCoordinates();
}

void Map::updateSize() {
    maxWidth  = static_cast<double>(width() * cubeSize);
    maxHeight = static_cast<double>(height() * cubeSize);
}

void Map::loadFromFile(const std::string& mapName) {
    auto file = std::filesystem::path(mapName);
    std::ifstream jStream(file);
    if (!jStream.is_open()) return;
    auto data = nlohmann::json::parse(jStream);
    jStream.close();
    fromJson(data);
}

void Map::saveToFile(const std::string& mapName) {
    nlohmann::json data = toJson();
    core::fs::DataFile file;
    file.setPath(std::filesystem::path(mapName));
    file.writeJson(data);
}

void Map::loadFromData(const std::string& mapName) {
    core::fs::DataFile file;
    file.setPath(std::filesystem::path("maps") / (mapName + ".map"));
    auto data = file.readJson();
    fromJson(data);
}

void Map::saveToData(const std::string& mapName) {
    nlohmann::json data = toJson();
    core::fs::DataFile file;
    file.setPath(std::filesystem::path("maps") / (mapName + ".map"));
    file.writeJson(data);
}

void Map::fromJson(const nlohmann::json& data) {
    [[maybe_unused]] uint8_t version = data["version"];// but still to be read when map will contain more data
    cubeSize                         = data["cubeSize"];
    mapArray                         = data["cells"];
    PlayerInitialPosition            = data["playerStart"];
    PlayerInitialDirection           = data["playerStartDir"];
    updateSize();
}

nlohmann::json Map::toJson() const {
    nlohmann::json data;
    data["version"]        = 2;
    data["width"]          = width();
    data["height"]         = height();
    data["cubeSize"]       = cubeSize;
    data["cells"]          = mapArray;
    data["playerStart"]    = PlayerInitialPosition;
    data["playerStartDir"] = PlayerInitialDirection;
    return data;
}

}// namespace rc::game
