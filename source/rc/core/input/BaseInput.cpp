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
        break;
    case FunctionKey::Forward:
        return mForwardKey;
        break;
    case FunctionKey::Backward:
        return mBackwardKey;
        break;
    case FunctionKey::TurnLeft:
        return mTurnLeftKey;
        break;
    case FunctionKey::TurnRight:
        return mTurnRightKey;
        break;
    case FunctionKey::StrafeLeft:
        return mStrafeLeftKey;
        break;
    case FunctionKey::StrafeRight:
        return mStrafeRightKey;
        break;
    case FunctionKey::Use:
        return mUseKey;
        break;
    case FunctionKey::DisplayMap:
        return mDisplayMapKey;
        break;
    case FunctionKey::DisplayRayOnMap:
        return mDisplayRayOnMapKey;
        break;
    case FunctionKey::lastKey:
        return mVoidChar;
        break;
    }
    return mVoidChar;// Void char by default!
}

void Settings::fromJson(const nlohmann::json& data) {
    if (data.contains("ExitKey")){
        uint8_t val = data["ExitKey"];
        mExitKey = val;
    }
    if (data.contains("ForwardKey")){
        uint8_t val = data["ForwardKey"];
        mForwardKey = val;
    }
    if (data.contains("BackwardKey")){
        uint8_t val = data["BackwardKey"];
        mBackwardKey = val;
    }
    if (data.contains("TurnLeftKey")){
        uint8_t val = data["TurnLeftKey"];
        mTurnLeftKey = val;
    }
    if (data.contains("TurnRightKey")){
        uint8_t val = data["TurnRightKey"];
        mTurnRightKey = val;
    }
    if (data.contains("StrafeLeftKey")){
        uint8_t val = data["StrafeLeftKey"];
        mStrafeLeftKey = val;
    }
    if (data.contains("StrafeRightKey")){
        uint8_t val = data["StrafeRightKey"];
        mStrafeRightKey = val;
    }
    if (data.contains("UseKey")){
        uint8_t val = data["UseKey"];
        mUseKey = val;
    }
    if (data.contains("DisplayMapKey")){
        uint8_t val = data["DisplayMapKey"];
        mDisplayMapKey = val;
    }
    if (data.contains("DisplayRayOnMapKey")){
        uint8_t val = data["DisplayRayOnMapKey"];
        mDisplayRayOnMapKey = val;
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
        break;
    case FunctionKey::Forward:
        return mForwardKey;
        break;
    case FunctionKey::Backward:
        return mBackwardKey;
        break;
    case FunctionKey::TurnLeft:
        return mTurnLeftKey;
        break;
    case FunctionKey::TurnRight:
        return mTurnRightKey;
        break;
    case FunctionKey::StrafeLeft:
        return mStrafeLeftKey;
        break;
    case FunctionKey::StrafeRight:
        return mStrafeRightKey;
        break;
    case FunctionKey::Use:
        return mUseKey;
        break;
    case FunctionKey::DisplayMap:
        return mDisplayMapKey;
        break;
    case FunctionKey::DisplayRayOnMap:
        return mDisplayRayOnMapKey;
        break;
    case FunctionKey::lastKey:
        return mVoidChar;
        break;
    }
    return mVoidChar;// Void char by default!
}

const bool& BaseInput::KeyState::operator[](const FunctionKey& key) const {
    switch (key) {
    case FunctionKey::Exit:
        return mExitKey;
        break;
    case FunctionKey::Forward:
        return mForwardKey;
        break;
    case FunctionKey::Backward:
        return mBackwardKey;
        break;
    case FunctionKey::TurnLeft:
        return mTurnLeftKey;
        break;
    case FunctionKey::TurnRight:
        return mTurnRightKey;
        break;
    case FunctionKey::StrafeLeft:
        return mStrafeLeftKey;
        break;
    case FunctionKey::StrafeRight:
        return mStrafeRightKey;
        break;
    case FunctionKey::Use:
        return mUseKey;
        break;
    case FunctionKey::DisplayMap:
        return mDisplayMapKey;
        break;
    case FunctionKey::DisplayRayOnMap:
        return mDisplayRayOnMapKey;
        break;
    case FunctionKey::lastKey:
        return mVoidChar;
        break;
    }
    return mVoidChar;// Void char by default!
}

}// namespace rc::core::input
