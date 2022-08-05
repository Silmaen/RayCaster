#include "graphics/Color.h"
#include "testHelper.h"

using Color=rc::graphics::Color;

TEST(Color, base){
    Color color;
    EXPECT_EQ(color.red(), 0);
    EXPECT_EQ(color.green(), 0);
    EXPECT_EQ(color.blue(), 0);
    EXPECT_EQ(color.alpha(), 255);
    const Color color2 = {0,0,255};
    EXPECT_EQ(color2.red(), 0);
    EXPECT_EQ(color2.green(), 0);
    EXPECT_EQ(color2.blue(), 255);
    EXPECT_EQ(color2.alpha(), 255);
}

TEST(Color, copy){
    Color color1{3, 67, 48};
    Color color2{7, 76, 78};
    EXPECT_EQ(color2.red(), 7);
    color2 = color1;
    EXPECT_EQ(color2.red(), 3);
    EXPECT_EQ(color1.red(), 3);
    color1.red() = 78;
    color2 = std::move(color1);
    EXPECT_EQ(color2.red(), 78);
    Color color3{std::move(color2)};
    EXPECT_EQ(color3.red(), 78);
}

TEST(Color, doubleFloat){
    Color color = Color::fromDouble(0.3,0.3,0.3);
    EXPECT_NEAR(color.redf(), 0.3, 0.01);
    EXPECT_NEAR(color.greenf(), 0.3, 0.01);
    EXPECT_NEAR(color.bluef(), 0.3, 0.01);
    EXPECT_NEAR(color.alphaf(), 1.0, 0.01);
}

TEST(Color, compare){
    Color color1{255,255,255,255};
    Color color2{0,0,0,0};
    EXPECT_EQ(color1, color1);
    EXPECT_NE(color1, color2);
}