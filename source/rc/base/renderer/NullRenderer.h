/**
 * @file NullRenderer.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "BaseRenderer.h"
namespace rc::base::renderer {

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
    void update() override{}

    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    void setDrawingCallback([[maybe_unused]] const std::function<void()>& func) override{}
    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    void setButtonCallback([[maybe_unused]] const std::function<void(uint8_t key,int32_t x,int32_t y)>& func) override{}
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
    void drawPoint([[maybe_unused]] const math::Vector2<double>& location, [[maybe_unused]] double size, [[maybe_unused]] const graphics::Color& color) const override{}
    /**
     * @brief Draw a line
     * @param line Line's data
     * @param width Width of the line
     * @param color Line's color
     */
    void drawLine([[maybe_unused]] const graphics::Line2<double>& line, [[maybe_unused]] double width, [[maybe_unused]] const graphics::Color& color) const override{}
    /**
     * @brief Draw a quad
     * @param quad Quad's data
     * @param color Quad's color
     */
    void drawQuad([[maybe_unused]] const graphics::Quad2<double>& quad, [[maybe_unused]] const graphics::Color& color) const override{}

private:
};

}// namespace rc::base::renderer
