#include <gtest/gtest.h>
#include "Data/player.hpp"
#include "Data/level.hpp"
#include "Data/game.hpp"
#include <iostream>
#include <vector>

class LevelTest: public testing::Test {
  protected:
  LevelTest(){
    g = Game();
    g.LoadLevel(test_level_path);
    l = g.GetLevel();
  }
  Game g;
  Level l;
  std::string test_level_path = "../resources/maps/text_level.txt";
};

TEST_F(LevelTest, LoadingLevel) {

  Level l = g.GetLevel();
  
  std::string exit_name = l.GetExitLegend()['X'];
  EXPECT_TRUE(exit_name == "resources/maps/second.txt") << "On load 'X' exit is 'resources/maps/second.txt'." << " Was " << exit_name << std::endl;

  std::string texture_name = l.GetTextureLegend()['X'];
  EXPECT_TRUE(texture_name == "resources/textures/surfaces/dipsa.jpg") << "On load 'X' texture is 'resources/textures/surfaces/dipsa.jpg'." << " Was " << texture_name << std::endl;

  EXPECT_TRUE(l.GetBGM() == "umg_8k") << "On init bgm should be set to 'umg_8k'." << " Was " << l.GetBGM() << std::endl;

  std::vector<std::string> level_map = l.GetMap();
  EXPECT_TRUE(!level_map.empty()) << "Map should not be empty" << " Was empty.";
  EXPECT_TRUE(!level_map[0].empty()) << "Map line should not be empty" << " Was empty.";
  EXPECT_TRUE(level_map[1][7]=='0') << "Should be 0" << " Was " << level_map[1][8];
  };


TEST_F(LevelTest, PathfindingInLevel) {
  g.LoadLevel(test_level_path);
  Level l = g.GetLevel();
  std::queue<std::pair<int, int>> q = l.ShortestPathFrom(std::pair<int, int>(2, 3), std::pair<int, int>(7, 6));
  EXPECT_TRUE(!q.empty()) << " Path should not be empty. " << "Was empty.";
  std::vector<std::pair<int,int>> q_answer = {
    std::pair<int,int>(1, 3),
    std::pair<int,int>(1, 4),
    std::pair<int,int>(1, 5),
    std::pair<int,int>(2, 5),
    std::pair<int,int>(3, 5),
    std::pair<int,int>(4, 5),
    std::pair<int,int>(5, 5),
    std::pair<int,int>(6, 5),
    std::pair<int,int>(7, 5),
    std::pair<int,int>(7, 6)
  };

  for (auto point : q_answer) {
      std::pair<int, int> qp = q.front();
      EXPECT_TRUE(point == qp) << "Path should have correct points. " << qp.first << ", " << qp.second << " != " << point.first << ", " << point.second;
      q.pop();
  }
};

class PlayerTest: public testing::Test {
  protected:
    PlayerTest(){
      Player p = Player();
      std::string w1_name = "TestWeapon";
      std::string w2_name = "NotTestWeapon";
      Weapon w1 = Weapon(w1_name, 1, 1, 1, 1, 1.0f);
      Weapon w2 = Weapon(w2_name, 30, 30, 30, 1, 1.0f);
    }
    Player p;
    std::string w1_name;
    std::string w2_name;
    Weapon w1;
    Weapon w2;
    
};

TEST_F(PlayerTest, AddingWeapons) {
  EXPECT_TRUE(p.GetCurrentWeapon()->GetName() == "Fist") << "On init player should only have fist.";
  p.AddWeapon(w1);
  ASSERT_TRUE(p.GetCurrentWeapon()->GetName() == w1_name) << "Player should have only " << w1_name;
  p.AddWeapon(w2);
  ASSERT_EQ(p.GetWeapons().size(), 2) << "Player should have 2 weapons.";
  EXPECT_TRUE(p.GetWeapons()[0]->GetName() == w1_name) << "Weapons at index 0 should be " << w1_name;
  EXPECT_TRUE(p.GetWeapons()[1]->GetName() == w2_name) << "Weapons at index 1 should be " << w2_name;
}

TEST_F(PlayerTest, RemovingWeapons){
  p.AddWeapon(w1);
  p.AddWeapon(w2);
  p.RemoveWeapon(1);
  ASSERT_EQ(p.GetWeapons().size(), 1) << "Player should have 1 weapon";
  EXPECT_TRUE(p.GetCurrentWeapon()->GetName() == w2_name) << "Remaining weapon should be " << w2_name;
}

TEST_F(PlayerTest, SwitchingWeapons){
  p.AddWeapon(w1);
  p.AddWeapon(w2);
  p.ChangeWeapon(2);
  ASSERT_TRUE(p.GetCurrentWeapon()->GetName() == w2_name) << "Weapon on slot 2 should be " << w2_name;
  p.ChangeWeapon(1);
  ASSERT_TRUE(p.GetCurrentWeapon()->GetName() == w1_name) << "Weapon on slot 1 should be " << w1_name;
  p.ChangeWeapon(3);
  EXPECT_TRUE(p.GetCurrentWeapon()->GetName() == w2_name) << "Trying to access OOB should return highest slot";
  p.NextWeapon();
  EXPECT_TRUE(p.GetCurrentWeapon()->GetName() == w1_name) << "NextWeapon() should work correctly";
}