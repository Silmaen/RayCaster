#include "graphics/Quad2.h"
#include "testHelper.h"

using Quad2=rc::graphics::Quad2<double>;

TEST(Quad2, base){
    Quad2 quad;
    EXPECT_EQ(quad[0][0], 0);
    EXPECT_EQ(quad[0][1], 0);
    EXPECT_EQ(quad[1][0], 0);
    EXPECT_EQ(quad[1][1], 0);
    EXPECT_EQ(quad[2][0], 0);
    EXPECT_EQ(quad[2][1], 0);
    EXPECT_EQ(quad[3][0], 0);
    EXPECT_EQ(quad[3][1], 0);
    const Quad2 quad2{{0,1},{2,3},{4,5},{6,7}};
    EXPECT_EQ(quad2[0][0], 0);
    EXPECT_EQ(quad2[0][1], 1);
    EXPECT_EQ(quad2[1][0], 2);
    EXPECT_EQ(quad2[1][1], 3);
    EXPECT_EQ(quad2[2][0], 4);
    EXPECT_EQ(quad2[2][1], 5);
    EXPECT_EQ(quad2[3][0], 6);
    EXPECT_EQ(quad2[3][1], 7);
}

TEST(Quad2, copy){
    Quad2 quad1{{0,1},{2,3},{4,5},{6,7}};
    Quad2 quad2{{1,2},{3,4},{5,6},{7,7}};
    EXPECT_EQ(quad2[0][0], 1);
    quad2 = quad1;
    EXPECT_EQ(quad2[0][0], 0);
    EXPECT_EQ(quad1[0][0], 0);
    quad1[0][0] = 78;
    quad2 = std::move(quad1);
    EXPECT_EQ(quad2[0][0], 78);
    Quad2 quad3{std::move(quad2)};
    EXPECT_EQ(quad3[0][0], 78);
}

TEST(Quad2, moves){
    using Vectf = rc::math::Vectf;
    Vectf vect{45,78};
    Quad2 quad{vect,vect + Vectf{0,10},vect + Vectf{10,10},vect + Vectf{10,0}};
    quad.move({15,12});
    EXPECT_NEAR(quad[2][0],70,0.001);
    EXPECT_NEAR(quad[2][1],100,0.001);
    quad.moveTo(vect);
    EXPECT_NEAR(quad[2][0],55,0.001);
    EXPECT_NEAR(quad[2][1],88,0.001);
}
