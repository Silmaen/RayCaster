#include "math/geometry/Box2.h"
#include "testHelper.h"

using Box2=rc::math::geometry::Box2;

TEST(Box2, base){
    Box2 box;
    EXPECT_EQ(box.height(), 0);
    EXPECT_EQ(box.width(), 0);
    EXPECT_EQ(box.surface(), 0);
}

TEST(Box2, copy){
    Box2 box1{{0,1},{2,3}};
    Box2 box2{{1,2},{3,4}};
    EXPECT_EQ(box2[0][0], 1);
    box2 = box1;
    EXPECT_EQ(box2.left(), 0);
    EXPECT_EQ(box1.left(), 0);
    box1[0][0] = 78;
    box2 = std::move(box1);
    EXPECT_EQ(box2.left(), 78);
    Box2 box3{std::move(box2)};
    EXPECT_EQ(box3.left(), 78);
}

TEST(Box2, operations){
    Box2 box1{{0,1},{2,3}};
    Box2 box2{{1,2},{3,4}};
    EXPECT_EQ(box1, box1);
    EXPECT_EQ(box1.top(), 1);
    EXPECT_EQ(box1.right(), 2);
    EXPECT_EQ(box1.bottom(), 3);
    EXPECT_NE(box1, box2);
    EXPECT_FALSE(box1.isIn({5,4}));
    EXPECT_FALSE(box1.isIn({1,4}));
    EXPECT_FALSE(box1.isIn({5,2}));
    EXPECT_TRUE(box1.isIn({1,2}));
    EXPECT_FALSE(box1.isIn(box2));
    EXPECT_TRUE(box1.intersect(box2));
}

TEST(Box2, saveLoad){
    Box2 box1{{0,1},{2,3}};
    nlohmann::json data = box1;
    Box2 box2 = data;
    EXPECT_EQ(box1, box2);
}
