
#include "game/Player.h"
#include "testHelper.h"

using Player=rc::game::Player;
using Unit=rc::math::geometry::Angle::Unit;

TEST(Player, base){
    Player player;
    EXPECT_EQ(player.getPosition()[0],0);
    EXPECT_EQ(player.getPosition()[1],0);
    EXPECT_EQ(player.getDirection()[0],1);
    EXPECT_EQ(player.getDirection()[1],0);
    player.setPosition({50,64});
    player.setDirection({5,10});
    EXPECT_NEAR(player.getPosition()[0],50,0.00001);
    EXPECT_NEAR(player.getPosition()[1],64,0.00001);
    EXPECT_NEAR(player.getDirection()[0],0.447214,0.00001);
    EXPECT_NEAR(player.getDirection()[1],0.894427,0.00001);
}

TEST(Player, Movement){
    Player player;
    player.setDirection({1,0});
    player.move(player.getDirection() * 50);
    EXPECT_EQ(player.getPosition()[0],50);
    EXPECT_EQ(player.getPosition()[1],0);
    player.rotate({-90.0, Unit::Degree});
    player.move(player.getDirection() * 50);
    player.rotate({-90.0, Unit::Degree});
    player.move(player.getDirection() * 50);

    EXPECT_NEAR(player.getPosition()[0],0,0.00001);
    EXPECT_NEAR(player.getPosition()[1],-50,0.00001);
}
