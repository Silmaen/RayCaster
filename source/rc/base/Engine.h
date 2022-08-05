/**
 * @file Engine.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "Map.h"
#include "Player.h"
#include "graphics/Line2.h"
#include "graphics/Quad2.h"
#include "renderer/BaseRenderer.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <vector>
/**
 * @namespace rc
 * @brief Base namespace of the project library
 */

/**
 * @brief Namespace for base objects
 */
namespace rc::base {

/**
 * @brief Engine setings
 */
struct EngineSettings {
    /// Type of renderer to use
    renderer::RendererType rendererType;
    /// Renderer Settings
    renderer::Settings rendererSettings;
};

/**
 * @brief Class Engine
 */
class Engine {
public:
    /// Type for screen resolution
    using ResolutionType=math::Vector2<int32_t>;

    Engine(const Engine&)= delete;
    Engine(Engine&&)= delete;
    Engine& operator=(const Engine&)= delete;
    Engine& operator=(Engine&&)= delete;
    /**
     * @brief Destructor.
     */
    ~Engine()= default;
    /**
     * @brief Get engine instance
     * @return The engine instance
     */
    static Engine& get(){
        static Engine instance;
        return instance;
    }

    /**
     * @brief Access the settings
     * @return Settings
     */
    [[nodiscard]] const EngineSettings& getSettings()const{return settings;}


    /**
     * @brief Define the settings
     * @param setting The Settings
     */
    void setSettings(const EngineSettings& setting);

    /**
     * @brief Initialize game engine
     */
    void init();

    /**
     * @brief Main loop
     */
    void run();

    /**
     * @brief Register the player for this game
     * @param player The player
     */
    void registerPlayer(const std::shared_ptr<Player>& player);
    /**
     * @brief Register the player for this game
     * @param map The current Map
     */
    void registerMap(const std::shared_ptr<Map>& map);

    /**
     * @brief Access yto the Engine renderer
     * @return The renderer
     */
    std::shared_ptr<renderer::BaseRenderer> getRenderer();
    /**
     * @brief Input callback function
     * @param key Input key
     * @param x X placement
     * @param y Y placement
     */
    void button(uint8_t key, int32_t x, int32_t y);
    /**
     * @brief Glut display call back function
     */
    void display();
private:

    void drawRayCasting();

    /**
     * @brief Default constructor.
     */
    Engine()= default;
    /// Engine's settings
    EngineSettings settings{renderer::RendererType::OpenGL,{{1024,512}}};
    /// Link to the renderer
    std::shared_ptr<renderer::BaseRenderer> renderer = nullptr;
    /// Link to the map
    std::shared_ptr<Map> map = nullptr;
    /// Link to the player
    std::shared_ptr<Player> player = nullptr;

    std::vector<std::function<void()>> toRender;
};

}// namespace rc::base
