
#include "base/Engine.h"
#include "testHelper.h"

using Engine = rc::base::Engine;

TEST(Engine, base) {
    auto& engine = Engine::get();
    auto sets    = engine.getSettings();
    engine.display();
    EXPECT_EQ(sets.rendererType, rc::base::renderer::RendererType::OpenGL);
    EXPECT_EQ(sets.rendererSettings.Background, (rc::graphics::Color(76, 76, 76, 255)));
    sets.rendererType = rc::base::renderer::RendererType::Null;
    engine.setSettings(sets);
    auto sets2 = engine.getSettings();
    EXPECT_EQ(sets2.rendererType, rc::base::renderer::RendererType::Null);
    auto player            = std::make_shared<rc::base::Player>();
    auto map = std::make_shared<rc::base::Map>(rc::base::Map::DataType
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
    EXPECT_EQ(renderer->getType(),rc::base::renderer::RendererType::Null);
    EXPECT_EQ(renderer->getStatus(), rc::base::renderer::Status::Ready);
    engine.setSettings(sets);
    EXPECT_EQ(renderer->getStatus(), rc::base::renderer::Status::Ready);
    renderer = engine.getRenderer();
    engine.run();
    EXPECT_EQ(renderer->getStatus(), rc::base::renderer::Status::Running);
    engine.registerPlayer(player);
    engine.registerMap(map);
    engine.setSettings(sets);
    renderer->update();
    renderer->drawPoint({0,0},1,{0,0,0});
    renderer->drawLine({{0,0},{1,1}},1,{0,0,0});
    renderer->drawQuad({{0,0},{1,0},{1,1},{0,1}},{0,0,0});
    player->draw();
    map->draw();
    engine.display();
}
