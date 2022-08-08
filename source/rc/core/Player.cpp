/**
 * @file Player.cpp
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Player.h"
#include "core/Engine.h"

namespace rc::core {

void Player::rotate(const math::Angle& angle) {
    direction.rotate(angle);
}

void Player::move(const DirectionType& step) {
    position += step;
}

void Player::setDirection(const Player::DirectionType& dir) {
    direction = dir;
    if (std::abs(dir.lengthSQ() - 1.0) > 0.0001){
        direction /= direction.length();
    }
}

}// namespace rc::base
