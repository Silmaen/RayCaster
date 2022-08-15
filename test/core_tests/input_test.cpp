/**
 * @file input_test.cpp
 * @author Silmaen
 * @date 12/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "testHelper.h"
#include "core/input/BaseInput.h"

using namespace rc::core::input;

TEST(InputSetting, base){
    Settings settings;
    settings[FunctionKey::Exit]='h';
    auto data = settings.toJson();
    Settings settings1;
    settings1.fromJson(data);
    EXPECT_EQ(settings1[FunctionKey::Exit], settings[FunctionKey::Exit]);
}

TEST(InputSetting, conversonValue){
    Settings settings;
    EXPECT_EQ(settings[settings.keyByChar('z')],'z');
    EXPECT_EQ(settings[settings.keyByChar('q')],'q');
    EXPECT_EQ(settings[settings.keyByChar('s')],'s');
    EXPECT_EQ(settings[settings.keyByChar('d')],'d');
    EXPECT_EQ(settings[settings.keyByChar('e')],'e');
    EXPECT_EQ(settings[settings.keyByChar('a')],'a');
    EXPECT_EQ(settings[settings.keyByChar('l')],'l');
    EXPECT_EQ(settings[settings.keyByChar('m')],'m');
    EXPECT_EQ(settings[settings.keyByChar('t')],'t');
    EXPECT_EQ(settings[settings.keyByChar('!')],'!');
    EXPECT_EQ(settings[settings.keyByChar(' ')],' ');
    EXPECT_EQ(settings[settings.keyByChar('_')],'*');
}
