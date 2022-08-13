/**
 * @file BaseInput.cpp
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "BaseInput.h"

namespace rc::core::input {

char& Settings::operator[](const FunctionKey& key) {
    switch (key) {
    case FunctionKey::Exit:
        return mExitKey;
    case FunctionKey::Forward:
        return mForwardKey;
    case FunctionKey::Backward:
        return mBackwardKey;
    case FunctionKey::TurnLeft:
        return mTurnLeftKey;
    case FunctionKey::TurnRight:
        return mTurnRightKey;
    case FunctionKey::StrafeLeft:
        return mStrafeLeftKey;
    case FunctionKey::StrafeRight:
        return mStrafeRightKey;
    case FunctionKey::Use:
        return mUseKey;
    case FunctionKey::DisplayMap:
        return mDisplayMapKey;
    case FunctionKey::DisplayRayOnMap:
        return mDisplayRayOnMapKey;
    case FunctionKey::lastKey:
        return mVoidChar;
    }
    return mVoidChar;// Void char by default!
}

void Settings::fromJson(const nlohmann::json& data) {
    if (data.contains("ExitKey")){
        uint8_t val = data["ExitKey"];
        mExitKey = static_cast<char>(val);
    }
    if (data.contains("ForwardKey")){
        uint8_t val = data["ForwardKey"];
        mForwardKey = static_cast<char>(val);
    }
    if (data.contains("BackwardKey")){
        uint8_t val = data["BackwardKey"];
        mBackwardKey = static_cast<char>(val);
    }
    if (data.contains("TurnLeftKey")){
        uint8_t val = data["TurnLeftKey"];
        mTurnLeftKey = static_cast<char>(val);
    }
    if (data.contains("TurnRightKey")){
        uint8_t val = data["TurnRightKey"];
        mTurnRightKey = static_cast<char>(val);
    }
    if (data.contains("StrafeLeftKey")){
        uint8_t val = data["StrafeLeftKey"];
        mStrafeLeftKey = static_cast<char>(val);
    }
    if (data.contains("StrafeRightKey")){
        uint8_t val = data["StrafeRightKey"];
        mStrafeRightKey = static_cast<char>(val);
    }
    if (data.contains("UseKey")){
        uint8_t val = data["UseKey"];
        mUseKey = static_cast<char>(val);
    }
    if (data.contains("DisplayMapKey")){
        uint8_t val = data["DisplayMapKey"];
        mDisplayMapKey = static_cast<char>(val);
    }
    if (data.contains("DisplayRayOnMapKey")){
        uint8_t val = data["DisplayRayOnMapKey"];
        mDisplayRayOnMapKey = static_cast<char>(val);
    }
}

nlohmann::json Settings::toJson() const {
    nlohmann::json data;
    data["ExitKey"] = mExitKey;
    data["ForwardKey"] = mForwardKey;
    data["BackwardKey"] = mBackwardKey;
    data["TurnLeftKey"] = mTurnLeftKey;
    data["TurnRightKey"] = mTurnRightKey;
    data["StrafeLeftKey"] = mStrafeLeftKey;
    data["StrafeRightKey"] = mStrafeRightKey;
    data["UseKey"] = mUseKey;
    data["DisplayMapKey"] = mDisplayMapKey;
    data["DisplayRayOnMapKey"] = mDisplayRayOnMapKey;
    return data;
}

FunctionKey Settings::keyByChar(const char& input) {
    if (input == mExitKey)
        return FunctionKey::Exit;
    if (input == mForwardKey)
        return FunctionKey::Forward;
    if (input == mBackwardKey)
        return FunctionKey::Backward;
    if (input == mTurnLeftKey)
        return FunctionKey::TurnLeft;
    if (input == mTurnRightKey)
        return FunctionKey::TurnRight;
    if (input == mStrafeLeftKey)
        return FunctionKey::StrafeLeft;
    if (input == mStrafeRightKey)
        return FunctionKey::StrafeRight;
    if (input == mUseKey)
        return FunctionKey::Use;
    if (input == mDisplayMapKey)
        return FunctionKey::DisplayMap;
    if (input == mDisplayRayOnMapKey)
        return FunctionKey::DisplayRayOnMap;
    return FunctionKey::lastKey;
}

BaseInput::~BaseInput() = default;

bool& BaseInput::KeyState::operator[](const FunctionKey& key) {
    switch (key) {
    case FunctionKey::Exit:
        return mExitKey;
    case FunctionKey::Forward:
        return mForwardKey;
    case FunctionKey::Backward:
        return mBackwardKey;
    case FunctionKey::TurnLeft:
        return mTurnLeftKey;
    case FunctionKey::TurnRight:
        return mTurnRightKey;
    case FunctionKey::StrafeLeft:
        return mStrafeLeftKey;
    case FunctionKey::StrafeRight:
        return mStrafeRightKey;
    case FunctionKey::Use:
        return mUseKey;
    case FunctionKey::DisplayMap:
        return mDisplayMapKey;
    case FunctionKey::DisplayRayOnMap:
        return mDisplayRayOnMapKey;
    case FunctionKey::lastKey:
        return mVoidChar;
    }
    return mVoidChar;// Void char by default!
}

const bool& BaseInput::KeyState::operator[](const FunctionKey& key) const {
    switch (key) {
    case FunctionKey::Exit:
        return mExitKey;
    case FunctionKey::Forward:
        return mForwardKey;
    case FunctionKey::Backward:
        return mBackwardKey;
    case FunctionKey::TurnLeft:
        return mTurnLeftKey;
    case FunctionKey::TurnRight:
        return mTurnRightKey;
    case FunctionKey::StrafeLeft:
        return mStrafeLeftKey;
    case FunctionKey::StrafeRight:
        return mStrafeRightKey;
    case FunctionKey::Use:
        return mUseKey;
    case FunctionKey::DisplayMap:
        return mDisplayMapKey;
    case FunctionKey::DisplayRayOnMap:
        return mDisplayRayOnMapKey;
    case FunctionKey::lastKey:
        return mVoidChar;
    }
    return mVoidChar;// Void char by default!
}

}// namespace rc::core::input
