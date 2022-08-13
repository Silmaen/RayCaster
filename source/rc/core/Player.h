/**
 * @file Player.h
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once

#include "math/Vector2.h"

namespace rc::core {

/**
 * @brief Class Player
 */
class Player {
public:
    /// Player position's type
    using PositionType = math::Vectf;
    /// Player direction's type
    using DirectionType = math::Vectf;
    /**
     * @brief Default copy constructor
     */
    Player(const Player&)= default;
    /**
     * @brief Default move constructor
     */
    Player(Player&&)= default;
    /**
     * @brief Default copy assignation
     * @return this
     */
    Player& operator=(const Player&)= default;
    /**
     * @brief Default move assignation
     * @return this
     */
    Player& operator=(Player&&)= default;
    /**
     * @brief Default constructor.
     */
    Player()= default;
    /**
     * @brief Destructor.
     */
    ~Player()= default;//---UNCOVER---

    /**
     * @brief Access to the player's position.
     * @return Player's position.
     */
    [[nodiscard]] const PositionType& getPosition()const{return position;}
    /**
     * @brief Define the new position
     * @param pos Position
     */
    void setPosition(const PositionType& pos){position = pos;}
    /**
     * @brief Access to player's direction.
     * @return Player's direction.
     */
    [[nodiscard]] const DirectionType& getDirection()const{return direction;}
    /**
     * @brief Define the direction
     * @param dir The Direction
     */
    void setDirection(const DirectionType& dir);

    /**
     * @brief Rotate player
     * @param angle Amount of rotation
     */
    void rotate(const math::Angle& angle);

    /**
     * @brief Move player in the look at direction
     * @param step Amount of movement
     */
    void move(const DirectionType& step);
private:
    /// Player's position
    PositionType position{};
    /// Player's direction
    DirectionType direction{1.,0.};
};

}// namespace rc::base
