
#include "core/Map.h"
#include "core/fs/DataFile.h"
#include "testHelper.h"
#include <chrono>
using testClock = std::chrono::steady_clock;
using timePoint = testClock::time_point;
using timeDelta = testClock::duration;

using Map = rc::core::Map;

TEST(Map, base) {
    Map map;
    EXPECT_FALSE(map.isValid());
    EXPECT_EQ(map.height(), 0);
    EXPECT_EQ(map.width(), 0);
    map.setMap({{}, {}});
    EXPECT_FALSE(map.isValid());
    EXPECT_EQ(map.height(), 0);
    EXPECT_EQ(map.width(), 2);
    map.setMap({{{false,false,1}, {false,false,1}}, {{false,false,1}}});
    EXPECT_FALSE(map.isValid());
    Map::DataType data = {{{false,false,1}, {true,true,0}}, {{true,true,0}, {false,false,1}}};
    map.setMap(data);
    EXPECT_TRUE(map.isValid());
    EXPECT_EQ(map.getMapData(), data);
    EXPECT_EQ(map({0, 1}), (Map::BaseType{true,true,0}));
}

TEST(Map, init) {
    {
        const Map map({{{false,false,1}, {false,false,2}, {false,false,3}}, {{false,false,4}, {false,false,5}, {false,false,6}}, {{false,false,7}, {false,false,8}, {false,false,9}}});
        EXPECT_EQ(map({0, 1}), (Map::BaseType{false,false,4}));
        EXPECT_EQ(map({1, 2}), (Map::BaseType{false,false,8}));
    }
    {
        Map::DataType data = {{{false,false,1}, {false,false,2}, {false,false,3}}, {{false,false,4}, {false,false,5}, {false,false,6}}, {{false,false,7}, {false,false,8}, {false,false,9}}};
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
    EXPECT_TRUE(map.isIn(rc::math::Vector2<double>{50.0, 50.0}));
    EXPECT_FALSE(map.isIn(rc::math::Vector2<double>{-50.0, 50.0}));
    EXPECT_FALSE(map.isIn(rc::math::Vector2<double>{50.0, -50.0}));
    EXPECT_FALSE(map.isIn(rc::math::Vector2<double>{5000000.0, 50.0}));
    EXPECT_FALSE(map.isIn(rc::math::Vector2<double>{50.0, 50000000.0}));
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
    using vecf = rc::math::Vector2<double>;
    using Unit = rc::math::Angle::Unit;
    rc::core::mapCell walls{false, false, 1};
    rc::core::mapCell voids{true, true, 0};
    Map map{{{walls, walls, walls, walls, walls, walls, walls, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, walls, walls, walls, walls, walls, walls, walls}}};
    std::vector<Map::rayCastResult> expecteds = {
            {198.00100000000000, {448.00100000000000, 320.00000000000000}, true}, //  0
            {203.92674045587697, {448.00100000000000, 368.80286335819466}, true}, //  1
            {223.61471858970370, {448.00100000000000, 423.91894133868175}, true}, //  2
            {193.02763735984399, {394.48326125791681, 448.00100000000000}, false},//  3
            {155.53281820538129, {338.35271098222279, 448.00100000000000}, false},//  4
            {136.89708708415000, {298.54437610192713, 448.00100000000000}, false},//  5
            {128.94112598348423, {265.54213527443255, 448.00100000000000}, false},//  6
            {128.94112598348426, {234.45786472556742, 448.00100000000000}, false},//  7
            {136.89708708415003, {201.45562389807279, 448.00100000000000}, false},//  8
            {155.53281820538132, {161.64728901777715, 448.00100000000000}, false},//  9
            {193.02763735984399, {105.51673874208318, 448.00100000000000}, false},// 10
            {210.06237984860419, {63.999000000000000, 417.62085549030633}, true}, // 11
            {191.56760648448025, {63.999000000000000, 365.84512900181090}, true}, // 12
            {186.00100000000000, {63.999000000000000, 320.00000000000000}, true}, // 13
            {191.56760648448025, {63.999000000000000, 274.15487099818910}, true}, // 14
            {210.06237984860422, {63.999000000000000, 222.37914450969362}, true}, // 15
            {248.49476170445357, {63.999000000000000, 155.21749305962976}, true}, // 16
            {311.06442132011330, {73.295268285716332, 63.999000000000000}, false},// 17
            {273.79310466816264, {152.91162704612108, 63.999000000000000}, false},// 18
            {257.88124462229160, {218.91585087311816, 63.999000000000000}, false},// 19
            {257.88124462229160, {281.08414912688193, 63.999000000000000}, false},// 20
            {273.79310466816264, {347.08837295387895, 63.999000000000000}, false},// 21
            {311.06442132011330, {426.70473171428370, 63.999000000000000}, false},// 22
            {264.52659562176279, {448.00100000000000, 144.58642073590875}, true}, // 23
            {223.61471858970370, {448.00100000000000, 216.08105866131825}, true}, // 24
            {203.92674045587697, {448.00100000000000, 271.19713664180534}, true}, // 25
    };
    ASSERT_TRUE(map.isValid());
    const vecf position{250, 320};
    ASSERT_TRUE(map.isIn(position));
    vecf ray{1, 0};
    Map::rayCastResult cast;
    auto starting = testClock::now();
    for (auto expected : expecteds) {
        cast = map.castRay(position, ray);
        EXPECT_NEAR(cast.distance, expected.distance, 0.0001);
        EXPECT_NEAR(cast.wallPoint[0], expected.wallPoint[0], 0.0001);
        EXPECT_NEAR(cast.wallPoint[1], expected.wallPoint[1], 0.0001);
        EXPECT_EQ(cast.hitVertical, expected.hitVertical);
        ray.rotate({360.0 / expecteds.size(), Unit::Degree});
    }
    {
        ray  = {0, 1};
        cast = map.castRay(position, ray);
        EXPECT_NEAR(cast.distance, 128.001, 0.0001);
        EXPECT_NEAR(cast.wallPoint[0], 250, 0.0001);
        EXPECT_NEAR(cast.wallPoint[1], 448.001, 0.0001);
        EXPECT_EQ(cast.hitVertical, false);
    }
    {
        ray  = {0, -1};
        cast = map.castRay(position, ray);
        EXPECT_NEAR(cast.distance, 256.001, 0.0001);
        EXPECT_NEAR(cast.wallPoint[0], 250, 0.0001);
        EXPECT_NEAR(cast.wallPoint[1], 63.999, 0.0001);
        EXPECT_EQ(cast.hitVertical, false);
    }
    auto duration = testClock::now() - starting;
    auto micros   = static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / (expecteds.size() + 2.0);

#ifdef NDEBUG
    const double maxDuration = 0.5;
#else
    const double maxDuration = 3.5;
#endif
    EXPECT_LE(micros, maxDuration);
}

TEST(Map, saveMap) {
    rc::core::mapCell walls{false, false, 1};
    rc::core::mapCell voids{true, true, 0};
    Map map{{{walls, walls, walls, walls, walls, walls, walls, walls},
             {walls, voids, walls, voids, voids, voids, voids, walls},
             {walls, voids, walls, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, voids, voids, voids, voids, walls, voids, walls},
             {walls, voids, voids, voids, voids, voids, voids, walls},
             {walls, walls, walls, walls, walls, walls, walls, walls}}};
    map.setPlayerStart({245, 125}, {0, -1});
    map.saveToFile("test");
    rc::core::fs::DataFile testMap("maps/test.map");
    ASSERT_TRUE(testMap.exists());
    Map map2;
    map2.loadFromFile("test");
    EXPECT_EQ(map2.getCellSize(), map.getCellSize());
    testMap.remove();
    EXPECT_FALSE(testMap.exists());
    testMap.remove();
    EXPECT_FALSE(testMap.exists());
}
