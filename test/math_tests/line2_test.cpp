#include "math/geometry/Line2.h"
#include "testHelper.h"

using Line2=rc::math::geometry::Line2<double>;

TEST(Line2, base){
    Line2 line;
    EXPECT_EQ(line.getPoint(0)[0], 0);
    EXPECT_EQ(line.getPoint(0)[1], 0);
    EXPECT_EQ(line.getPoint(1)[0], 0);
    EXPECT_EQ(line.getPoint(1)[1], 0);
    const Line2 line2{{0,5},{10,15}};
    EXPECT_EQ(line2.getPoint(0)[0], 0);
    EXPECT_EQ(line2.getPoint(0)[1], 5);
    EXPECT_EQ(line2.getPoint(1)[0], 10);
    EXPECT_EQ(line2.getPoint(1)[1], 15);
}

TEST(Line2, copy){
    Line2 line1{{0,5},{10,15}};
    Line2 line2{{1,6},{11,16}};
    EXPECT_EQ(line2.getPoint(0)[0], 1);
    line2 = line1;
    EXPECT_EQ(line2.getPoint(0)[0], 0);
    EXPECT_EQ(line1.getPoint(0)[0], 0);
    line1.getPoint(0)[0] = 78;
    line2 = std::move(line1);
    EXPECT_EQ(line2.getPoint(0)[0], 78);
    Line2 line3{std::move(line2)};
    EXPECT_EQ(line3.getPoint(0)[0], 78);
}

TEST(Line2, Raylength){
    Line2 line{{0,5},{10,15}, 10};
    EXPECT_EQ(line.getPoint(0)[0], 0);
    EXPECT_EQ(line.getPoint(0)[1], 5);
    EXPECT_EQ(line.getPoint(1)[0], 100);
    EXPECT_EQ(line.getPoint(1)[1], 155);
}
