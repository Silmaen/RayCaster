/**
 * @file Engine.cpp
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "Engine.h"
#include "core/fs/DataFile.h"
#include "graphics/image/TextureManager.h"
#include "graphics/renderer/NullRenderer.h"
#include "graphics/renderer/OpenGlRenderer.h"
#include "input/GlInput.h"

namespace rc::core {

void EngineSettings::fromJson(const nlohmann::json& data) {
    if (data.contains("rendererType"))
        rendererType = data["rendererType"];
    if (data.contains("rendererSettings"))
        rendererSettings.fromJson(data["rendererSettings"]);
    if (data.contains("inputType"))
        inputType = data["inputType"];
    if (data.contains("inputSettings"))
        inputSettings.fromJson(data["inputSettings"]);
    if (data.contains("layout3D"))
        layout3D = data["layout3D"];
    if (data.contains("layoutMap"))
        layoutMap = data["layoutMap"];
    if (data.contains("drawTexture"))
        drawTexture = data["drawTexture"];
    if (data.contains("drawMap"))
        drawMap = data["drawMap"];
    if (data.contains("drawRays"))
        drawRays = data["drawRays"];
}

nlohmann::json EngineSettings::toJson() const {
    nlohmann::json data;
    data["rendererType"]     = rendererType;
    data["rendererSettings"] = rendererSettings.toJson();
    data["inputType"]        = inputType;
    data["inputSettings"]    = inputSettings.toJson();
    data["layout3D"]         = layout3D;
    data["layoutMap"]        = layoutMap;
    data["drawTexture"]      = drawTexture;
    data["drawMap"]          = drawMap;
    data["drawRays"]         = drawRays;
    return data;
}

Engine::Engine() {
    // load setting at creation
    loadSettings("settings.json");
}

void Engine::setSettings(const EngineSettings& setting) {
    settings = setting;
    if (renderer != nullptr)
        init();
}

void Engine::init() {
    switch (settings.rendererType) {
    case graphics::renderer::RendererType::OpenGL:
        renderer = std::make_shared<graphics::renderer::OpenGLRenderer>();
        break;
    case graphics::renderer::RendererType::Null:
        renderer = std::make_shared<graphics::renderer::NullRenderer>();
        break;
    case graphics::renderer::RendererType::Unknown:
        renderer = nullptr;
        return;
    }
    switch (settings.inputType) {
    case input::InputType::GL:
        input = std::make_shared<input::GLInput>();
        break;
    case input::InputType::Unknown:
        input = nullptr;
        return;
    }
    renderer->settings() = settings.rendererSettings;
    renderer->Init();
    renderer->setDrawingCallback([this] { this->display(); });
    input->settings() = settings.inputSettings;
    input->Init();
    input->setButtonCallback([this]() { renderer->update(); });
    map    = std::make_shared<game::Map>();
    player = std::make_shared<game::Player>();

    status = Status::Ready;
    frames = engineClock::now();
}

void Engine::display() {
    if (status != Status::Running)
        return;
    engineClock::time_point temp = engineClock ::now();
    deltaMillis                  = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(temp - frames).count());
    fps                          = 1000.0 / deltaMillis;
    frames                       = temp;
    button();
    renderer->update();
    if (settings.drawMap)
        drawMap();
    drawRayCasting();
    if (settings.drawMap)
        drawPlayerOnMap();
    // draw fps.
    std::stringstream text;
    text << "fps " << fps;
    renderer->drawText(text.str(), {875, 50}, {200U, 20U, 0U});
}

void Engine::button() {
    if (input->isKeyPressed(input::FunctionKey::TurnLeft)) {
        player->rotate({-0.2 * deltaMillis, math::geometry::Angle::Unit::Degree});
    }
    if (input->isKeyPressed(input::FunctionKey::TurnRight)) {
        player->rotate({0.2 * deltaMillis, math::geometry::Angle::Unit::Degree});
    }
    if (input->isKeyPressed(input::FunctionKey::Forward)) {
        player->move(map->possibleMove(player->getPosition(), player->getDirection() * 0.2 * deltaMillis));
    }
    if (input->isKeyPressed(input::FunctionKey::Backward)) {
        player->move(map->possibleMove(player->getPosition(), player->getDirection() * -0.2 * deltaMillis));
    }
    if (input->isKeyPressed(input::FunctionKey::StrafeLeft)) {
        player->move(map->possibleMove(player->getPosition(), player->getDirection().rotated90() * 0.2 * deltaMillis));
    }
    if (input->isKeyPressed(input::FunctionKey::StrafeRight)) {
        player->move(map->possibleMove(player->getPosition(), player->getDirection().rotated90() * -0.2 * deltaMillis));
    }

    // toggle button: freeze time
    auto freezeTime = std::chrono::duration_cast<std::chrono::milliseconds>(frames - freeze).count();
    if (freezeTime < 400)
        return;
    if (input->isKeyPressed(input::FunctionKey::Use)) {
        // use action
        freeze = frames;
    }
    if (input->isKeyPressed(input::FunctionKey::DisplayTexture)) {
        settings.drawTexture = !settings.drawTexture;
        freeze           = frames;
    }
    if (input->isKeyPressed(input::FunctionKey::DisplayMap)) {
        settings.drawMap = !settings.drawMap;
        freeze           = frames;
    }
    if (input->isKeyPressed(input::FunctionKey::DisplayRayOnMap)) {
        settings.drawRays = !settings.drawRays;
        freeze            = frames;
    }
    if (input->isKeyPressed(input::FunctionKey::Exit)) {
        // exit action
        freeze = frames;
        exit(0);
    }
}

void Engine::run() {
    checkState();
    if (status == Status::Ready) {
        status = Status::Running;
        renderer->run();
    }
}

std::shared_ptr<graphics::renderer::BaseRenderer> Engine::getRenderer() {
    if (renderer == nullptr)
        return std::make_shared<graphics::renderer::NullRenderer>();
    return renderer;
}

void Engine::drawRayCasting() {
    using Unit                      = rc::math::geometry::Angle::Unit;
    double fov                      = 60.0;
    const double increment          = fov / settings.layout3D.width();
    auto ray                        = player->getDirection().rotated(rc::math::geometry::Angle{-fov / 2, Unit::Degree});
    auto [scaleFactor, offsetPoint] = getMapLayoutInfo();
    auto& texMng = graphics::image::TextureManager::get();
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

        game::Map::BaseType& cell = map->at(map->whichCell(result.wallPoint));
        if (cell.getTextureName() == "")
            continue;
        if (cell.textureId>6)
            continue;
        graphics::Color color{cell.getRayColor()};
        cell.isViewed = true;
        if (settings.drawRays && settings.drawMap) {// draw the ray on map
            if (result.hitVertical)
                color.darken();
            renderer->drawLine({player->getPosition() * scaleFactor + offsetPoint, ray, result.distance * scaleFactor}, 2, color);//draw 2D ray
        }
        auto lineH = static_cast<int32_t>((map->getCellSize() * 1.2 * settings.layout3D.height()) /
                                          (result.distance * player->getDirection().dot(ray)));
        //if (lineH > settings.layout3D.height()) { lineH = settings.layout3D.height(); }//line height and limit
        double lineOff = settings.layout3D.center()[1] - (lineH >> 1);                 //line offset
        //draw vertical wall
        double lineX = rays + settings.layout3D[0][0];
        if( settings.drawTexture) {
            const auto& tex   = texMng.getTexture(cell.getTextureName());
            double texX = tex.width() * result.hitXRatio / map->getCellSize();
            renderer->drawTextureVerticalLine(lineX, lineOff, lineH, tex, texX, settings.layout3D, result.hitVertical);
        }else
            renderer->drawLine({{lineX, lineOff}, {lineX, lineOff + lineH}}, 1, color);
        ray.rotate(rc::math::geometry::Angle{increment, Unit::Degree});//go to next ray
    }
}

void Engine::drawMap() {
    auto [scaleFactor, offsetPoint] = getMapLayoutInfo();
    double offset                   = map->getCellSize() * scaleFactor;
    math::geometry::Quad2<double> quad{math::geometry::Vectf{0, 0},
                                       math::geometry::Vectf{0, offset},
                                       math::geometry::Vectf{offset, offset},
                                       math::geometry::Vectf{offset, 0}};
    quad.move(offsetPoint);
    for (game::Map::LineType& line : map->getMapData()) {
        for (game::Map::BaseType cell : line) {
            if (cell.isViewed)
                renderer->drawQuad(quad,
                                   cell.passable ? graphics::Color{0, 0, 0} : cell.visibility ? graphics::Color{40, 40, 40} :
                                                                                                cell.getMapColor());
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

std::tuple<double, math::geometry::Vectf> Engine::getMapLayoutInfo() const {
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
    map->loadFromData(mapName);
    auto [pos, dir] = map->getPlayerStart();
    player->setPosition(pos);
    player->setDirection(dir);
}

void Engine::loadSettings(const std::string& filename) {
    // load setting at creation
    fs::DataFile settingsFile(filename);
    if (settingsFile.exists()) {
        settings.fromJson(settingsFile.readJson());
    }
}

void Engine::saveSettings(const std::string& filename) {
    // save current settings
    fs::DataFile settingsFile(filename);
    settingsFile.writeJson(settings.toJson());
}

}// namespace rc::core
