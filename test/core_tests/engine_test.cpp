
#include "core/Engine.h"
#include "testHelper.h"

using Engine = rc::core::Engine;

TEST(Engine, base) {
    auto& engine = Engine::get();
    auto sets    = engine.getSettings();
    engine.display();
    EXPECT_EQ(sets.rendererType, rc::core::renderer::RendererType::OpenGL);
    EXPECT_EQ(sets.rendererSettings.Background, (rc::graphics::Color(76, 76, 76, 255)));
    sets.rendererType = rc::core::renderer::RendererType::Null;
    engine.setSettings(sets);
    auto sets2 = engine.getSettings();
    EXPECT_EQ(sets2.rendererType, rc::core::renderer::RendererType::Null);
    auto player            = std::make_shared<rc::core::Player>();
    auto map = std::make_shared<rc::core::Map>(rc::core::Map::DataType
                                               {{1, 1, 1, 1, 1, 1, 1, 1},
                                                   {1, 0, 1, 0, 0, 0, 0, 1},
                                                   {1, 0, 1, 0, 0, 0, 0, 1},
                                                   {1, 0, 1, 0, 0, 1, 0, 1},
                                                   {1, 0, 0, 0, 0, 0, 0, 1},
                                                   {1, 0, 0, 0, 0, 1, 0, 1},
                                                   {1, 0, 0, 0, 0, 0, 0, 1},
                                                   {1, 1, 1, 1, 1, 1, 1, 1}});
    engine.registerPlayer(player);
    engine.registerMap(map);
    engine.run();
    auto renderer = engine.getRenderer();
    engine.init();
    engine.registerPlayer(player);
    engine.registerMap(map);
    renderer = engine.getRenderer();
    EXPECT_EQ(renderer->getType(),rc::core::renderer::RendererType::Null);
    EXPECT_EQ(renderer->getStatus(), rc::core::renderer::Status::Ready);
    engine.setSettings(sets);
    EXPECT_EQ(renderer->getStatus(), rc::core::renderer::Status::Ready);
    renderer = engine.getRenderer();
    engine.run();
}

TEST(Engine, base2) {
    auto& engine = Engine::get();
    auto sets    = engine.getSettings();
    sets.rendererType = rc::core::renderer::RendererType::Null;
    engine.setSettings(sets);
    engine.init();
    engine.mapLoad();
    engine.run();
    auto renderer = engine.getRenderer();
    EXPECT_EQ(renderer->getStatus(), rc::core::renderer::Status::Running);
    renderer->update();
    renderer->drawPoint({0,0},1,{0,0,0});
    renderer->drawLine({{0,0},{1,1}},1,{0,0,0});
    renderer->drawQuad({{0,0},{1,0},{1,1},{0,1}},{0,0,0});
    engine.display();
}
