#include <gtest/gtest.h>
#include "Hand.h"
#include "Player.h"
#include "GameField.h"
#include "spells/ISpell.h"
#include "spells/DirectDamageSpell.h"
#include "spells/AreaDamageSpell.h"
#include "spells/TrapSpell.h"
#include "spells/ImproveSpell.h"
#include "spells/SummonSpell.h"

class SpellTests : public ::testing::Test {
protected:
    void SetUp() override {
        field = std::make_unique<GameField>(10, 10);
        field->getPlayer().setPosition(5, 5);
        field->getCells()[5][5].setHasEntity(true);
    }

    std::unique_ptr<GameField> field;
};

TEST_F(SpellTests, HandAddSpell) {
    Hand hand(5);
    
    EXPECT_TRUE(hand.addSpell(SpellType::DIRECT_DAMAGE));
    EXPECT_EQ(hand.getSpells().size(), 1);
    EXPECT_EQ(hand.getSpells()[0], SpellType::DIRECT_DAMAGE);
}

TEST_F(SpellTests, HandAddMultipleSpells) {
    Hand hand(5);
    
    EXPECT_TRUE(hand.addSpell(SpellType::DIRECT_DAMAGE));
    EXPECT_TRUE(hand.addSpell(SpellType::AREA_DAMAGE));
    EXPECT_TRUE(hand.addSpell(SpellType::TRAP));
    
    EXPECT_EQ(hand.getSpells().size(), 3);
    EXPECT_EQ(hand.getSpells()[0], SpellType::DIRECT_DAMAGE);
    EXPECT_EQ(hand.getSpells()[1], SpellType::AREA_DAMAGE);
    EXPECT_EQ(hand.getSpells()[2], SpellType::TRAP);
}

TEST_F(SpellTests, HandAddSpellOverLimit) {
    Hand hand(2);
    
    EXPECT_TRUE(hand.addSpell(SpellType::DIRECT_DAMAGE));
    EXPECT_TRUE(hand.addSpell(SpellType::AREA_DAMAGE));
    EXPECT_FALSE(hand.addSpell(SpellType::TRAP));
    
    EXPECT_EQ(hand.getSpells().size(), 2);
}

TEST_F(SpellTests, HandRemoveSpell) {
    Hand hand(5);
    hand.addSpell(SpellType::DIRECT_DAMAGE);
    hand.addSpell(SpellType::AREA_DAMAGE);
    hand.addSpell(SpellType::TRAP);
    
    hand.removeSpell(1);
    
    EXPECT_EQ(hand.getSpells().size(), 2);
    EXPECT_EQ(hand.getSpells()[0], SpellType::DIRECT_DAMAGE);
    EXPECT_EQ(hand.getSpells()[1], SpellType::TRAP);
}

TEST_F(SpellTests, HandRemoveSpellInvalidIndex) {
    Hand hand(5);
    hand.addSpell(SpellType::DIRECT_DAMAGE);
    
    hand.removeSpell(10);
    
    EXPECT_EQ(hand.getSpells().size(), 1);
}

TEST_F(SpellTests, HandRemoveSpellFromEmpty) {
    Hand hand(5);
    
    hand.removeSpell(0);
    
    EXPECT_EQ(hand.getSpells().size(), 0);
}

TEST_F(SpellTests, HandPendingBoostMultiplier) {
    Hand hand(5);
    
    EXPECT_EQ(hand.getPendingBoostMultiplier(), 1);
    
    hand.setPendingBoostMultiplier(3);
    EXPECT_EQ(hand.getPendingBoostMultiplier(), 3);
    
    hand.setPendingBoostMultiplier(5);
    EXPECT_EQ(hand.getPendingBoostMultiplier(), 5);
}

TEST_F(SpellTests, HandSpellCastDelay) {
    Hand hand(5);
    
    EXPECT_EQ(hand.getSpellCastDelayActive(), false);
    
    hand.setSpellCastDelayActive(true);
    EXPECT_EQ(hand.getSpellCastDelayActive(), true);
    
    hand.setSpellCastDelayActive(false);
    EXPECT_EQ(hand.getSpellCastDelayActive(), false);
}

TEST_F(SpellTests, DirectDamageSpellHasTarget) {
    DirectDamageSpell spell;
    EXPECT_TRUE(spell.getHasTarget());
}

TEST_F(SpellTests, AreaDamageSpellHasTarget) {
    AreaDamageSpell spell;
    EXPECT_TRUE(spell.getHasTarget());
}

TEST_F(SpellTests, TrapSpellNoTarget) {
    TrapSpell spell;
    EXPECT_FALSE(spell.getHasTarget());
}

TEST_F(SpellTests, ImproveSpellNoTarget) {
    ImproveSpell spell;
    EXPECT_FALSE(spell.getHasTarget());
}

TEST_F(SpellTests, SummonSpellNoTarget) {
    SummonSpell spell;
    EXPECT_FALSE(spell.getHasTarget());
}

TEST_F(SpellTests, PlayerHandInitialization) {
    Player player(100, 20, 15);
    
    EXPECT_EQ(player.getHand().getMaxSpellsCount(), 5);
    // Игрок создается с одним случайным заклинанием
    EXPECT_EQ(player.getHand().getSpells().size(), 1);
}

TEST_F(SpellTests, PlayerHandAddSpell) {
    Player player(100, 20, 15);
    
    // Игрок уже имеет одно случайное заклинание
    size_t initialSize = player.getHand().getSpells().size();
    EXPECT_TRUE(player.getHand().addSpell(SpellType::DIRECT_DAMAGE));
    EXPECT_EQ(player.getHand().getSpells().size(), initialSize + 1);
}

TEST_F(SpellTests, HandAllSpellTypes) {
    Hand hand(10);
    
    EXPECT_TRUE(hand.addSpell(SpellType::DIRECT_DAMAGE));
    EXPECT_TRUE(hand.addSpell(SpellType::AREA_DAMAGE));
    EXPECT_TRUE(hand.addSpell(SpellType::TRAP));
    EXPECT_TRUE(hand.addSpell(SpellType::IMPROVE));
    EXPECT_TRUE(hand.addSpell(SpellType::SUMMON));
    
    EXPECT_EQ(hand.getSpells().size(), 5);
    EXPECT_EQ(hand.getSpells()[0], SpellType::DIRECT_DAMAGE);
    EXPECT_EQ(hand.getSpells()[1], SpellType::AREA_DAMAGE);
    EXPECT_EQ(hand.getSpells()[2], SpellType::TRAP);
    EXPECT_EQ(hand.getSpells()[3], SpellType::IMPROVE);
    EXPECT_EQ(hand.getSpells()[4], SpellType::SUMMON);
}

TEST_F(SpellTests, HandMaxSpellsCount) {
    Hand hand(3);
    
    EXPECT_EQ(hand.getMaxSpellsCount(), 3);
    
    hand.setMaxSpellsCount(7);
    EXPECT_EQ(hand.getMaxSpellsCount(), 7);
}

TEST_F(SpellTests, HandRemoveAllSpells) {
    Hand hand(5);
    hand.addSpell(SpellType::DIRECT_DAMAGE);
    hand.addSpell(SpellType::AREA_DAMAGE);
    hand.addSpell(SpellType::TRAP);
    
    hand.removeSpell(0);
    hand.removeSpell(0);
    hand.removeSpell(0);
    
    EXPECT_EQ(hand.getSpells().size(), 0);
}

TEST_F(SpellTests, HandRemoveFirstSpell) {
    Hand hand(5);
    hand.addSpell(SpellType::DIRECT_DAMAGE);
    hand.addSpell(SpellType::AREA_DAMAGE);
    hand.addSpell(SpellType::TRAP);
    
    hand.removeSpell(0);
    
    EXPECT_EQ(hand.getSpells().size(), 2);
    EXPECT_EQ(hand.getSpells()[0], SpellType::AREA_DAMAGE);
    EXPECT_EQ(hand.getSpells()[1], SpellType::TRAP);
}

TEST_F(SpellTests, HandRemoveLastSpell) {
    Hand hand(5);
    hand.addSpell(SpellType::DIRECT_DAMAGE);
    hand.addSpell(SpellType::AREA_DAMAGE);
    hand.addSpell(SpellType::TRAP);
    
    hand.removeSpell(2);
    
    EXPECT_EQ(hand.getSpells().size(), 2);
    EXPECT_EQ(hand.getSpells()[0], SpellType::DIRECT_DAMAGE);
    EXPECT_EQ(hand.getSpells()[1], SpellType::AREA_DAMAGE);
}

