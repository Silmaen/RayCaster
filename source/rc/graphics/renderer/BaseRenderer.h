/**
 * @file BaseRenderer.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <functional>

#include "graphics/Color.h"
#include "math/geometry/Line2.h"
#include "math/geometry/Quad2.h"
#include "math/geometry/Vector2.h"

namespace rc::graphics::renderer {

/// Base type for screen resolution
using Resolution = math::geometry::Vector2<int32_t>;

/**
 * @brief The renderer statuses
 */
enum struct Status {
    Uninitialized,    ///< Un initialized
    Ready,            ///< Initialized and OK
    BadInitialization,///< Problem in initialization
    Running,          ///< Current running
};

/**
 * @brief Renderer errors
 */
enum struct Error {
    OK,///< No error
};

/**
* @brief Renderer setting
*/
struct Settings {
    /// Screen resolution
    Resolution ScreenResolution{1024, 512};
    /// Background color
    graphics::Color Background = graphics::Color::fromDouble(0.3, 0.3, 0.3);
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
 * @brief Renderer's type
 */
enum struct RendererType {
    Null,  ///< Null renderer
    OpenGL,///< OpenGL renderer
    Unknown///< anythong else
};

/**
 * @brief Class BaseRenderer
 */
class BaseRenderer {
public:
    BaseRenderer(const BaseRenderer&)            = delete;
    BaseRenderer(BaseRenderer&&)                 = delete;
    BaseRenderer& operator=(const BaseRenderer&) = delete;
    BaseRenderer& operator=(BaseRenderer&&)      = delete;
    /**
     * @brief Default constructor.
     */
    BaseRenderer() = default;
    /**
     * @brief Destructor.
     */
    virtual ~BaseRenderer();

    // --------- SETTINGS ----------------
    /**
     * @brief Access to renderer Settings
     * @return Renderer Settings
     */
    Settings& settings() { return settingInternal; }

    // --------- STATUS & ERRORS ------
    /**
     * @brief Get the renderer status
     * @return The renderer status
     */
    [[nodiscard]] const Status& getStatus() const { return status; }
    /**
     * @brief Get the last error
     * @return The las error
     */
    [[nodiscard]] const Error& getError() const { return lastError; }
    /**
     * @brief Initialize the renderer
     */
    virtual void Init() = 0;
    /**
     * @brief Starts the renderer
     */
    virtual void run() = 0;

    /**
     * @brief Force display update
     */
    virtual void update() = 0;

    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    virtual void setDrawingCallback(const std::function<void()>& func) = 0;

    /**
     * @brief Gets the renderer Type
     * @return Renderer type
     */
    [[nodiscard]] virtual RendererType getType() const { return RendererType::Unknown; }// ---UNCOVER---


    // Drawing functions
    /**
     * @brief Draw a point
     * @param location Where to draw in screen coordinate
     * @param size Size of the point
     * @param color Color of the point
     */
    virtual void drawPoint(const math::geometry::Vectf& location, double size, const graphics::Color& color) const = 0;
    /**
     * @brief Draw a line
     * @param line Line's data
     * @param width Width of the line
     * @param color Line's color
     */
    virtual void drawLine(const math::geometry::Line2<double>& line, double width, const graphics::Color& color) const = 0;
    /**
     * @brief Draw a quad
     * @param quad Quad's data
     * @param color Quad's color
     */
    virtual void drawQuad(const math::geometry::Quad2<double>& quad, const graphics::Color& color) const = 0;

    /**
     * @brief Draw text on the screen
     * @param text Text to draw
     * @param location Localisation on the screen
     * @param color Color of the text
     */
    virtual void drawText(const std::string& text, const math::geometry::Vectf& location, const graphics::Color& color) const = 0;

protected:
    /// The settings
    Settings settingInternal;
    /// The status
    Status status = Status::Uninitialized;
    /// The last error
    Error lastError = Error::OK;
};


}// namespace rc::core::renderer
