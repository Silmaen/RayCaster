/**
 * @file OpenGlRenderer.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "BaseRenderer.h"

namespace rc::core::renderer {

/**
 * @brief OpenGL utilities
 */
namespace gl {
/**
 * @brief Initialize Glut
 */
void init();
}// namespace gl

/**
 * @brief Class OpenGLRenderer
 */
class OpenGLRenderer : public BaseRenderer {
public:
    OpenGLRenderer(const OpenGLRenderer&)            = delete;
    OpenGLRenderer(OpenGLRenderer&&)                 = delete;
    OpenGLRenderer& operator=(const OpenGLRenderer&) = delete;
    OpenGLRenderer& operator=(OpenGLRenderer&&)      = delete;
    /**
     * @brief Default constructor.
     */
    OpenGLRenderer() = default;
    /**
     * @brief Destructor.
     */
    ~OpenGLRenderer() override;
    /**
     * @brief Initialize the renderer
     */
    void Init() override;
    /**
     * @brief Starts the renderer
     */
    void run() override;
    /**
     * @brief Force display update
     */
    void update() override;
    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    void setDrawingCallback(const std::function<void()>& func) override;

    /**
    * @brief Gets the renderer Type
    * @return Renderer type
    */
    [[nodiscard]] RendererType getType() const override { return RendererType::OpenGL; }

    // Drawing functions
    /**
     * @brief Draw a point
     * @param location Where to draw in screen coordinate
     * @param size Size of the point
     * @param color Color of the point
     */
    void drawPoint(const math::Vectf& location, double size, const graphics::Color& color) const override;
    /**
     * @brief Draw a line
     * @param line Line's data
     * @param width Width of the line
     * @param color Line's color
     */
    void drawLine(const graphics::Line2<double>& line, double width, const graphics::Color& color) const override;
    /**
     * @brief Draw a quad
     * @param quad Quad's data
     * @param color Quad's color
     */
    void drawQuad(const graphics::Quad2<double>& quad, const graphics::Color& color) const override;

    /**
     * @brief Draw text on the screen
     * @param text Text to draw
     * @param location Localisation on the screen
     * @param color Color of the text
     */
    void drawText(const std::string& text, const math::Vectf& location, const graphics::Color& color) const override;

    /**
     * @brief Display call back
     */
    void display_cb();

private:
    std::function<void()> mainDraw;

    static void setColor(const graphics::Color& color);

    static void pushVertex(const math::Vectf& vertex);
};

}// namespace rc::core::renderer
