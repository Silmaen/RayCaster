/**
 * @file texturemanager_test.cpp
 * @author Silmaen
 * @date 14/08/2022
 * Copyright © 2022 All rights reserved.
 * All modification must get authorization from the author.
 */

#include "graphics/image/TextureManager.h"
#include "testHelper.h"

using TextureManager = rc::graphics::image::TextureManager;

TEST(TextureManager, base){
    auto& texMng = TextureManager::get();
    EXPECT_EQ(texMng.getLoadedTextureCount(),0);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 0, 0.001);
    EXPECT_EQ(texMng.getMemoryLimit(), 1073741824);
}

TEST(TextureManager, load){
    auto& texMng = TextureManager::get();
    // texture manager should be empty
    EXPECT_EQ(texMng.getLoadedTextureCount(), 0);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 0.0, 0.0001);
    // Try to get a texture: should load the file
    auto tex = texMng.getTexture("brickpattern.png");
    EXPECT_EQ(tex.width(), 64);
    EXPECT_EQ(tex.height(), 64);
    EXPECT_EQ(texMng.getLoadedTextureCount(), 1);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 0.0015, 0.0001);
    // try to get a second time the same texture: should not load
    auto tex2 = texMng.getTexture("brickpattern.png");
    EXPECT_EQ(tex2.width(), 64);
    EXPECT_EQ(tex2.height(), 64);
    EXPECT_EQ(texMng.getLoadedTextureCount(), 1);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 0.0015, 0.0001);
    // Empty the texture manager
    texMng.unloadAll();
    EXPECT_EQ(texMng.getLoadedTextureCount(), 0);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 0.0, 0.0001);
}

TEST(TextureManager, memoryClean){
    auto& texMng = TextureManager::get();
    size_t memLimit = texMng.getMemoryLimit();
    texMng.setMemoryLimit(24576); // limit the manager to 24kb (enough for one 64x64 texture, not for 2)
    // load a texture... should be OK
    auto tex = texMng.getTexture("brickpattern.png");
    EXPECT_EQ(tex.width(), 64);
    EXPECT_EQ(tex.height(), 64);
    EXPECT_EQ(texMng.getLoadedTextureCount(), 1);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 66.86, 0.01);
    // load a second texture... should unload the first one
    auto tex2 = texMng.getTexture("doorpattern.png");
    EXPECT_EQ(tex.width(), 64);
    EXPECT_EQ(tex.height(), 64);
    EXPECT_EQ(texMng.getLoadedTextureCount(), 1);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 66.86, 0.01);
    // reset limit to default
    texMng.setMemoryLimit(memLimit);
    // should have 2 textures in manager
    tex = texMng.getTexture("brickpattern.png");
    EXPECT_EQ(tex.width(), 64);
    EXPECT_EQ(tex.height(), 64);
    EXPECT_EQ(texMng.getLoadedTextureCount(), 2);
    EXPECT_NEAR(texMng.getMemoryPercentage(), 0.00306, 0.0001);
    // Empty the texture manager
    texMng.unloadAll();
    EXPECT_EQ(texMng.getLoadedTextureCount(), 0);
}
