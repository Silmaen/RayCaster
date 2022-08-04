/**
 * @file Engine.h
 * @author Silmaen
 * @date 01/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */
#pragma once

#include "Player.h"
#include "graphics/Line2.h"
#include "graphics/Quad2.h"
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
     * @brief Access the screen resolution
     * @return Screen resolution
     */
    const ResolutionType& getResolution()const{return resolution;}

    /**
     * @brief Initialize game engine
     * @param argc Number of argument
     * @param argv Argument’s list
     */
    void init(int argc, char* argv[]);

    /**
     * @brief Main loop
     */
    void run();

    /**
     * @brief Append a function to the rendering process
     * @param func Function to add
     */
    void registerRender(const std::function<void()>& func);

    /**
     * @brief Register the player for this game
     * @param player The player
     */
    void registerPlayer(const std::shared_ptr<Player>& player);

    // Drawing functions
    /**
     * @brief Draw a point
     * @param location Where to draw in screen coordinate
     * @param size Size of the point
     * @param color Color of the point
     */
    void drawPoint(const math::Vector2<double>& location, double size, const graphics::Color& color) const;
    /**
     * @brief Draw a line
     * @param line Line's data
     * @param width Width of the line
     * @param color Line's color
     */
    void drawLine(const graphics::Line2<double>& line, double width, const graphics::Color& color) const;
    /**
     * @brief Draw a quad
     * @param quad Quad's data
     * @param color Quad's color
     */
    void drawQuad(const graphics::Quad2<double>& quad,  const graphics::Color& color) const;

#ifndef INTERNAL
private:
#endif
    /**
     * @brief Glut display call back function
     */
    void display();
    /**
     * @brief Glut input callback function
     * @param key Input key
     * @param x X placement
     * @param y Y placement
     */
    void button(uint8_t key, int32_t x, int32_t y);
#ifdef INTERNAL
private:
#endif
    /**
     * @brief Default constructor.
     */
    Engine()= default;
    /// If already initialized
    bool initialized = false;
    /// If engine is running
    bool running = false;
    /// Screen resolution
    ResolutionType resolution{1024,512};
    /// Link to the player
    std::shared_ptr<Player> player = nullptr;

    std::vector<std::function<void()>> toRender;

    static void setColor(const graphics::Color& color);
    static void pushVertex(const math::Vector2<double>& vertex);
};

}// namespace rc::base
