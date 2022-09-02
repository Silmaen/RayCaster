/**
 * @file TextureManager.h
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "Texture.h"

namespace rc::graphics::image {

/**
 * @brief Class TextureManager
 */
class TextureManager {
public:
    TextureManager(const TextureManager&)            = delete;
    TextureManager(TextureManager&&)                 = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager& operator=(TextureManager&&)      = delete;
    /**
     * @brief Destructor.
     */
    ~TextureManager() = default;
    /**
     * @brief Get engine instance
     * @return The engine instance
     */
    static TextureManager& get() {
        static TextureManager instance;
        return instance;
    }

    /**
     * @brief Get the texture
     * @param name Texture's name
     * @return The texture
     */
    const Texture& getTexture(const std::string& name);

    /**
     * @brief unload all textures
     */
    void unloadAll();

    /**
     * @brief Get the loaded texture amount
     * @return Count of loaded texture
     */
    uint16_t getLoadedTextureCount()const{return static_cast<uint16_t>(m_textures.size());}

    /**
     * @brief Get the percentage of used memory
     * @return Used memory
     */
    double getMemoryPercentage()const{return 100.0*static_cast<double>(m_MemoryUsage)/static_cast<double>(m_MemoryLimit);}

    /**
     * @brief Get the memory limit of the Texture manager
     * @return Memory linit
     */
    const size_t& getMemoryLimit()const{return m_MemoryLimit;}
    /**
     * @brief Define the new memory limit
     * @param limit the new limit
     */
    void setMemoryLimit(const size_t& limit){m_MemoryLimit=limit;}
private:
    /**
     * @brief Default constructor.
     */
    TextureManager() = default;


    using texClock = std::chrono::steady_clock;
    using texTime  = texClock::time_point;

    /// Limit memory usage
    size_t m_MemoryLimit = 1073741824;
    /// Current memory
    size_t m_MemoryUsage = 0;

    /**
     * @brief Structure holding info on texture
     */
    struct TextureInfo {
        ///
        texTime m_lastCalled;
        /// The texture
        Texture m_texture;
    };

    /**
     * @brief Load the texture
     * @param name Texture's name
     */
    void loadTexture(const std::string& name);

    /**
     * @brief Unload the texture
     * @param name Texture's name
     */
    void unloadTexture(const std::string& name);

    /**
     * @brief Check actual memory
     */
    void memoryCheck();

    /**
     * @brief The textures al ready loaded
     */
    std::unordered_map<std::string, TextureInfo> m_textures;
};

}// namespace rc::graphics::image
