/**
 * @file BaseRenderer.cpp
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "BaseRenderer.h"

namespace rc::graphics::renderer {

void Settings::fromJson(const nlohmann::json& data) {
    if (data.contains("ScreenResolution")) {
        ScreenResolution = data["ScreenResolution"];
    }
    if (data.contains("Background")) {
        Background = data["Background"];
    }
}

nlohmann::json Settings::toJson() const {
    nlohmann::json data;
    data["ScreenResolution"] = ScreenResolution;
    data["Background"]       = Background;
    return data;
}

BaseRenderer::~BaseRenderer() = default;

}// namespace rc::core::renderer
