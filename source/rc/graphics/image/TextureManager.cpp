/**
 * @file TextureManager.cpp
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "TextureManager.h"
#include <spdlog/spdlog.h>

namespace rc::graphics::image {

/**
 * @brief a void texture
 */
static Texture dummyTex;

const Texture& TextureManager::getTexture(const std::string& name) {
    if (name.empty()) return dummyTex;
    operating.lock();
    if (m_textures.contains(name)) { // texture already loaded
        m_textures[name].m_lastCalled = texClock::now(); // update the touch time
        operating.unlock();
        return m_textures[name].m_texture;
    }
    loadTexture(name);
    m_textures[name].m_lastCalled = texClock::now(); // update the touch time
    operating.unlock();
    return m_textures[name].m_texture;
}

void TextureManager::loadTexture(const std::string& name) {
    spdlog::debug("Loading texture {}",name);
    auto& tex = m_textures[name].m_texture;
    m_textures[name].m_lastCalled = texClock ::now();
    tex.loadFromFile(name);
    m_MemoryUsage += tex.height() * tex.width() * 4 + sizeof(Texture);
    memoryCheck();
}

void TextureManager::unloadTexture(const std::string& name) {
    auto& tex = m_textures[name].m_texture;
    m_MemoryUsage -= tex.height() * tex.width() * 4 + sizeof(Texture);
    m_textures.erase(name);
}

void TextureManager::memoryCheck() {
    while (m_MemoryUsage > m_MemoryLimit) {
        // memory state not ok -> unload oldest texture
        auto min = std::min_element(m_textures.begin(), m_textures.end(),
                         [](const auto& l, const auto& r){return l.second.m_lastCalled<r.second.m_lastCalled;});
        unloadTexture(min->first);
    }
}

void TextureManager::unloadAll() {
    operating.lock();
    m_textures.clear();
    m_MemoryUsage = 0;
    operating.unlock();
}

}// namespace rc::graphics::image
