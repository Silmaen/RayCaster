/**
 * @file BaseInput.cpp
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "BaseInput.h"

namespace rc::core::input {

BaseInput::BaseInput() {
    constexpr auto values = magic_enum::enum_values<FunctionKey>();
    for (const auto c : values)
        m_keyStates[c] =false;
}

BaseInput::~BaseInput() = default;


}// namespace rc::core::input
