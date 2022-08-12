/**
 * @file Map.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Map.h"
#include "fs/DataFile.h"
#include <fstream>
#include <iostream>

namespace rc::core {

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

const graphics::Color& mapCell::getMapColor()const{
    return mapColors[textureId];
}
const graphics::Color& mapCell::getRayColor()const{
    return rayColors[textureId];
}

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
        std::fill(line.begin(), line.end(), mapCell{false,false,2});
    }
    PlayerInitialPosition  = {0,0};
    PlayerInitialDirection = {0,1};
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
        } else {                                                 // ray is almost vertical: no projection
            verticalDistance = -1;
        }
        if (verticalDistance > 0) {
            verticalOffset[1] = verticalOffset[0] * progression;
            verticalPoint[1]  = from[1] + progression * (verticalPoint[0] - from[0]);
            while (isInVisible(verticalPoint)) {
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
        } else {                                                   // ray is almost horizontal: no projection
            horizontalDistance = -1;
        }
        if (horizontalDistance > 0) {
            horizontalOffset[0] = horizontalOffset[1] * progression;
            horizontalPoint[0]  = from[0] + progression * (horizontalPoint[1] - from[1]);
            while (isInVisible(horizontalPoint)) {
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
    result[0] = static_cast<unsigned char>(static_cast<uint64_t>(from[0]) / cubeSize);
    result[1] = static_cast<unsigned char>(static_cast<uint64_t>(from[1]) / cubeSize);
    return result;
}

bool Map::isIn(const math::Vector2<double>& from) const {
    return from[0] >= 0 && from[0] <= maxWidth && from[1] >= 0 && from[1] <= maxHeight;
}

bool Map::isIn(const gridCoordinate& from) const {
    return from[0] < width() && from[1] < height();
}

bool Map::isInPassable(const math::Vector2<double>& from) const {
    return isIn(from) && at(whichCell(from)).passable;
}
bool Map::isInPassable(const Map::gridCoordinate& from) const {
    return isIn(from) && at(from).passable;
}

bool Map::isInVisible(const math::Vector2<double>& from) const {
    return isIn(from) && at(whichCell(from)).visibility;
}
bool Map::isInVisible(const Map::gridCoordinate& from) const {
    return isIn(from) && at(from).visibility;
}

void Map::updateSize() {
    maxWidth  = static_cast<double>(width() * cubeSize);
    maxHeight = static_cast<double>(height() * cubeSize);
}

void Map::loadFromFile(const std::string& mapName){
    auto file=std::filesystem::path(mapName);
    std::ifstream jStream(file);
    if (! jStream.is_open()){
        std::cout << "Problem opening file " << file.string() << "\n";
        return;
    }
    auto data = nlohmann::json::parse(jStream);
    jStream.close();
    fromJson(data);
}

void Map::saveToFile(const std::string& mapName){
    nlohmann::json data = toJson();
    fs::DataFile file;
    file.setPath(std::filesystem::path(mapName));
    file.writeJson(data);
}

void Map::loadFromData(const std::string& mapName) {
    fs::DataFile file;
    file.setPath(std::filesystem::path("maps") / (mapName + ".map"));
    auto data       = file.readJson();
    fromJson(data);
}

void Map::saveToData(const std::string& mapName) {
    nlohmann::json data = toJson();
    fs::DataFile file;
    file.setPath(std::filesystem::path("maps") / (mapName + ".map"));
    file.writeJson(data);
}

void Map::fromJson(const nlohmann::json& data){
    uint8_t version = data["version"];
    cubeSize        = data["cubeSize"];
    if (version == 1) {
        std::vector<std::vector<uint8_t>> dataTmp = data["cells"];
        reset(dataTmp[0].size(), dataTmp.size());
        for (uint32_t i = 0; i < dataTmp.size(); ++i) {
            for (uint32_t j = 0; j < dataTmp[0].size(); ++j) {
                if (dataTmp[i][j] == 0) {
                    mapArray[i][j].visibility = true;
                    mapArray[i][j].passable   = true;
                    mapArray[i][j].textureId = 0;
                }else{
                    mapArray[i][j].visibility = false;
                    mapArray[i][j].passable   = false;
                    mapArray[i][j].textureId = dataTmp[i][j];
                }
            }
        }
    } else {
        mapArray = data["cells"];
    }
    PlayerInitialPosition  = data["playerStart"];
    PlayerInitialDirection = data["playerStartDir"];
    updateSize();
}

nlohmann::json Map::toJson()const{
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

}// namespace rc::core
