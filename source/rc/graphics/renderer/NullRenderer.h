/**
 * @file NullRenderer.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "BaseRenderer.h"

/**
 * @brief Namespace for the graphics renderers
 */
namespace rc::graphics::renderer {

/**
 * @brief Class NullRenderer
 */
class NullRenderer : public BaseRenderer {
public:
    NullRenderer(const NullRenderer&)            = delete;
    NullRenderer(NullRenderer&&)                 = delete;
    NullRenderer& operator=(const NullRenderer&) = delete;
    NullRenderer& operator=(NullRenderer&&)      = delete;
    /**
     * @brief Default constructor.
     */
    NullRenderer() = default;
    /**
     * @brief Destructor.
     */
    ~NullRenderer() override = default;
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
    void update() override {}

    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    void setDrawingCallback([[maybe_unused]] const std::function<void()>& func) override {}
    /**
      * @brief Gets the renderer Type
      * @return Renderer type
      */
    [[nodiscard]] RendererType getType() const override { return RendererType::Null; }
    // Drawing functions
    /**
     * @brief Draw a point
     * @param location Where to draw in screen coordinate
     * @param size Size of the point
     * @param color Color of the point
     */
    void drawPoint([[maybe_unused]] const math::geometry::Vectf& location, [[maybe_unused]] double size, [[maybe_unused]] const graphics::Color& color) const override {}
    /**
     * @brief Draw a line
     * @param line Line's data
     * @param width Width of the line
     * @param color Line's color
     */
    void drawLine([[maybe_unused]] const math::geometry::Line2<double>& line, [[maybe_unused]] double width, [[maybe_unused]] const graphics::Color& color) const override {}

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
    virtual void drawTextureVerticalLine([[maybe_unused]] double lineX, [[maybe_unused]] double lineY, [[maybe_unused]] double lineLength, [[maybe_unused]] const image::Texture& tex, [[maybe_unused]] double texX, [[maybe_unused]] const math::geometry::Box2& drawBox, [[maybe_unused]] bool dark=false) const override{};
    /**
     * @brief Draw a quad
     * @param quad Quad's data
     * @param color Quad's color
     */
    void drawQuad([[maybe_unused]] const math::geometry::Quad2<double>& quad, [[maybe_unused]] const graphics::Color& color) const override {}

    /**
     * @brief Draw text on the screen
     * @param text Text to draw
     * @param location Localisation on the screen
     * @param color Color of the text
     */
    void drawText([[maybe_unused]] const std::string& text, [[maybe_unused]] const math::geometry::Vectf& location, [[maybe_unused]] const graphics::Color& color) const override {}

private:
};

}// namespace rc::core::renderer
