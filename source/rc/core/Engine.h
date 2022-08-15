/**
 * @file Engine.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "game/Map.h"
#include "game/Player.h"
#include "input/BaseInput.h"
#include "math/geometry/Box2.h"
#include "math/geometry/Line2.h"
#include "math/geometry/Quad2.h"
#include "graphics/renderer/BaseRenderer.h"
#include <chrono>
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
 * @brief Engine settings
 */
struct EngineSettings {
    /// Renderer's type to use
    graphics::renderer::RendererType rendererType = graphics::renderer::RendererType::Null;
    /// Renderer Settings
    graphics::renderer::Settings rendererSettings{{1280, 720}};
    /// Input type
    input::InputType inputType = input::InputType::GL;
    /// Input type
    input::Settings inputSettings{};
    /// Screen zone where to draw the 3D scene
    math::geometry::Box2 layout3D{{0, 0}, {860, 550}};
    /// Screen zone where to draw the map
    math::geometry::Box2 layoutMap{{880, 150}, {1280, 550}};
    /// If daw the rays in the map
    bool drawTexture = true;
    /// If daw the rays in the map
    bool drawMap = false;
    /// If daw the rays in the map
    bool drawRays = false;
    /**
     * @brief Set from json
     * @param data The input json
     */
    void fromJson(const nlohmann::json& data);
    /**
     * @brief Write to json
     * @return The resulting json
     */
    [[nodiscard]] nlohmann::json toJson() const;
};

/**
 * @brief Class Engine
 */
class Engine {
public:
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
        Running,      ///< running
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
    std::shared_ptr<graphics::renderer::BaseRenderer> getRenderer();
    /**
     * @brief Input callback function
     */
    void button();
    /**
     * @brief Glut display call back function
     */
    void display();

    /**
     * @brief Load the map
     * @param mapName Name of the map to load
     */
    void mapLoad(const std::string& mapName);

    /**
     * @brief Load setings from file
     * @param filename The file to load
     */
    void loadSettings(const std::string& filename);
    /**
     * @brief Save setting to file
     * @param filename The file to save
     */
    void saveSettings(const std::string& filename);
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
    [[nodiscard]] std::tuple<double, math::geometry::Vectf> getMapLayoutInfo() const;
    /**
     * @brief Default constructor.
     */
    Engine();
    /// Engine's settings
    EngineSettings settings{
            graphics::renderer::RendererType::OpenGL};
    /// Current status of the engine
    Status status = Status::Uninitialized;
    /// Link to the renderer
    std::shared_ptr<graphics::renderer::BaseRenderer> renderer = nullptr;
    /// Link to the input
    std::shared_ptr<input::BaseInput> input = nullptr;
    /// Link to the map
    std::shared_ptr<game::Map> map = nullptr;
    /// Link to the player
    std::shared_ptr<game::Player> player = nullptr;

    std::vector<std::function<void()>> toRender;

    /// Clock type
    using engineClock = std::chrono::steady_clock;
    /// Record last frame
    engineClock::time_point frames;
    uint32_t deltaMillis = 0;
    /// Record last freeze call
    engineClock::time_point freeze;
    /// frames per second
    double fps = 0;
};

}// namespace rc::core
