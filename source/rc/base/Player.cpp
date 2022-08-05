/**
 * @file Player.cpp
 * @author Silmaen
 * @date 02/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Player.h"
#include "base/Engine.h"

namespace rc::base {

void Player::draw() {
    auto engine = Engine::get().getRenderer();
    engine->drawPoint(position, 8, {255U, 255U, 0U});
    engine->drawLine({position, direction * 20.0, 1}, 4, {255U, 255U, 0U});
}

void Player::rotate(const math::Angle& angle) {
    direction.rotate(angle);
}

void Player::walk(double step) {
    position += direction * step;
}

void Player::setDirection(const Player::DirectionType& dir) {
    direction = dir;
    if (std::abs(dir.lengthSQ() - 1.0) > 0.0001){
        direction /= direction.length();
    }
}

}// namespace rc::base
