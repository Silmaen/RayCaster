
#include "base/Engine.h"
#include "testHelper.h"

using Engine=rc::base::Engine;

TEST(Engine, base){
    Engine& engine = Engine::get();
}
