#pragma once
#include <vector>
#include <SFML/Window/Keyboard.hpp> 
#include "Character.h"
#include "Hand.h"
#include "Bullet.h"             

class Game; 

class Player : public Character {
public:
    Player(int startX, int startY);
    ~Player();

    void update(float dt) override;
    void handleInput(sf::Keyboard::Key key, Game* game);
    
    void attack(Bullet::Direction dir);
    void switchAttackMode(); 
    bool isRangedMode() const { return m_isRangedMode; }
    
    void addScore(int points) { m_score += points; }
    int getScore() const { return m_score; }
    void addKill() { m_kills++; }
    int getKills() const { return m_kills; }
    
    void addToHand(Spell* spell);
    Hand& getHand();
    const Hand& getHand() const;
    void nextSpell();
    void prevSpell();
    int getSelectedSpellIndex() const;
    void addRandomSpell();
    void clearHand() { m_hand.clear(); }

    void startTargeting() { m_isTargeting = true; }
    void cancelTargeting() { m_isTargeting = false; }
    bool isTargeting() const { return m_isTargeting; }

    // Upgrades
    void upgradeMelee() { m_meleeDamage += 10; }
    void upgradeMeleeDamage() { m_meleeDamage += 10; } 
    void upgradeRanged() { m_rangedDamage += 15; }
    void upgradeRangedDamage() { m_rangedDamage += 15; } 
    void upgradeMaxHP() { m_maxHealth += 100; m_health += 100; }
    void upgradeMaxHealth() { m_maxHealth += 100; m_health += 100; } 
    
    int getMeleeDamage() const { return m_meleeDamage; }
    
    std::vector<Bullet*>& getBullets() { return m_bullets; }
    const std::vector<Bullet*>& getBullets() const { return m_bullets; }

    void setStats(int hp, int mhp, int mp, int mmp) {
        m_health = hp; m_maxHealth = mhp;
        m_mana = mp; m_maxMana = mmp;
    }

private:
    int m_score;
    int m_kills;
    bool m_isRangedMode;
    int m_meleeDamage;
    int m_rangedDamage;
    bool m_isTargeting;
    Hand m_hand;
    std::vector<Bullet*> m_bullets;
    float m_manaRegenTimer;
    float m_hpRegenTimer;
};