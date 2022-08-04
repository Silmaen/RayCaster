
#include "base/Engine.h"
#include "testHelper.h"

using Engine=rc::base::Engine;

TEST(Engine, base){
    auto& engine = Engine::get();
    EXPECT_EQ(engine.getResolution()[0], 1024);
    EXPECT_EQ(engine.getResolution()[1], 512);
}
