#include "graphics/Quad2.h"
#include "testHelper.h"

using Quad2=rc::graphics::Quad2<double>;

TEST(Quad2, base){
    Quad2 quad;
    EXPECT_EQ(quad.getPoint(0)[0], 0);
    EXPECT_EQ(quad.getPoint(0)[1], 0);
    EXPECT_EQ(quad.getPoint(1)[0], 0);
    EXPECT_EQ(quad.getPoint(1)[1], 0);
    EXPECT_EQ(quad.getPoint(2)[0], 0);
    EXPECT_EQ(quad.getPoint(2)[1], 0);
    EXPECT_EQ(quad.getPoint(3)[0], 0);
    EXPECT_EQ(quad.getPoint(3)[1], 0);
    const Quad2 quad2{{0,1},{2,3},{4,5},{6,7}};
    EXPECT_EQ(quad2.getPoint(0)[0], 0);
    EXPECT_EQ(quad2.getPoint(0)[1], 1);
    EXPECT_EQ(quad2.getPoint(1)[0], 2);
    EXPECT_EQ(quad2.getPoint(1)[1], 3);
    EXPECT_EQ(quad2.getPoint(2)[0], 4);
    EXPECT_EQ(quad2.getPoint(2)[1], 5);
    EXPECT_EQ(quad2.getPoint(3)[0], 6);
    EXPECT_EQ(quad2.getPoint(3)[1], 7);
}

TEST(Quad2, copy){
    Quad2 quad1{{0,1},{2,3},{4,5},{6,7}};
    Quad2 quad2{{1,2},{3,4},{5,6},{7,7}};
    EXPECT_EQ(quad2.getPoint(0)[0], 1);
    quad2 = quad1;
    EXPECT_EQ(quad2.getPoint(0)[0], 0);
    EXPECT_EQ(quad1.getPoint(0)[0], 0);
    quad1.getPoint(0)[0] = 78;
    quad2 = std::move(quad1);
    EXPECT_EQ(quad2.getPoint(0)[0], 78);
    Quad2 quad3{std::move(quad2)};
    EXPECT_EQ(quad3.getPoint(0)[0], 78);
}
