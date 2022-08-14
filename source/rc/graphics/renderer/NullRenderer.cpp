/**
 * @file NullRenderer.cpp
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "NullRenderer.h"

namespace rc::graphics::renderer {

void NullRenderer::Init() {
    status = Status::Ready;
}

void NullRenderer::run() {
    status = Status::Running;
}

}// namespace rc::base::renderer
