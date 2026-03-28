#ifndef HERO_H
#define HERO_H

#include "Character.hpp"
#include "AttackType.hpp"
#include "SpellCaster.hpp"
#include <memory>

class SpellHand;
class GameState;

class Hero : public Character {
private:
    int meleeDamage_;
    int rangedDamage_;
    int score_;
    int mana_;
    int maxMana_;
    AttackType currentAttackType_;
    bool isSlowed_;
    std::unique_ptr<SpellHand> spellHand_;
    int enhancementCount_;
    int rangedRange_;

    class HeroSpellCaster : public SpellCaster {
    private:
        const Hero& hero_;
    public:
        HeroSpellCaster(const Hero& hero) : hero_(hero) {}
        int getPositionX() const override { return hero_.getPositionX(); }
        int getPositionY() const override { return hero_.getPositionY(); }
    };

public:
    Hero();
    Hero(int startX, int startY, int health = 100, int meleeDamage = 20, 
         int rangedDamage = 10, int mana = 100, int maxMana = 100, int rangedRange = 3);
    Hero(const Hero& other);
    Hero& operator=(const Hero& other); 
    Hero(Hero&& other) noexcept;
    Hero& operator=(Hero&& other) noexcept;
    virtual ~Hero();

    int getDamage() const override;
    bool isSlowed() const;
    SpellHand& getSpellHand();
    const SpellHand& getSpellHand() const;

    void addScore(int points);
    void switchAttackType();
    void applySlowness();
    void removeSlowness();

    bool castSpell(int spellIndex, int targetX, int targetY, GameState& gameState);
    void gainMana(int amount);
    void heal(int heal);
    
    void addRandomSpell();
    void restoreMana(int amount = 10);

    void resetEnhancements();
    void addEnhancement();

    bool buySpell(int cost = 30);
    bool canBuySpell(int cost = 30) const;

    void serialize(std::ostream& os) const override;
    void deserialize(std::istream& is) override;

    int getMeleeDamage() const { return meleeDamage_; }
    int getRangedDamage() const { return rangedDamage_; }
    int getScore() const { return score_; }
    int getMana() const { return mana_; }
    int getMaxMana() const { return maxMana_; }
    AttackType getAttackType() const { return currentAttackType_; }
    bool getIsSlowed() const { return isSlowed_; }
    int getEnhancementCount() const { return enhancementCount_; }
    int getRangedRange() const { return rangedRange_; }
    
    void setMeleeDamage(int damage) { meleeDamage_ = damage; }
    void setRangedDamage(int damage) { rangedDamage_ = damage; }
    void setScore(int score) { score_ = score; }
    void setMana(int mana) { mana_ = mana; }
    void setMaxMana(int maxMana) { maxMana_ = maxMana; }
    void setAttackType(AttackType type) { currentAttackType_ = type; }
    void setIsSlowed(bool slowed) { isSlowed_ = slowed; }
    void setEnhancementCount(int count) { enhancementCount_ = count; }
    void setRangedRange(int range) { rangedRange_ = range; }
    void setSlowed(bool isSlowed) { isSlowed_ = isSlowed; }
    void setHealth(int health) { health_ = health; }
    void setMaxHealth(int maxHealth) { 
        maxHealth_ = maxHealth; 
        if (health_ > maxHealth_) {
            health_ = maxHealth_;
        }
    }
};

#endif