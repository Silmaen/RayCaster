
#include "core/fs/DataFile.h"
#include "game/Map.h"
#include "testHelper.h"
#include <chrono>

using testClock = std::chrono::steady_clock;
using timePoint = testClock::time_point;
using timeDelta = testClock::duration;

using Map = rc::game::Map;

static Map ConstructBaseMap() {
    rc::game::mapCell walls{false, false, 10};
    rc::game::mapCell voids{true, true, 0};
    Map map{{{walls, walls, walls, walls, walls, walls, walls, walls},
             {walls, voids, walls, voids, voids, voids, voids, walls},
             {walls, voids, walls, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, walls, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, walls, walls, walls, walls, walls, walls, walls}}};
    map.setPlayerStart({245, 125}, {0, -1});
    return map;
}

TEST(Map, base) {
    Map map;
    EXPECT_FALSE(map.isValid());
    EXPECT_EQ(map.height(), 0);
    EXPECT_EQ(map.width(), 0);
    map.setMap({{}, {}});
    EXPECT_FALSE(map.isValid());
    EXPECT_EQ(map.height(), 0);
    EXPECT_EQ(map.width(), 2);
    map.setMap({{{false, false, 1}, {false, false, 1}}, {{false, false, 1}}});
    EXPECT_FALSE(map.isValid());
    Map::DataType data = {{{false, false, 1}, {true, true, 0}}, {{true, true, 0}, {false, false, 1}}};
    map.setMap(data);
    EXPECT_TRUE(map.isValid());
    EXPECT_EQ(map.getMapData(), data);
    EXPECT_EQ(map({0, 1}), (Map::BaseType{true, true, 0}));
}

TEST(Map, init) {
    {
        const Map map({{{false, false, 1}, {false, false, 2}, {false, false, 3}}, {{false, false, 4}, {false, false, 5}, {false, false, 6}}, {{false, false, 7}, {false, false, 8}, {false, false, 9}}});
        EXPECT_EQ(map({0, 1}), (Map::BaseType{false, false, 4}));
        EXPECT_EQ(map({1, 2}), (Map::BaseType{false, false, 8}));
    }
    {
        Map::DataType data = {{{false, false, 1}, {false, false, 2}, {false, false, 3}}, {{false, false, 4}, {false, false, 5}, {false, false, 6}}, {{false, false, 7}, {false, false, 8}, {false, false, 9}}};
        Map map(data);
        EXPECT_EQ(map.getMapData(), data);
    }
    {
        Map map(0, 0);
        EXPECT_FALSE(map.isValid());
    }
    {
        Map map(10, 12);
        EXPECT_EQ(map.height(), 10);
        EXPECT_EQ(map.width(), 12);
    }
}

TEST(Map, CheckInside) {
    Map map(10, 10);
    ASSERT_TRUE(map.isValid());
    EXPECT_TRUE(map.isIn(Map::worldCoordinates{50.0, 50.0}));
    EXPECT_FALSE(map.isIn(Map::worldCoordinates{-50.0, 50.0}));
    EXPECT_FALSE(map.isIn(Map::worldCoordinates{50.0, -50.0}));
    EXPECT_FALSE(map.isIn(Map::worldCoordinates{5000000.0, 50.0}));
    EXPECT_FALSE(map.isIn(Map::worldCoordinates{50.0, 50000000.0}));
    EXPECT_TRUE(map.isIn(Map::gridCoordinate{5, 5}));
    EXPECT_FALSE(map.isIn(Map::gridCoordinate{11, 5}));
    EXPECT_FALSE(map.isIn(Map::gridCoordinate{5, 11}));
}

TEST(Map, whichCell) {
    Map map(10, 10, 64);
    auto loc = map.whichCell({150, 250});
    EXPECT_EQ(loc[0], 2);
    EXPECT_EQ(loc[1], 3);
    loc = map.whichCell({-1, -1});
    EXPECT_EQ(loc[0], 255);
    EXPECT_EQ(loc[1], 255);
}

TEST(Map, castRay) {
    using Unit = rc::math::geometry::Angle::Unit;
    rc::game::mapCell walls{false, false, 1};
    rc::game::mapCell voids{true, true, 0};
    Map map{{{walls, walls, walls, walls, walls, walls, walls, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, walls, walls, walls, walls, walls, walls, walls}}};
    std::vector<Map::rayCastResult2D> expecteds = {
            {198.00100000000000, {448.00100000000000, 320.00000000000000}, true, 0},       //  0
            {203.92674045587697, {448.00100000000000, 368.80261688033164}, true, 48.8026}, //  1
            {223.61447468465050, {448.00100000000000, 423.91841649819440}, true, 39.9184}, //  2
            {193.02679246731799, {394.48213249125672, 448.00100000000000}, false, 53.5179},//  3
            {155.53242609963525, {338.35202073206085, 448.00100000000000}, false, 45.648}, //  4
            {136.89695260071460, {298.54399685195176, 448.00100000000000}, false, 21.456}, //  5
            {128.94112598348423, {265.54201385244937, 448.00100000000000}, false, 54.458}, //  6
            {128.94112598348426, {234.45798614755063, 448.00100000000000}, false, 21.542}, //  7
            {136.89695260071460, {201.45600314804818, 448.00100000000000}, false, 54.544}, //  8
            {155.53242609963530, {161.64797926793909, 448.00100000000000}, false, 30.352}, //  9
            {193.02679246731799, {105.51786750874327, 448.00100000000000}, false, 22.4821},// 10
            {210.06213594358218, {63.999000000000000, 417.62033064981898}, true, 30.3797}, // 11
            {191.56760648448025, {63.999000000000000, 365.84488252394789}, true, 18.1551}, // 12
            {186.00100000000000, {63.999000000000000, 320.00000000000000}, true, 64},      // 13
            {191.56760648448025, {63.999000000000000, 274.15511747605211}, true, 45.8449}, // 14
            {210.06213594358218, {63.999000000000000, 222.37966935018102}, true, 33.6203}, // 15
            {248.49417422992676, {63.999000000000000, 155.21837898232343}, true, 36.7816}, // 16
            {311.06402921384853, {73.295958535878299, 63.999000000000000}, false, 9.29596},// 17
            {273.79297018449762, {152.91200629609648, 63.999000000000000}, false, 24.912}, // 18
            {257.88124462229160, {218.91597229510137, 63.999000000000000}, false, 26.916}, // 19
            {257.88124462229160, {281.08402770489874, 63.999000000000000}, false, 25.084}, // 20
            {273.79297018449762, {347.08799370390352, 63.999000000000000}, false, 27.088}, // 21
            {311.06402921384853, {426.70404146412170, 63.999000000000000}, false, 42.704}, // 22
            {264.52600814718232, {448.00100000000000, 144.58730665860242}, true, 16.5873}, // 23
            {223.61447468465050, {448.00100000000000, 216.08158350180560}, true, 24.0816}, // 24
            {203.92674045587697, {448.00100000000000, 271.19738311966836}, true, 15.1974}, // 25
    };
    ASSERT_TRUE(map.isValid());
    const Map::worldCoordinates position{250, 320};
    ASSERT_TRUE(map.isIn(position));
    Map::worldCoordinates ray{1, 0};
    Map::rayCastResult2D cast;
    auto starting = testClock::now();
    for (auto expected : expecteds) {
        cast = map.cast2DRay(position, ray);
        EXPECT_NEAR(cast.distance, expected.distance, 0.0001);
        EXPECT_NEAR(cast.wallPoint[0], expected.wallPoint[0], 0.0001);
        EXPECT_NEAR(cast.wallPoint[1], expected.wallPoint[1], 0.0001);
        EXPECT_EQ(cast.hitVertical, expected.hitVertical);
        EXPECT_NEAR(cast.hitXRatio, expected.hitXRatio, 0.0001);
        ray.rotate({360.0 / static_cast<double>(expecteds.size()), Unit::Degree});
    }
    {
        ray  = {0, 1};
        cast = map.cast2DRay(position, ray);
        EXPECT_NEAR(cast.distance, 128.001, 0.0001);
        EXPECT_NEAR(cast.wallPoint[0], 250, 0.0001);
        EXPECT_NEAR(cast.wallPoint[1], 448.001, 0.0001);
        EXPECT_EQ(cast.hitVertical, false);
    }
    {
        ray  = {0, -1};
        cast = map.cast2DRay(position, ray);
        EXPECT_NEAR(cast.distance, 256.001, 0.0001);
        EXPECT_NEAR(cast.wallPoint[0], 250, 0.0001);
        EXPECT_NEAR(cast.wallPoint[1], 63.999, 0.0001);
        EXPECT_EQ(cast.hitVertical, false);
    }
    auto duration = testClock::now() - starting;
    auto micros   = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / (static_cast<double>(expecteds.size()) + 2.0);

#ifdef NDEBUG
    const double maxDuration = 0.5;
#else
    const double maxDuration = 3.5;
#endif
    EXPECT_LE(micros, maxDuration);
}

TEST(Map, saveMap) {
    Map map = ConstructBaseMap();
    map.saveToData("test");
    rc::core::fs::DataFile testMap("maps/test.map");
    ASSERT_TRUE(testMap.exists());
    Map map2;
    map2.loadFromData("test");
    EXPECT_EQ(map2.getCellSize(), map.getCellSize());
    testMap.remove();
    EXPECT_FALSE(testMap.exists());
    testMap.remove();
    EXPECT_FALSE(testMap.exists());
}

TEST(Map, saveMapFile) {
    Map map = ConstructBaseMap();
    rc::core::fs::DataFile testMap("maps/test.map");
    ASSERT_FALSE(testMap.exists());
    map.loadFromFile(testMap.getFullPath().string());
    map.saveToFile(testMap.getFullPath().string());
    ASSERT_TRUE(testMap.exists());
    map.loadFromFile(testMap.getFullPath().string());
    EXPECT_TRUE(map.isValid());
    testMap.remove();
    EXPECT_FALSE(testMap.exists());
}

TEST(Map, PassableVisibility) {
    Map map = ConstructBaseMap();
    EXPECT_FALSE(map.isInVisible(Map::gridCoordinate{255, 255}));       // outside
    EXPECT_FALSE(map.isInVisible(Map::gridCoordinate{0, 0}));           // in a wall
    EXPECT_TRUE(map.isInVisible(Map::gridCoordinate{1, 1}));            // in a visible zone
    EXPECT_FALSE(map.isInVisible(Map::worldCoordinates{25500, 25500})); // outside
    EXPECT_FALSE(map.isInVisible(Map::worldCoordinates{1, 1}));         // in a wall
    EXPECT_TRUE(map.isInVisible(Map::worldCoordinates{100, 100}));      // in a visible zone
    EXPECT_FALSE(map.isInPassable(Map::gridCoordinate{255, 255}));      // outside
    EXPECT_FALSE(map.isInPassable(Map::gridCoordinate{0, 0}));          // in a wall
    EXPECT_TRUE(map.isInPassable(Map::gridCoordinate{1, 1}));           // in a visible zone
    EXPECT_FALSE(map.isInPassable(Map::worldCoordinates{25500, 25500}));// outside
    EXPECT_FALSE(map.isInPassable(Map::worldCoordinates{1, 1}));        // in a wall
    EXPECT_TRUE(map.isInPassable(Map::worldCoordinates{100, 100}));     // in a visible zone
}

TEST(Map, Colors) {
    rc::game::mapCell cell{true, true, 0};
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{255, 0, 255}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0, 0, 0}));
    cell.textureId = 1;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{255, 0, 255}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0, 0, 0}));
    cell.textureId = 2;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{0x60, 0x60, 0x60}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0x80, 0x80, 0x80}));
    cell.textureId = 3;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{0x40, 0x40, 0x40}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0x70, 0x70, 0x70}));
    cell.textureId = 4;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{0x19, 0x19, 0x8c}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0x39, 0x39, 0xbc}));
    cell.textureId = 5;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{0x0a, 0x0a, 0x64}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0x3a, 0x3a, 0x94}));
    cell.textureId = 6;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{0x80, 0x40, 0x10}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0xb0, 0x70, 0x40}));
    cell.textureId = 7;
    EXPECT_EQ(cell.getMapColor(), (rc::graphics::Color{0x64, 0x32, 0x08}));
    EXPECT_EQ(cell.getRayColor(), (rc::graphics::Color{0x94, 0x62, 0x38}));
}

TEST(Map, possibleMove) {
    Map map = ConstructBaseMap();
    {
        Map::worldCoordinates expected{5, 0};
        auto result = map.possibleMove(Map::worldCoordinates{100, 100}, expected);
        EXPECT_NEAR((expected - result).length(), 0, 0.001);
    }
    {// X movement possible, not Y
        Map::worldCoordinates expected{5, -50};
        auto result = map.possibleMove(Map::worldCoordinates{100, 100}, expected);
        Map::worldCoordinates expectedResult{5, 0};
        EXPECT_NEAR((expectedResult - result).length(), 0, 0.001);
    }
    {// Y movement possible, not X
        Map::worldCoordinates expected{-50, 5};
        auto result = map.possibleMove(Map::worldCoordinates{100, 100}, expected);
        Map::worldCoordinates expectedResult{0, 5};
        EXPECT_NEAR((expectedResult - result).length(), 0, 0.001);
    }
    {// movement impossible
        Map::worldCoordinates expected{-50, -50};
        auto result = map.possibleMove(Map::worldCoordinates{100, 100}, expected);
        Map::worldCoordinates expectedResult{0, 0};
        EXPECT_NEAR((expectedResult - result).length(), 0, 0.001);
    }
}

TEST(Map, castRay3D) {
    Map map = ConstructBaseMap();
    Map::world3DCoordinates position{map.fullWidth()/2.0,map.fullHeight()/2.0, map.getCellSize()/2.0};
    auto result = map.cast3DRay(position,{0,1,-0.5});
    EXPECT_EQ(result.hit, true);
    EXPECT_EQ(result.hCell, Map::gridCoordinate(4,5));
    EXPECT_EQ(result.texUV, Map::worldCoordinates(0,0));
    position += {map.getCellSize()/2.0,map.getCellSize()/2.0,0};
    result = map.cast3DRay(position,{0,1,-0.5});
    EXPECT_EQ(result.hit, true);
    EXPECT_EQ(result.hCell, Map::gridCoordinate(4,5));
    EXPECT_EQ(result.texUV, Map::worldCoordinates(0.5,0.5));
    position += {static_cast<double>(-map.getCellSize()),0,0};
    result = map.cast3DRay(position,{1,1,-0.5});
    EXPECT_EQ(result.hit, true);
    EXPECT_EQ(result.hCell, Map::gridCoordinate(4,5));
    EXPECT_EQ(result.texUV, Map::worldCoordinates(0.5,0.5));
    position = {map.getCellSize()/2.0,map.getCellSize()/2.0,map.getCellSize()/2.0};
    result = map.cast3DRay(position,{4,5,-0.5});
    EXPECT_EQ(result.hit, true);
    EXPECT_EQ(result.hCell, Map::gridCoordinate(4,5));
    EXPECT_EQ(result.texUV, Map::worldCoordinates(0.5,0.5));
}
