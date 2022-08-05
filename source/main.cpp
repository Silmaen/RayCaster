#include "base/Engine.h"
#include "base/Map.h"

int main() {
    // creation & initialization of the engine
    auto& app = rc::base::Engine::get();
    app.init();

    // load a map
    auto map = std::make_shared<rc::base::Map>(rc::base::Map::DataType
            {{1, 1, 1, 1, 1, 1, 1, 1},
             {1, 0, 1, 0, 0, 0, 0, 1},
             {1, 0, 1, 0, 0, 0, 0, 1},
             {1, 0, 1, 0, 0, 1, 0, 1},
             {1, 0, 0, 0, 0, 0, 0, 1},
             {1, 0, 0, 0, 0, 1, 0, 1},
             {1, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 1, 1}});
    // create a player
    auto player            = std::make_shared<rc::base::Player>();
    player->setPosition({200, 300});
    player->setDirection({1.0, 0.0});

    // Register player to the engine
    app.registerPlayer(player);
    // register the map to the engine
    app.registerMap(map);

    // run main App
    app.run();

    // end
    return 0;
}
