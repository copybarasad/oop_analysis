#include <gtest/gtest.h>
#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "Hand.h"
#include "TrapSpell.h"
#include "spells/ISpell.h"

TEST(BasicTests, CellCreation) {
    Cell cell;
    EXPECT_EQ(cell.getType(), CellType::EMPTY);
    EXPECT_EQ(cell.getHasEntity(), false);
}

TEST(BasicTests, PlayerCreation) {
    Player player;
    EXPECT_EQ(player.getX(), 0);
    EXPECT_EQ(player.getY(), 0);
    EXPECT_EQ(player.getHealth(), 100);
    EXPECT_EQ(player.getMaxHealth(), 100);
}

TEST(BasicTests, EnemyCreation) {
    Enemy enemy;
    EXPECT_EQ(enemy.getX(), 0);
    EXPECT_EQ(enemy.getY(), 0);
    EXPECT_EQ(enemy.getHealth(), 50);
    EXPECT_EQ(enemy.getMaxHealth(), 50);
}

TEST(BasicTests, TrapSpellCreation) {
    TrapSpell trapSpell;
    EXPECT_FALSE(trapSpell.getHasTarget());
}

TEST(BasicTests, HandAddSpell) {
    Hand hand(5);
    EXPECT_TRUE(hand.addSpell(SpellType::TRAP));
    EXPECT_EQ(hand.getSpells().size(), 1);
    EXPECT_EQ(hand.getSpells()[0], SpellType::TRAP);
}
