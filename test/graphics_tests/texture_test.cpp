/**
 * @file texture_test.cpp
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "graphics/image/Texture.h"
#include "testHelper.h"

using namespace rc::graphics::image;

TEST(Texture, dummy){
    Texture voidTex;
    EXPECT_EQ(voidTex.width(), 0);
    EXPECT_EQ(voidTex.height(), 0);
    EXPECT_EQ(voidTex.getPixel(45,45),rc::graphics::Color(0,0,0,0));
    EXPECT_EQ(voidTex.getPixel(45,45,2),rc::graphics::Color(0,0,0,0));
}

TEST(Texture, loadbadname) {
    Texture baseTex;
    baseTex.loadFromFile("bad.png");
    EXPECT_EQ(baseTex.width(), 0);
    EXPECT_EQ(baseTex.height(), 0);
}

TEST(Texture, LoadSavePNG){
    Texture baseTex;
    baseTex.loadFromFile("brickpattern.png");
    EXPECT_EQ(baseTex.width(), 64);
    EXPECT_EQ(baseTex.height(), 64);
    EXPECT_EQ(baseTex.getPixel(45,45),rc::graphics::Color(0,0,0));
    EXPECT_EQ(baseTex.getPixel(45,45,2),rc::graphics::Color(102,102,102));
    baseTex.saveToFile("testSave.png");
    Texture baseTex2;
    baseTex2.loadFromFile("testSave.png");
    EXPECT_EQ(baseTex2.getPixel(45,45),rc::graphics::Color(0,0,0));
    rc::core::fs::DataFile file("textures/testSave.png");
    file.remove();
}
