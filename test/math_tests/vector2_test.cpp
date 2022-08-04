
#include "math/Vector2.h"
#include "testHelper.h"

using veci = rc::math::Vector2<uint8_t>;

TEST(VectorInt, base) {
    {
        veci voidVect;
        EXPECT_EQ(voidVect.lengthSQ(), 0);
        EXPECT_EQ(voidVect[0], 0);
        EXPECT_EQ(voidVect[1], 0);
    }
    {
        const veci dummyVect{1,2};
        EXPECT_EQ(dummyVect.length(), 2);
        EXPECT_EQ(dummyVect[0], 1);
        EXPECT_EQ(dummyVect[1], 2);
    }
}

TEST(VectorInt, copy){
    veci vect1{3, 67};
    veci vect2{7, 76};
    EXPECT_EQ(vect2[0], 7);
    vect2 = vect1;
    EXPECT_EQ(vect2[0], 3);
    EXPECT_EQ(vect1[0], 3);
    vect1[0] = 78;
    vect2 = std::move(vect1);
    EXPECT_EQ(vect2[0], 78);
    veci vect3{std::move(vect2)};
    EXPECT_EQ(vect3[0], 78);
}

TEST(VectorInt, rotation) {
    {
        veci dummyVect{4,2};
        auto angle = dummyVect.getAngle();
        angle.get() *= -1;
        veci rot = dummyVect.rotated(angle);
        EXPECT_EQ(rot[1], 0);
    }
}

TEST(VectorInt, operation) {
    veci vect1{3, 67};
    veci vect2{7, 76};
    auto vect3 = vect1+vect2;
    EXPECT_EQ(vect3[1], 143);
    auto vect4 = (vect3 - veci{10,10}) / 2;
    EXPECT_EQ(vect4[1], 66);
    vect1 = vect4 * 4;
    EXPECT_EQ(vect1[0], 0);
    EXPECT_EQ(vect2.dot(vect3), 186);
}
