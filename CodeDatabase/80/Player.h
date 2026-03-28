#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "hand.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <limits>
#include <vector>
#include <cstdint>

class GameEventManager;

class Player : public Entity {
private:
    int attack_mode;
    static const size_t SAVE_SIZE = 6 * sizeof(int) + sizeof(bool);
    Hand hand;
    int enemies_defeated;

    int maxHealth;
    int damage;
    int spellPower;

public:
    bool slowed;

    Player();

    int GetSpellCount() const;
    void RemoveRandomSpell();
    void RemoveHalfOfSpells();
    void Refresh();
    int GetDefeatedCount();

    void Slowed() override;
    bool IsSlowed();
    void Spawn(int x, int y, Map& map) override;
    void Move(Map& map) override;

    void Step(char direction, Map& map);
    void Attack(Map& map);
    void ChangeMode();
    void UseSpell(Map& map);
    void ShowSpells();

    int GetAttackMode();
    int GetMaxHealth() const { return maxHealth; }
    int GetBaseDamage() const { return damage; }

    std::vector<uint8_t> Save() const override;
    void Load(const std::vector<uint8_t>& data) override;
    size_t GetSaveSize() const override;
    std::string SaveToString() const override;
    void LoadFromString(const std::string& data) override;
    std::string GetSaveHeader() const override;

    void AddRandomSpell();
    void EnemyDefeated();

    void UpgradeMaxHealth(int amount = 10);
    void UpgradeBaseDamage(int amount = 5);
    void UpgradeRandomSpell();

    std::string GetStats() const;

    void LogCurrentState() const;

    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
};

#endif