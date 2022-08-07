#include "core/Engine.h"

int main() {
    // creation & initialization of the engine
    auto& app = rc::core::Engine::get();
    app.init();

    // load the map
    app.mapLoad();

    // run main App
    app.run();

    // end
    return 0;
}
