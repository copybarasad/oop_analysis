#ifndef PLAYER_H
#define PLAYER_H

#include "character.h"
#include "spell.h"
#include <vector>
#include <memory>
#include <utility>

class Player : public Character {
public:
    Player(int health = 5, int damage = 5, int max_spells = 10);
    ~Player() = default;
    Player(const Player&) = delete;
    Player& operator=(const Player&) = delete;
    Player(Player&&) noexcept;
    Player& operator=(Player&&) noexcept;
    std::pair<int,int> GetPosition() const;
    void SetPosition(int r, int c);
    void SwitchCombatMode();
    bool IsRanged() const;
    int CurrentDamage() const;
    void SetSkipNextMove(bool v);
    bool ShouldSkipMove() const;
    void ConsumedSkipMove();
    int GetEnhancementStacks() const;
    int GetMaxEnhancementStacks() const;
    void AddEnhancementStack();
    void UpgradeEnhancementStack();
    void ConsumeEnhancementStack();
    void SetEnhancementStacks(int stacks);
    void SetMaxEnhancementStacks(int max_stacks);
    bool AddSpell(std::unique_ptr<Spell> s);
    int GetSpellCount() const;
    const Spell* GetSpell(int idx) const;
    bool CastSpell(int idx, Field& field, const std::pair<int,int>& target, std::vector<Enemy>& enemies, std::vector<EnemyTower>& towers, std::vector<EnemyBase>& bases);
    void RemoveSpellAt(int idx);
    void LevelUp();
    void UpgradeHealth();
    void UpgradeDamage();
    void UpgradeMaxSpells();
    void DecreaseMaxSpells();
    int GetMaxSpells() const;

    const std::vector<std::unique_ptr<Spell>>& GetSpells() const { return spells_list_; }

private:
    bool is_ranged_;
    bool skip_move_;
    std::pair<int,int> pos_;
    std::vector<std::unique_ptr<Spell>> spells_list_;
    int max_spells_;
    int enhancement_stacks_;
    int max_enhancement_stacks_;
};

#endif