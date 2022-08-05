/**
 * @file functions.h
 * @author Silmaen
 * @date 05/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#pragma once
#include <algorithm>
#include <cstdint>

namespace rc::math {
/**
 * @brief Simple clamp function with no checks
 * @tparam data Data's type to clamp (must be order-able)
 * @param input Input value
 * @param minValue min value
 * @param maxValue max value
 * @return Clamped value
 */
template<typename data>
constexpr data clamp(const data& input, const data& minValue, const data& maxValue) { return input < minValue ? minValue : input > maxValue ? maxValue :
                                                                                                                                              input; }

/**
 * @brief Clamp function with mix/max ordering
 * @tparam data Data's type to clamp (must be order-able)
 * @param input Input value
 * @param minValue min value
 * @param maxValue max value
 * @return Clamped value
 */
template<typename data>
constexpr data clampSafe(const data& input, data minValue, data maxValue) {
    if (maxValue < minValue) std::swap(minValue, maxValue);
    return input < minValue ? minValue : input > maxValue ? maxValue :
                                                            input;
}

}// namespace rc::math
