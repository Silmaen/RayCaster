/**
 * @file BaseInput.h
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include "magic_enum.hpp"
#include <nlohmann/json.hpp>

namespace rc::core::input {

/**
 * @brief Type of inputs
 */
enum struct InputType {
    Unknown,///< Unknown input
    GL,     ///< Glut-based input
};

/**
 * @brief List of function keys
 */
enum struct FunctionKey : uint8_t {
    Exit,           ///< Exit Game
    Forward,        ///< Instruct to move forward
    Backward,       ///< Instruct to move backward
    TurnLeft,       ///< Instruct to turn left
    TurnRight,      ///< Instruct to turn right
    StrafeLeft,     ///< Instruct to move left
    StrafeRight,    ///< Instruct to move right
    Use,            ///< Use item or open door
    DisplayTexture, ///< Toggle texture render
    DisplayMap,     ///< Toggle map display
    DisplayRayOnMap,///< toggle ray display on map
    lastKey         ///< lst key for enumeration
};


/**
 * @brief Input settings
 */
struct Settings {
    /**
     * @brief Member access
     * @param key The desired key
     * @return The corresponding char
     */
    char& operator[](const FunctionKey& key) { return m_keys[key]; }
    /**
     * @brief Set from json
     * @param data The input json
     */
    void fromJson(const nlohmann::json& data) {
        for (auto& key : m_keys) {
            if (key.first == FunctionKey::lastKey) continue;// not saving this one!
            if (data.contains(magic_enum::enum_name(key.first)))
                key.second = std::string(data[magic_enum::enum_name(key.first)]).front();
        }
    }
    /**
     * @brief Write to json
     * @return The resulting json
     */
    [[nodiscard]] nlohmann::json toJson() const {
        nlohmann::json data;
        for (const auto& key : m_keys) {
            if (key.first == FunctionKey::lastKey) continue;// not saving this one!
            data[magic_enum::enum_name(key.first)] = std::string(1, key.second);
        }
        return data;
    }

    /**
     * @brief Get the key corresponding to the char
     * @param input The char to check
     * @return The key, or 'lastKey'
     */
    FunctionKey keyByChar(const char& input) {
        auto result = std::find_if(m_keys.begin(), m_keys.end(), [&input](const auto& item) { return item.second == input; });
        if (result == m_keys.end())
            return FunctionKey::lastKey;
        return result->first;
    }

private:
    /**
     * @brief The list of keys
     */
    std::map<FunctionKey, char> m_keys = {
            {FunctionKey::Exit, char(27)},           ///< Exit Game key
            {FunctionKey::Forward, 'z'},        ///< Instruct to move forward key
            {FunctionKey::Backward, 's'},       ///< Instruct to move backward key
            {FunctionKey::TurnLeft, 'q'},       ///< Instruct to turn left key
            {FunctionKey::TurnRight, 'd'},      ///< Instruct to turn right key
            {FunctionKey::StrafeLeft, 'a'},     ///< Instruct to move left key
            {FunctionKey::StrafeRight, 'e'},    ///< Instruct to move right key
            {FunctionKey::Use, ' '},            ///< Use item or open door key
            {FunctionKey::DisplayTexture, 't'}, ///< Toggle map display key
            {FunctionKey::DisplayMap, char(9)},     ///< Toggle map display key
            {FunctionKey::DisplayRayOnMap, 'l'},///< toggle ray display on map key
            {FunctionKey::lastKey, '*'},        ///< The Void
    };
};


/**
 * @brief Class BaseInput
 */
class BaseInput {
public:
    BaseInput(const BaseInput&)            = delete;
    BaseInput(BaseInput&&)                 = delete;
    BaseInput& operator=(const BaseInput&) = delete;
    BaseInput& operator=(BaseInput&&)      = delete;
    /**
     * @brief Default constructor.
     */
    BaseInput();
    /**
     * @brief Destructor.
     */
    virtual ~BaseInput();
    // --------- SETTINGS ----------------
    /**
     * @brief Access to renderer Settings
     * @return Renderer Settings
     */
    Settings& settings() { return m_settingInternal; }
    /**
     * @brief Initialize the renderer
     */
    virtual void Init() = 0;
    /**
     * @brief Check if the given key is pressed
     * @param key Key to check
     * @return True if key pressed
     */
    [[nodiscard]] bool isKeyPressed(const FunctionKey& key) const {
        auto res = m_keyStates.find(key);
        return (res == m_keyStates.end()) ? false : res->second;
    }
    /**
     * @brief Defines the main draw call back
     * @param func The drawing callback
     */
    virtual void setButtonCallback(const std::function<void()>& func) = 0;
    /**
     * @brief Gets the renderer Type
     * @return Renderer type
     */
    [[nodiscard]] virtual InputType getType() const { return InputType::Unknown; }// ---UNCOVER---

protected:
    /**
     * @brief Access to the key states
     * @param key Key to check
     * @return Link to the key state
     */
    bool& getState(const FunctionKey& key) {// ---UNCOVER---
        return m_keyStates[key];            // ---UNCOVER---
    }

private:
    /// The settings
    Settings m_settingInternal;

    /**
     * @brief Key states
     */
    std::map<FunctionKey, bool> m_keyStates;
};

}// namespace rc::core::input
