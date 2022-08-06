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
}

void Engine::display() {
    if (!player || !map)
        return;
    drawMap();
    drawRayCasting();
    drawPlayerOnMap();
}

void Engine::button(uint8_t key, [[maybe_unused]] int32_t x, [[maybe_unused]] int32_t y) {
    if (player == nullptr)
        return;
    if (key == 'q') {
        player->rotate({-5, math::Angle::Unit::Degree});
    }
    if (key == 'd') {
        player->rotate({5, math::Angle::Unit::Degree});
    }
    if (key == 'z') {
        player->walk(5);
    }
    if (key == 's') {
        player->walk(-5);
    }
    if (renderer == nullptr)
        return;
    renderer->update();
}

void Engine::run() {
    if (renderer == nullptr)
        return;
    renderer->run();
}

void Engine::registerPlayer(const std::shared_ptr<Player>& player_) {
    if (renderer == nullptr)
        return;
    if (renderer->getStatus() == renderer::Status::Running)
        return;
    player = player_;
}

void Engine::registerMap(const std::shared_ptr<Map>& map_) {
    if (renderer == nullptr)
        return;
    if (renderer->getStatus() == renderer::Status::Running)
        return;
    map = map_;
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
                       {0, 255, 255});
    renderer->drawQuad({{static_cast<double>(settings.layout3D[0][0]), static_cast<double>(settings.layout3D.center()[1])},
                        {static_cast<double>(settings.layout3D[1][0]), static_cast<double>(settings.layout3D.center()[1])},
                        {static_cast<double>(settings.layout3D[1][0]), static_cast<double>(settings.layout3D[1][1])},
                        {static_cast<double>(settings.layout3D[0][0]), static_cast<double>(settings.layout3D[1][1])}},
                       {0, 0, 255});
    // ray casting
    for (int32_t rays = 0; rays <= settings.layout3D.width(); ++rays) {
        auto result = map->castRay(player->getPosition(), ray);
        graphics::Color color{0, 153, 0};
        if (result.hitVertical)
            color = {0, 204, 0};
        if (settings.drawRays) {
            renderer->drawLine({player->getPosition() * scaleFactor + offsetPoint, ray, result.distance * scaleFactor}, 2, color);//draw 2D ray
        }
        auto lineH = static_cast<int32_t>((map->getCellSize() * settings.layout3D.height()) /
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
                         math::Vector2<double>{0, offset - 1},
                         math::Vector2<double>{offset - 1, offset - 1},
                         math::Vector2<double>{offset - 1, 0}};
    quad.move(offsetPoint);
    for (Map::LineType& line : map->getMapData()) {
        for (uint8_t cell : line) {
            renderer->drawQuad(quad,
                               cell > 0 ? graphics::Color{255, 255, 255} : graphics::Color{0, 0, 0});
            quad.move({offset, 0});
        }
        offsetPoint += {0, offset};
        quad.moveTo(offsetPoint);
    }
}
void Engine::drawPlayerOnMap() {
    auto [scaleFactor, offsetPoint] = getMapLayoutInfo();
    renderer->drawPoint(player->getPosition() * scaleFactor + offsetPoint, 8 * scaleFactor, {255U, 255U, 0U});
    renderer->drawLine({player->getPosition() * scaleFactor + offsetPoint, player->getDirection() * 20.0 * scaleFactor, 1}, 4, {255U, 255U, 0U});
}

std::tuple<double, math::Vector2<double>> Engine::getMapLayoutInfo() const {
    double scaleFactor = std::min(settings.layoutMap.width() / static_cast<double>(map->fullWidth()),
                                  settings.layoutMap.height() / static_cast<double>(map->fullHeight()));
    return std::tuple<double, math::Vector2<double>>(scaleFactor,
                                                     {settings.layoutMap.center()[0] - map->fullWidth() * scaleFactor / 2.0,
                                                      settings.layoutMap.center()[1] - map->fullHeight() * scaleFactor / 2.0});
}

}// namespace rc::core