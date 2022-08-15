/**
 * @file GlInput.h
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "BaseInput.h"

namespace rc::core::input {

/**
 * @brief Class GLInput
 */
class GLInput : public BaseInput {
public:
    GLInput(const GLInput&)            = delete;
    GLInput(GLInput&&)                 = delete;
    GLInput& operator=(const GLInput&) = delete;
    GLInput& operator=(GLInput&&)      = delete;
    /**
     * @brief Default constructor.
     */
    GLInput();
    /**
     * @brief Destructor.
     */
    ~GLInput() override;
    /**
     * @brief Initialize the renderer
     */
    void Init() override;
    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    void setButtonCallback(const std::function<void()>& func) override;
    /**
     * @brief Glut input callback function
     * @param key Input key
     * @param state If the button is pressed (true) or released (false)
     */
    void button_cb(char key, bool state);

private:
    /// Call back after button update
    std::function<void()> btn;
};

}// namespace rc::core::input
