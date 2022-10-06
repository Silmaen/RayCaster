#include "core/Engine.h"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

static std::filesystem::path baseExecPath;

void startSpdlog() {
    spdlog::default_logger()->sinks().push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>((baseExecPath / "exec.log").generic_string()));
#ifdef RAY_DEBUG
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_every(std::chrono::seconds(1U));
#else
    spdlog::set_level(spdlog::level::info);
#endif
}

int main([[maybe_unused]]int argc, char* argv[]) {
    baseExecPath= std::filesystem::absolute(std::filesystem::path(argv[0])).parent_path();
    startSpdlog();
    spdlog::info("Start Application in {}", baseExecPath.string());
    // creation & initialization of the engine
    auto& app = rc::core::Engine::get();
    app.init();
    spdlog::info("Application initialized");

    // load the map
    app.mapLoad("E1L1");
    spdlog::info("initial map loaded");

    // run main App
    app.run();
    spdlog::info("Application finished");

    // end
    return 0;
}
