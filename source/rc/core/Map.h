/**
 * @file Map.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "graphics/Color.h"
#include "math/Vector2.h"
#include <string>
#include <tuple>
#include <vector>

namespace rc::core {


/**
 * @brief Structure Holding cell data
 */
struct mapCell {
    bool passable;    ///< if player can pass through
    bool visibility;  ///< if player can see through
    uint8_t textureId;///< wall texture ID (color)
    /**
     * @brief Get the associated color for map and 3D view
     * @return the color
     */
    const graphics::Color& getMapColor()const;
    /**
     * @brief Get the associated color for ray on map
     * @return the color
     */
    const graphics::Color& getRayColor()const;
    /**
     * @brief Comparison operator
     * @return True if equal
     */
    [[nodiscard]] bool operator==(const mapCell& )const =default;
    /**
     * @brief Comparison operator
     * @return True if not equal
     */
    [[nodiscard]] bool operator!=(const mapCell& )const =default;
    /// if the player has seen this wall
    bool isViewed = false;
};

/**
 * @brief Serialize this objet to json
 * @param jso The json output
 * @param mCell The mapCell to serialize
 */
inline void to_json(nlohmann::json& jso, const mapCell& mCell){
    uint8_t result = (mCell.passable * 0b10000000) | (mCell.visibility * 0b01000000) | (mCell.textureId & 0b00111111);
    jso = nlohmann::json{result};
}
/**
 * @brief Deserialize this object from json
 * @param jso Json source
 * @param mCell Destination mapCell
 */
inline void from_json(const nlohmann::json& jso, mapCell& mCell){
    uint8_t result =  jso.at(0);
    mCell.textureId = result & 0b00111111;
    mCell.visibility = (result & 0b01000000) == 0b01000000;
    mCell.passable = (result & 0b10000000) == 0b10000000;
}

/**
 * @brief Class Map
 */
class Map {
public:
    /// Base type map tile data
    using BaseType = mapCell;
    /// Map line's type
    using LineType = std::vector<BaseType>;
    /// Map data's type
    using DataType = std::vector<LineType>;
    /// Index's type in map data
    using IndexType = uint8_t;
    /// Grid coordinate's type
    using gridCoordinate = math::Vector2<IndexType>;

    /**
     * @brief Default constructor.
     */
    Map() = default;
    /**
     * @brief Default copy constructor
     */
    Map(const Map&) = default;
    /**
     * @brief Default move constructor
     */
    Map(Map&&) = default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Map& operator=(const Map&) = default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Map& operator=(Map&&) = default;
    /**
     * @brief Destructor.
     */
    ~Map() = default;

    /**
     * @brief Construct map by size
     * @param w width
     * @param h height
     * @param cube Cube size
     */
    Map(IndexType w, IndexType h, uint8_t cube = 64) :
        cubeSize{cube} { reset(w, h); }
    /**
     * @brief Constructor by Data
     * @param data The raw data to copy
     * @param cube The cube size
     */
    Map(const DataType& data, uint8_t cube = 64);

    /**
     * @brief Constructor by Data
     * @param data The raw data to copy
     * @param cube The cube size
     */
    Map(DataType&& data, uint8_t cube = 64);

    /**
     * @brief Set the map dat
     * @param data The map data
     *
     * Will update th width and height according to the data
     */
    void setMap(const DataType& data);
    /**
     * @brief Set the map dat
     * @param data The map data
     *
     * Will update th width and height according to the data
     */
    void setMap(DataType&& data);

    /**
     * @brief Get the raw map data
     * @return Map data
     */
    DataType& getMapData() { return mapArray; }

    /**
     * @brief Access to map value at the coordinate
     * @param location coordinates
     * @return The local map data
     */
    BaseType& operator()(const gridCoordinate& location) { return at(location); }
    /**
     * @brief Access to map value at the coordinate
     * @param location coordinates
     * @return The local map data
     */
    const BaseType& operator()(const gridCoordinate& location) const { return at(location); }

    /**
     * @brief Access to map value at the coordinate
     * @param location coordinates
     * @return The local map data
     */
    BaseType& at(const gridCoordinate& location);
    /**
     * @brief Access to map value at the coordinate
     * @param location coordinates
     * @return The local map data
     */
    [[nodiscard]] const BaseType& at(const gridCoordinate& location) const;

    /**
     * @brief Get the map's width
     * @return Map's width
     */
    [[nodiscard]] size_t width() const { return mapArray.size(); }
    /**
     * @brief Get the map's height
     * @return Map's height
     */
    [[nodiscard]] size_t height() const { return mapArray.empty() ? 0 : mapArray.front().size(); }
    /**
     * @brief Check the map validity
     * @return True if map valid.
     */
    [[nodiscard]] bool isValid() const;

    /**
     * @brief Ray casting result
     */
    struct rayCastResult {
        double distance;                ///< Distance of the hit
        math::Vector2<double> wallPoint;///< Point on the wall
        bool hitVertical;               ///< If we hit a vertical wall
    };
    /**
     * @brief Cast a ray in the 2D space
     * @param from Starting point
     * @param direction Ray's direction
     * @return Hit data
     */
    [[nodiscard]] rayCastResult castRay(const math::Vector2<double>& from, const math::Vector2<double>& direction) const;

    /**
     * @brief Determine the cell where the point lies.
     * @param from The point to check
     * @return The cell
     */
    [[nodiscard]] gridCoordinate whichCell(const math::Vector2<double>& from) const;
    /**
     * @brief Check if a point is in the map
     * @param from The point to check
     * @return True if in the map
     */
    [[nodiscard]] bool isIn(const math::Vector2<double>& from) const;
    /**
     * @brief Checks if grid coordinates are in the map
     * @param from Grid coordinates to check
     * @return True if in the map
     */
    [[nodiscard]] bool isIn(const gridCoordinate& from) const;

    /**
     * @brief Check if a point is in the map and player can pass through
     * @param from The point to check
     * @return True if in the map
     */
    [[nodiscard]] bool isInPassable(const math::Vector2<double>& from) const;
    /**
     * @brief Checks if grid coordinates are in the map and player can pass through
     * @param from Grid coordinates to check
     * @return True if in the map
     */
    [[nodiscard]] bool isInPassable(const gridCoordinate& from) const;

    /**
     * @brief Check if a point is in the map and player can see through
     * @param from The point to check
     * @return True if in the map
     */
    [[nodiscard]] bool isInVisible(const math::Vector2<double>& from) const;
    /**
     * @brief Checks if grid coordinates are in the map and player can see through
     * @param from Grid coordinates to check
     * @return True if in the map
     */
    [[nodiscard]] bool isInVisible(const gridCoordinate& from) const;

    /**
     * @brief Get the cube's size
     * @return Cube's size
     */
    [[nodiscard]] uint8_t getCellSize() const { return cubeSize; }
    /**
     * @brief Get the full pixel width of the map
     * @return Pixel width of the map
     */
    [[nodiscard]] uint32_t fullWidth() const { return width() * cubeSize; }
    /**
     * @brief Get the full pixel height of the map
     * @return Pixel height of the map
     */
    [[nodiscard]] uint32_t fullHeight() const { return height() * cubeSize; }
    /**
     * @brief Define player starts
     * @param pos Position
     * @param dir Direction
     */
    void setPlayerStart(const math::Vectf& pos, const math::Vectf& dir) {
        PlayerInitialPosition  = pos;
        PlayerInitialDirection = dir;
    }
    /**
     * @brief Get player start information
     * @return Player start state
     */
    [[nodiscard]] std::tuple<const math::Vectf&, const math::Vectf&> getPlayerStart() const { return {PlayerInitialPosition, PlayerInitialDirection}; }

    /**
     * @brief load a map
     * @param mapName Map's name
     */
    void loadFromFile(const std::string& mapName);
    /**
     * @brief save a map
     * @param mapName Map's name
     */
    void saveToFile(const std::string& mapName);

private:
    /**
     * @brief Reset the map to the given size
     * @param w width
     * @param h height
     */
    void reset(uint8_t w, uint8_t h);
    /**
     * @brief Update the size
     */
    void updateSize();
    /// Size of a cube
    uint8_t cubeSize = 64;
    /// Player stating point in the map
    math::Vectf PlayerInitialPosition{};
    /// Player Starting direction in the map
    math::Vectf PlayerInitialDirection{};
    /// The map data
    DataType mapArray;
    double maxWidth  = 0;
    double maxHeight = 0;
};

}// namespace rc::core
