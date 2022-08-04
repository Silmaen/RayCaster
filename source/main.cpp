#include "base/Engine.h"
#include "base/Map.h"

void drawRays2D(const std::shared_ptr<rc::base::Player>& player, const rc::base::Map& map) {
    using Unit = rc::math::Angle::Unit;
    auto& engine = rc::base::Engine::get();
    double fov = 60.0;
    double increment = 1.0;
    auto ray = player->getDirection().rotated(rc::math::Angle{-fov/2, Unit::Degree});
    rc::math::Vector2<double> rayTmp;
    engine.drawQuad({{526, 0}, {1006, 0}, {1006, 160}, {526, 160}}, {0, 255, 255});
    engine.drawQuad({{526, 160}, {1006, 160}, {1006, 320}, {526, 320}}, {0, 0, 255});
    for (double rays = 0; rays <= fov/increment; ++rays) {
        auto result = map.castRay(player->getPosition(),ray);
        if (result.hitVertical) {//vertical hit first
            engine.drawLine({player->getPosition(), ray, result.distance}, 2, {0,204,0});//draw 2D ray
        }else{//horizontal hit first
            engine.drawLine({player->getPosition(), ray, result.distance}, 2, {0,153,0});//draw 2D ray
        }
        constexpr uint32_t mapS = 64;
        auto lineH = static_cast<int32_t>((mapS * 320) / (result.distance * player->getDirection().dot(ray)));
        if (lineH > 320) { lineH = 320; }//line height and limit
        double lineOff = 160 - (lineH >> 1);//line offset
        //draw vertical wall
        if (result.hitVertical) {
            engine.drawLine({{rays * 8 + 530, lineOff}, {rays * 8 + 530, lineOff + lineH}}, 8.0, {0,204,0});
        }else{
            engine.drawLine({{rays * 8 + 530, lineOff}, {rays * 8 + 530, lineOff + lineH}}, 8.0, {0,153,0});
        }
        ray.rotate(rc::math::Angle{increment, Unit::Degree});//go to next ray
    }
}

int main(int argc, char* argv[]) {
    auto& app = rc::base::Engine::get();
    app.init(argc, argv);
    rc::base::Map TheMap{
            {{1, 1, 1, 1, 1, 1, 1, 1},
             {1, 0, 1, 0, 0, 0, 0, 1},
             {1, 0, 1, 0, 0, 0, 0, 1},
             {1, 0, 1, 0, 0, 0, 0, 1},
             {1, 0, 0, 0, 0, 0, 0, 1},
             {1, 0, 0, 0, 0, 1, 0, 1},
             {1, 0, 0, 0, 0, 0, 0, 1},
             {1, 1, 1, 1, 1, 1, 1, 1}}};
    auto player            = std::make_shared<rc::base::Player>();
    player->setPosition({200, 300});
    player->setDirection({1.0, 0.0});
    app.registerPlayer(player);
    app.registerRender([&TheMap] { TheMap.draw(); });
    app.registerRender([&TheMap, player] { drawRays2D(player,TheMap); });
    app.run();
    return 0;
}
