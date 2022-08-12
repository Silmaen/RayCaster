/**
 * @file BaseInput.h
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <nlohmann/json.hpp>

namespace rc::core::input {

/**
 * @brief Type of inputs
 */
enum struct InputType {
    Unknown,///< unknown input
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
    char& operator[](const FunctionKey& key);
    /**
     * @brief Set from json
     * @param data The input json
     */
    void fromJson(const nlohmann::json& data);
    /**
     * @brief Write to json
     * @return The resulting json
     */
    [[nodiscard]] nlohmann::json toJson() const;

    /**
     * @brief Get the key corresponding to the char
     * @param input The char to check
     * @return The key, or 'lastKey'
     */
    FunctionKey keyByChar(const char& input);

private:
    char mExitKey            = '!';///< Exit Game key
    char mForwardKey         = 'z';///< Instruct to move forward key
    char mBackwardKey        = 's';///< Instruct to move backward key
    char mTurnLeftKey        = 'q';///< Instruct to turn left key
    char mTurnRightKey       = 'd';///< Instruct to turn right key
    char mStrafeLeftKey      = 'a';///< Instruct to move left key
    char mStrafeRightKey     = 'e';///< Instruct to move right key
    char mUseKey             = ' ';///< Use item or open door key
    char mDisplayMapKey      = 'm';///< Toggle map display key
    char mDisplayRayOnMapKey = 'l';///< toggle ray display on map key
    char mVoidChar           = '$';///< The Void
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
    BaseInput() = default;
    /**
     * @brief Destructor.
     */
    virtual ~BaseInput();
    // --------- SETTINGS ----------------
    /**
     * @brief Access to renderer Settings
     * @return Renderer Settings
     */
    Settings& settings() { return settingInternal; }
    /**
     * @brief Initialize the renderer
     */
    virtual void Init() = 0;
    /**
     * @brief Check if the given key is pressed
     * @param key Key to check
     * @return True if key pressed
     */
    virtual bool isKeyPressed(const FunctionKey& key) const {
        return state[key];
    };
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
        return state[key];                  // ---UNCOVER---
    }

private:
    /// The settings
    Settings settingInternal;

    /**
     * @brief Structure holding the key state
     */
    struct KeyState {
        /**
         * @brief Member access
         * @param key The desired key
         * @return The corresponding char
         */
        bool& operator[](const FunctionKey& key);
        /**
         * @brief Member access
         * @param key The desired key
         * @return The corresponding char
         */
        const bool& operator[](const FunctionKey& key) const;

    private:
        bool mExitKey            = false;///< Exit Game key
        bool mForwardKey         = false;///< Instruct to move forward key
        bool mBackwardKey        = false;///< Instruct to move backward key
        bool mTurnLeftKey        = false;///< Instruct to turn left key
        bool mTurnRightKey       = false;///< Instruct to turn right key
        bool mStrafeLeftKey      = false;///< Instruct to move left key
        bool mStrafeRightKey     = false;///< Instruct to move right key
        bool mUseKey             = false;///< Use item or open door key
        bool mDisplayMapKey      = false;///< Toggle map display key
        bool mDisplayRayOnMapKey = false;///< toggle ray display on map key
        bool mVoidChar           = false;///< The Void
    };
    /// keyboard state
    KeyState state;
};

}// namespace rc::core::input
