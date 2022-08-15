/**
 * @file OpenGlRenderer.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "BaseRenderer.h"

namespace rc::graphics::renderer {

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
    void drawPoint(const math::geometry::Vectf& location, double size, const graphics::Color& color) const override;
    /**
     * @brief Draw a line
     * @param line Line's data
     * @param width Width of the line
     * @param color Line's color
     */
    void drawLine(const math::geometry::Line2<double>& line, double width, const graphics::Color& color) const override;

    /**
     * @brief Draw a textured vertical line
     * @param lineX X coordinate of the line
     * @param lineY Y starting of the line (may be outside the layout)
     * @param lineLength Length of the line
     * @param tex Texture to apply
     * @param texX X coordinate on the texture image
     * @param drawBox Drawing layout
     * @param dark If the color should be shaded
     */
    void drawTextureVerticalLine(double lineX, double lineY, double lineLength, const image::Texture& tex, double texX, const math::geometry::Box2& drawBox, bool dark=false)const override;
    /**
     * @brief Draw a quad
     * @param quad Quad's data
     * @param color Quad's color
     */
    void drawQuad(const math::geometry::Quad2<double>& quad, const graphics::Color& color) const override;

    /**
     * @brief Draw text on the screen
     * @param text Text to draw
     * @param location Localisation on the screen
     * @param color Color of the text
     */
    void drawText(const std::string& text, const math::geometry::Vectf& location, const graphics::Color& color) const override;

    /**
     * @brief Display call back
     */
    void display_cb();

private:
    std::function<void()> mainDraw;

    static void setColor(const graphics::Color& color);

    static void pushVertex(const math::geometry::Vectf& vertex);
};

}// namespace rc::core::renderer
