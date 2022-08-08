/**
 * @file Engine.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Engine.h"
#include "renderer/NullRenderer.h"
#include "renderer/OpenGlRenderer.h"

namespace rc::core {

void Engine::setSettings(const EngineSettings& setting) {
    settings = setting;
    if (renderer != nullptr)
        init();
}

void Engine::init() {
    switch (settings.rendererType) {
    case renderer::RendererType::OpenGL:
        renderer = std::make_shared<renderer::OpenGLRenderer>();
        break;
    case renderer::RendererType::Null:
        renderer = std::make_shared<renderer::NullRenderer>();
        break;
    case renderer::RendererType::Unknown:
        renderer = nullptr;
        return;
    }
    renderer->settings() = settings.rendererSettings;
    renderer->Init();
    renderer->setDrawingCallback([this] { this->display(); });
    renderer->setButtonCallback([this](auto&& PH1, auto&& PH2, auto&& PH3) { button(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), std::forward<decltype(PH3)>(PH3)); });
    map    = std::make_shared<Map>();
    player = std::make_shared<Player>();
    status = Status::Ready;
}

void Engine::display() {
    if (!player || !map)
        return;
    if (settings.drawMap)
        drawMap();
    drawRayCasting();
    if (settings.drawMap)
        drawPlayerOnMap();
}

void Engine::button(uint8_t key, [[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y) {
    if (status == Status::Uninitialized)
        return;
    if (key == 'q') {
        player->rotate({-5, math::Angle::Unit::Degree});
    }
    if (key == 'd') {
        player->rotate({5, math::Angle::Unit::Degree});
    }
    if (key == 'z') {
        Player::DirectionType expectedMove = player->getDirection() * 5;
        Player::PositionType newPos        = player->getPosition() + expectedMove;
        if (map->isInPassable(newPos)) {
            player->move(expectedMove);
        } else {
            newPos = player->getPosition() + Player::PositionType{expectedMove[0], 0.0};
            if (map->isInPassable(newPos)) {
                player->move({expectedMove[0], 0.0});
            } else {
                newPos = player->getPosition() + Player::PositionType{0.0, expectedMove[1]};
                if (map->isInPassable(newPos))
                    player->move({0.0, expectedMove[1]});
            }
        }
    }
    if (key == 's') {
        Player::DirectionType expectedMove = player->getDirection() * -5;
        Player::PositionType newPos        = player->getPosition() + expectedMove;
        if (map->isInPassable(newPos)) {
            player->move(expectedMove);
        } else {
            newPos = player->getPosition() + Player::PositionType{expectedMove[0], 0.0};
            if (map->isInPassable(newPos)) {
                player->move({expectedMove[0], 0.0});
            } else {
                newPos = player->getPosition() + Player::PositionType{0.0, expectedMove[1]};
                if (map->isInPassable(newPos))
                    player->move({0.0, expectedMove[1]});
            }
        }
    }
    if (key == 'e') {
        // use action
    }
    if (key == 'm') {
        settings.drawMap = !settings.drawMap;
    }
    if (key == 'r') {
        settings.drawRays = !settings.drawRays;
    }
    renderer->update();
}

void Engine::run() {
    checkState();
    if (status == Status::Ready) {
        renderer->run();
    }
}

std::shared_ptr<renderer::BaseRenderer> Engine::getRenderer() {
    if (renderer == nullptr)
        return std::make_shared<renderer::NullRenderer>();
    return renderer;
}

void Engine::drawRayCasting() {
    using Unit                      = rc::math::Angle::Unit;
    double fov                      = 60.0;
    const double increment          = fov / settings.layout3D.width();
    auto ray                        = player->getDirection().rotated(rc::math::Angle{-fov / 2, Unit::Degree});
    auto [scaleFactor, offsetPoint] = getMapLayoutInfo();
    // Sky and floor
    renderer->drawQuad({{static_cast<double>(settings.layout3D[0][0]), static_cast<double>(settings.layout3D[0][1])},
                        {static_cast<double>(settings.layout3D[1][0]), static_cast<double>(settings.layout3D[0][1])},
                        {static_cast<double>(settings.layout3D[1][0]), static_cast<double>(settings.layout3D.center()[1])},
                        {static_cast<double>(settings.layout3D[0][0]), static_cast<double>(settings.layout3D.center()[1])}},
                       {65, 65, 65});
    renderer->drawQuad({{static_cast<double>(settings.layout3D[0][0]), static_cast<double>(settings.layout3D.center()[1])},
                        {static_cast<double>(settings.layout3D[1][0]), static_cast<double>(settings.layout3D.center()[1])},
                        {static_cast<double>(settings.layout3D[1][0]), static_cast<double>(settings.layout3D[1][1])},
                        {static_cast<double>(settings.layout3D[0][0]), static_cast<double>(settings.layout3D[1][1])}},
                       {105, 105, 105});
    // ray casting
    for (int32_t rays = 0; rays <= settings.layout3D.width(); ++rays) {
        auto result = map->castRay(player->getPosition(), ray);

        Map::BaseType& cell = map->at(map->whichCell(result.wallPoint));
        graphics::Color color{cell.getRayColor()};
        cell.isViewed = true;
        if (result.hitVertical)
            color.darken();
        if (settings.drawRays && settings.drawMap) {
            renderer->drawLine({player->getPosition() * scaleFactor + offsetPoint, ray, result.distance * scaleFactor}, 2, color);//draw 2D ray
        }
        auto lineH = static_cast<int32_t>((map->getCellSize() * 1.2 * settings.layout3D.height()) /
                                          (result.distance * player->getDirection().dot(ray)));
        if (lineH > settings.layout3D.height()) { lineH = settings.layout3D.height(); }//line height and limit
        double lineOff = settings.layout3D.center()[1] - (lineH >> 1);                 //line offset
        //draw vertical wall
        double lineX = rays + settings.layout3D[0][0];
        renderer->drawLine({{lineX, lineOff}, {lineX, lineOff + lineH}}, 1, color);
        ray.rotate(rc::math::Angle{increment, Unit::Degree});//go to next ray
    }
}

void Engine::drawMap() {
    auto [scaleFactor, offsetPoint] = getMapLayoutInfo();
    double offset                   = map->getCellSize() * scaleFactor;
    graphics::Quad2 quad{math::Vector2<double>{0, 0},
                         math::Vector2<double>{0, offset},
                         math::Vector2<double>{offset, offset},
                         math::Vector2<double>{offset, 0}};
    quad.move(offsetPoint);
    for (Map::LineType& line : map->getMapData()) {
        for (Map::BaseType cell : line) {
            if (cell.isViewed)
                renderer->drawQuad(quad,
                               cell.passable ? graphics::Color{0, 0, 0} : cell.visibility ? graphics::Color{40, 40, 40} : cell.getMapColor());
            quad.move({offset, 0});
        }
        offsetPoint += {0, offset};
        quad.moveTo(offsetPoint);
    }
}
void Engine::drawPlayerOnMap() {
    auto [scaleFactor, offsetPoint] = getMapLayoutInfo();
    renderer->drawPoint(player->getPosition() * scaleFactor + offsetPoint, 32 * scaleFactor, {255U, 255U, 0U});
    renderer->drawLine({player->getPosition() * scaleFactor + offsetPoint, player->getDirection() * 60.0 * scaleFactor, 1}, 8, {255U, 255U, 0U});
}

std::tuple<double, math::Vector2<double>> Engine::getMapLayoutInfo() const {
    double scaleFactor = std::min(settings.layoutMap.width() / static_cast<double>(map->fullWidth()),
                                  settings.layoutMap.height() / static_cast<double>(map->fullHeight()));
    return {scaleFactor,
            {settings.layoutMap.center()[0] - map->fullWidth() * scaleFactor / 2.0,
             settings.layoutMap.center()[1] - map->fullHeight() * scaleFactor / 2.0}};
}

void Engine::checkState() {
    if (status == Status::Uninitialized)
        return;
    if (renderer == nullptr) {
        status = Status::Error;
        return;
    }
    if (player == nullptr) {
        status = Status::Error;
        return;
    }
    if (map == nullptr) {
        status = Status::Error;
        return;
    }
    if (!map->isValid()) {
        status = Status::Error;
        return;
    }
    if (!map->isIn(player->getPosition())) {
        status = Status::Error;
        return;
    }
    status = Status::Ready;
}

void Engine::mapLoad(const std::string& mapName) {
    map->loadFromFile(mapName);
    auto [pos, dir] = map->getPlayerStart();
    player->setPosition(pos);
    player->setDirection(dir);
}

}// namespace rc::core
