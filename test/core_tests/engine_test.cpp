
#include "core/Engine.h"
#include "testHelper.h"
#include "core/fs/DataFile.h"

using Engine = rc::core::Engine;

TEST(Engine, base) {
    auto& engine = Engine::get();
    auto sets    = engine.getSettings();
    engine.display();
    EXPECT_EQ(sets.rendererType, rc::graphics::renderer::RendererType::OpenGL);
    EXPECT_EQ(sets.rendererSettings.Background, (rc::graphics::Color(76, 76, 76, 255)));
    sets.rendererType = rc::graphics::renderer::RendererType::Null;
    engine.setSettings(sets);
    auto sets2 = engine.getSettings();
    EXPECT_EQ(sets2.rendererType, rc::graphics::renderer::RendererType::Null);
    engine.run();
    auto renderer = engine.getRenderer();
    engine.init();
    renderer = engine.getRenderer();
    EXPECT_EQ(renderer->getType(),rc::graphics::renderer::RendererType::Null);
    EXPECT_EQ(renderer->getStatus(), rc::graphics::renderer::Status::Ready);
    engine.setSettings(sets);
    EXPECT_EQ(renderer->getStatus(), rc::graphics::renderer::Status::Ready);
    renderer = engine.getRenderer();
    engine.run();
}

TEST(Engine, base2) {
    auto& engine = Engine::get();
    auto sets    = engine.getSettings();
    sets.rendererType = rc::graphics::renderer::RendererType::Null;
    engine.setSettings(sets);
    engine.init();
    engine.mapLoad("E1L1");
    engine.saveSettings("settings_temp.json");
    engine.run();
    auto renderer = engine.getRenderer();
    EXPECT_EQ(renderer->getStatus(), rc::graphics::renderer::Status::Running);
    renderer->update();
    renderer->drawPoint({0,0},1,{0,0,0});
    renderer->drawLine({{0,0},{1,1}},1,{0,0,0});
    renderer->drawQuad({{0,0},{1,0},{1,1},{0,1}},{0,0,0});
    engine.display();
    rc::core::fs::DataFile fSets("settings_temp.json");
    fSets.remove();
}
