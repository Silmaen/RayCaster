/**
 * @file Engine.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Engine.h"
#include "base/renderer/NullRenderer.h"
#include "base/renderer/OpenGlRenderer.h"

namespace rc::base {

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
    renderer->setDrawingCallback([this]{this->display();});
    renderer->setButtonCallback([this](auto && PH1, auto && PH2, auto && PH3) { button(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2), std::forward<decltype(PH3)>(PH3)); });
}

void Engine::display() {
    if (!player || !map)
        return;
    map->draw();
    drawRayCasting();
    player->draw();
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
    if (renderer==nullptr)
        return;
    renderer->update();
}

void Engine::run() {
    if (renderer==nullptr)
        return;
    renderer->run();
}

void Engine::registerPlayer(const std::shared_ptr<Player>& player_) {
    if (renderer==nullptr)
        return;
    if (renderer->getStatus() == renderer::Status::Running)
        return;
    player = player_;
}

void Engine::registerMap(const std::shared_ptr<Map>& map_){
    if (renderer==nullptr)
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
    using Unit = rc::math::Angle::Unit;
    auto engine = rc::base::Engine::get().getRenderer();
    double fov = 60.0;
    double increment = 0.1;
    auto ray = player->getDirection().rotated(rc::math::Angle{-fov/2, Unit::Degree});
    rc::math::Vector2<double> rayTmp;
    engine->drawQuad({{526, 0}, {1006, 0}, {1006, 160}, {526, 160}}, {0, 255, 255});
    engine->drawQuad({{526, 160}, {1006, 160}, {1006, 320}, {526, 320}}, {0, 0, 255});
    for (double rays = 0; rays < fov/increment; ++rays) {
        auto result = map->castRay(player->getPosition(),ray);
        if (result.hitVertical) {//vertical hit first
            engine->drawLine({player->getPosition(), ray, result.distance}, 2, {0,204,0});//draw 2D ray
        }else{//horizontal hit first
            engine->drawLine({player->getPosition(), ray, result.distance}, 2, {0,153,0});//draw 2D ray
        }
        constexpr uint32_t mapS = 64;
        auto lineH = static_cast<int32_t>((mapS * 320) / (result.distance * player->getDirection().dot(ray)));
        if (lineH > 320) { lineH = 320; }//line height and limit
        double lineOff = 160 - (lineH >> 1);//line offset
        //draw vertical wall
        double lineX = (rays * 8 + 0.5)* increment + 526;
        if (result.hitVertical) {
            engine->drawLine({{lineX, lineOff}, {lineX, lineOff + lineH}}, 8.2*increment, {0,204,0});
        }else{
            engine->drawLine({{lineX, lineOff}, {lineX, lineOff + lineH}}, 8.2*increment, {0,153,0});
        }
        ray.rotate(rc::math::Angle{increment, Unit::Degree});//go to next ray
    }
}

}// namespace rc::base
