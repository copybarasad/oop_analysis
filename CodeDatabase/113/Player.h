#ifndef PLAYER_H
#define PLAYER_H

#include "PlayerStats.h"
#include "CombatSystem.h"
#include "MovementSystem.h"
#include "Hand.h"
#include "IAttacker.h"
#include <vector>
#include <stdexcept>
#include <iostream>

class Enemy;
class EnemyTower;
class GameField;
class Spell;

class Player : public IAttacker {
private:
    PlayerStats m_stats;
    CombatSystem m_combat;
    MovementSystem m_movement;
    Hand m_hand;
    std::vector<Enemy>* m_enemies;
    std::vector<EnemyTower>* m_towers;

public:
    int getMaxHealth() const { return m_stats.getMaxHealth(); }
    // Конструктор с 4 параметрами
    Player(int startHealth, int meleeDamage, int rangedDamage, int startMana)
        : m_stats(startHealth, startMana),
        m_combat(meleeDamage, rangedDamage),
        m_movement(0, 0),
        m_hand(5),
        m_enemies(nullptr),
        m_towers(nullptr) {}

    Player(int startHealth, int meleeDamage, int rangedDamage, int startMana, int handSize)
        : m_stats(startHealth, startMana),
        m_combat(meleeDamage, rangedDamage),
        m_movement(0, 0),
        m_hand(handSize),
        m_enemies(nullptr),
        m_towers(nullptr) {}

    bool addSpellToHand(std::unique_ptr<Spell> spell) {
        if (!spell) {
            std::cout << "Cannot add null spell to hand!" << std::endl;
            return false;
        }

        // Сохраняем имя ДО перемещения
        std::string spellName = spell->getName();
        bool success = m_hand.addSpell(std::move(spell));

        if (success) {
            std::cout << "Added " << spellName << " to hand!" << std::endl;
        }
        else {
            std::cout << "Hand is full! Cannot add " << spellName << std::endl;
        }

        return success;
    }

    template<typename SpellType, typename... Args>
    bool createAndAddSpell(Args&&... args) {
        try {
            auto spell = std::make_unique<SpellType>(std::forward<Args>(args)...);
            return addSpellToHand(std::move(spell));
        }
        catch (const std::exception& e) {
            std::cout << "Error creating spell: " << e.what() << std::endl;
            return false;
        }
    }

    int getDamage() const override { return m_combat.getCurrentDamage(); }
    AttackType getAttackType() const override { return m_combat.getCurrentAttackType(); }

    int getHealth() const { return m_stats.getHealth(); }
    bool isAlive() const { return m_stats.isAlive(); }
    int getMana() const { return m_stats.getMana(); }
    int getMaxMana() const { return m_stats.getMaxMana(); }
    int getScore() const { return m_stats.getScore(); }
    void takeDamage(int amount);
    void heal(int amount);

    void setHealth(int health) {
        if (health <= 0) {
            m_stats.takeDamage(m_stats.getHealth()); // Умирает
        }
        else if (health > m_stats.getMaxHealth()) {
            m_stats.heal(m_stats.getMaxHealth() - m_stats.getHealth()); // Лечится до максимума
        }
        else {
            int currentHealth = m_stats.getHealth();
            if (health > currentHealth) {
                m_stats.heal(health - currentHealth);
            }
            else {
                m_stats.takeDamage(currentHealth - health);
            }
        }
    }

    void addScore(int points);
    void addMana(int amount);
    bool spendMana(int amount);

    void attack(std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, const GameField& field);
    void switchAttackType();

    
    bool move(int deltaX, int deltaY, GameField& field);
    int getPositionX() const { return m_movement.getPositionX(); }
    int getPositionY() const { return m_movement.getPositionY(); }
    void setPosition(int x, int y) { m_movement.setPosition(x, y); }

    
    bool castSpell(int spellIndex, GameField& field);
    Hand& getHand() { return m_hand; }
    const Hand& getHand() const { return m_hand; }

    void setEnemiesReference(std::vector<Enemy>& enemies);
    void setTowersReference(std::vector<EnemyTower>& towers);
    std::vector<Enemy>& getEnemies();
    const std::vector<Enemy>& getEnemies() const;
    std::vector<EnemyTower>& getTowers();
    const std::vector<EnemyTower>& getTowers() const;

    bool areReferencesSet() const { return m_enemies != nullptr && m_towers != nullptr; }

    struct DamageInfo {
        int currentDamage;
        int meleeBase;
        int rangedBase;
    };
    DamageInfo getDamageInfo() const;
};

#endif // PLAYER_H