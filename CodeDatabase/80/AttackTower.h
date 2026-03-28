#ifndef ATTACKTOWER_H
#define ATTACKTOWER_H

#include "Entity.h"
#include "SpellDirectDamage.h"
#include <memory>
#include <cstdint>
#include <string>

class AttackTower : public Entity {
private:
    int attack_range;
    int attack_cooldown;
    int current_cooldown;
    std::unique_ptr<SpellDirectDamage> attack_spell;
    static const size_t SAVE_SIZE = 4 * sizeof(int) + sizeof(int);

public:
    AttackTower();
    void Slowed() override;
    void Spawn(int x, int y, Map& map) override;
    void Move(Map& map) override;
    void AttackIfPlayerInRange(Map& map);
    std::vector<uint8_t> Save() const override;
    void Load(const std::vector<uint8_t>& data) override;
    size_t GetSaveSize() const override;
    std::string SaveToString() const override;
    void LoadFromString(const std::string& data) override;
    std::string GetSaveHeader() const override;

    int GetCurrentCooldown() const { return current_cooldown; }
    void SetCurrentCooldown(int value) { current_cooldown = value; }
    int GetAttackRange() const { return attack_range; }
    int GetDamage() const { return damage; }

};

#endif