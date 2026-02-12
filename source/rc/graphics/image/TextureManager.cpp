/**
 * @file TextureManager.cpp
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "TextureManager.h"
#include <algorithm>
#include <vector>

namespace rc::graphics::image {

/**
 * @brief a void texture
 */
static Texture dummyTex;

const Texture& TextureManager::getTexture(const std::string& name) {
    if (name.empty()) return dummyTex;
    if (m_textures.contains(name)) { // texture already loaded
        m_textures[name].m_lastCalled = texClock::now(); // update the touch time
        return m_textures[name].m_texture;
    }
    loadTexture(name);
    m_textures[name].m_lastCalled = texClock::now(); // update the touch time
    return m_textures[name].m_texture;
}

void TextureManager::loadTexture(const std::string& name) {
    auto& tex = m_textures[name].m_texture;
    m_textures[name].m_lastCalled = texClock ::now();
    tex.loadFromFile(name);
    m_MemoryUsage += tex.height() * tex.width() * 4 + sizeof(Texture);
    memoryCheck();
}

void TextureManager::unloadTexture(const std::string& name) {
    const auto& tex = m_textures[name].m_texture;
    m_MemoryUsage -= tex.height() * tex.width() * 4 + sizeof(Texture);
    m_textures.erase(name);
}

void TextureManager::memoryCheck() {
    if (m_MemoryUsage <= m_MemoryLimit)
        return;
    // Sort texture names by last access time (oldest first)
    std::vector<std::string> sortedNames;
    sortedNames.reserve(m_textures.size());
    for (const auto& [name, data] : m_textures)
        sortedNames.push_back(name);
    std::sort(sortedNames.begin(), sortedNames.end(),
              [this](const auto& l, const auto& r) { return m_textures[l].m_lastCalled < m_textures[r].m_lastCalled; });
    for (const auto& name : sortedNames) {
        if (m_MemoryUsage <= m_MemoryLimit)
            break;
        unloadTexture(name);
    }
}

void TextureManager::unloadAll() {
    m_textures.clear();
    m_MemoryUsage = 0;
}

}// namespace rc::graphics::image
