/**
 * @file GlInput.cpp
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "GlInput.h"
#include "core/renderer/OpenGlRenderer.h"
#include <GL/glut.h>

namespace rc::core::input {

static GLInput* globalPtr = nullptr;///< Global pointer to the actual input...

/**
 * @brief Keyboard input function for glut callback
 * @param key The key pressed
 */
static void buttonDown(uint8_t key, int32_t /*x*/, int32_t /*y*/) {
    globalPtr->button_cb(key, true);
}

/**
 * @brief Keyboard input up function for glut callback
 * @param key The key pressed
 */
static void buttonUp(uint8_t key, int32_t /*x*/, int32_t /*y*/) {
    globalPtr->button_cb(key, false);
}

GLInput::~GLInput() {
    globalPtr = nullptr;
}

void GLInput::Init() {
    globalPtr = this;
    renderer::gl::init();
    glutKeyboardFunc(buttonDown);
    glutKeyboardUpFunc(buttonUp);
}

void GLInput::button_cb(uint8_t key, bool state) {
    getState(settings().keyByChar(key)) = state;
    if (btn)
        btn();
}

void GLInput::setButtonCallback(const std::function<void()>& func) {
    btn = func;
}

}// namespace rc::core::input