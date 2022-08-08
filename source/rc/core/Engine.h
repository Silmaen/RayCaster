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
#include "graphics/Box2.h"
#include "graphics/Line2.h"
#include "graphics/Quad2.h"
#include "renderer/BaseRenderer.h"
#include <memory>
/**
 * @namespace rc
 * @brief Base namespace of the project library
 */

/**
 * @brief Namespace for base objects
 */
namespace rc::core {

/**
 * @brief Engine setings
 */
struct EngineSettings {
    /// Renderer's type to use
    renderer::RendererType rendererType = renderer::RendererType::Null;
    /// Renderer Settings
    renderer::Settings rendererSettings{{1280, 720}};
    /// Screen zone where to draw the 3D scene
    graphics::Box2 layout3D{{0, 0}, {860, 550}};
    /// Screen zone where to draw the map
    graphics::Box2 layoutMap{{880, 150}, {1280, 550}};
    /// If daw the rays in the map
    bool drawMap = false;
    /// If daw the rays in the map
    bool drawRays = false;
};

/**
 * @brief Class Engine
 */
class Engine {
public:
    /// Type for screen resolution
    using ResolutionType = math::Vector2<int32_t>;

    Engine(const Engine&)            = delete;
    Engine(Engine&&)                 = delete;
    Engine& operator=(const Engine&) = delete;
    Engine& operator=(Engine&&)      = delete;
    /**
     * @brief Destructor.
     */
    ~Engine() = default;
    /**
     * @brief Get engine instance
     * @return The engine instance
     */
    static Engine& get() {
        static Engine instance;
        return instance;
    }

    /**
     * @brief Access the settings
     * @return Settings
     */
    [[nodiscard]] const EngineSettings& getSettings() const { return settings; }

    /**
     * @brief Define the settings
     * @param setting The Settings
     */
    void setSettings(const EngineSettings& setting);

    /**
     * @brief Engine statuses
     */
    enum struct Status {
        Uninitialized,///< Uninitialized, just created
        Ready,        ///< Ready to run
        Error,        ///< In error state
    };

    /**
     * @brief Get the engine's status
     * @return The engine status
     */
    [[nodiscard]] const Status& getStatus()const{return status;}

    /**
     * @brief Initialize game engine
     */
    void init();

    /**
     * @brief Main loop
     */
    void run();

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

    /**
     * @brief Load the map
     * @param mapName Name of the map to load
     */
    void mapLoad(const std::string& mapName);
private:
    /**
     * @brief Function that draw the 3D environment
     */
    void drawRayCasting();

    /**
     * @brief Function that draw the map
     */
    void drawMap();
    /**
      * @brief Function that draw player's position in the map
      */
    void drawPlayerOnMap();

    /**
     * @brief Check the engine state and update status
     */
    void checkState();
    /**
      * @brief Compute map layout infos
      * @return Scale factor and offset point
      */
    [[nodiscard]] std::tuple<double, math::Vector2<double>> getMapLayoutInfo() const;
    /**
     * @brief Default constructor.
     */
    Engine() = default;
    /// Engine's settings
    EngineSettings settings{
            renderer::RendererType::OpenGL};
    /// Current status of the engine
    Status status = Status::Uninitialized;
    /// Link to the renderer
    std::shared_ptr<renderer::BaseRenderer> renderer = nullptr;
    /// Link to the map
    std::shared_ptr<Map> map = nullptr;
    /// Link to the player
    std::shared_ptr<Player> player = nullptr;

    std::vector<std::function<void()>> toRender;
};

}// namespace rc::core
