#ifndef PLAYER_H_
#define PLAYER_H_

#include "fight_strategy.h"
#include "melee_fight.h"
#include "ranged_fight.h"
#include "hand.h"
#include <memory>
#include <string>

class FightStrategy;

class Player {
public:
    Player();
    Player(const Player& other);
    Player& operator=(const Player& other);
    ~Player();

    int GetHealth() const;
    int GetPoints() const;
    void TakeDamage(int damage);
    void AddPoints(int points);
    void SwitchFightMode();
    bool CanAttack(int distance) const;
    int GetDamage() const;
    std::string GetFightModeName() const;
    Hand& GetHand() { return *hand_; }  // ОСТАВЬТЕ ТОЛЬКО ЭТОТ ОДИН РАЗ
    void BuySpell();
    void GainSpellAfterKills();
    void KillEnemy();
    void AddBuff();
    void ApplyBuffs();
    void ResetBuffs();
    bool IsAlive() const { return health_ > 0; }
    int GetBuffRange() const { return buff_range_; }
    int GetBuffArea() const { return buff_area_; }
    int GetBuffTrapDamage() const { return buff_trap_damage_; }
    int GetBuffSummons() const { return buff_summons_; }

    int GetMaxHealth() const { return max_health_; }
    void SetMaxHealth(int mh) { max_health_ = mh; }
    void HealToMax() { health_ = max_health_; }
    int GetBaseDamage() const { return base_damage_; }
    void SetBaseDamage(int bd) { base_damage_ = bd; }
    void UpgradeHealth(int amt) { max_health_ += amt; health_ += amt; if (health_ > max_health_) health_ = max_health_; }
    void UpgradeDamage(int amt) { base_damage_ += amt; }
    int GetKillCount() const { return kill_count_; }
    void SetKillCount(int kc) { kill_count_ = kc; }
    void SetHealth(int h) { health_ = h; }
    void SetPoints(int p) { points_ = p; }

    void SetFightMode(const std::string& mode) {
        if (mode == "melee") {
            fight_strategy_ = std::make_unique<MeleeFight>();
        } else {
            fight_strategy_ = std::make_unique<RangedFight>();
        }
    }

    void SetBuffs(int range, int area, int trap_dmg, int summons) {
        buff_range_ = range;
        buff_area_ = area;
        buff_trap_damage_ = trap_dmg;
        buff_summons_ = summons;
    }

    void SetBuffRange(int r) { buff_range_ = r; }
    void SetBuffArea(int a) { buff_area_ = a; }
    void SetBuffTrapDamage(int td) { buff_trap_damage_ = td; }
    void SetBuffSummons(int s) { buff_summons_ = s; }

private:
    int max_health_ = 100;
    int base_damage_ = 0;
    int health_;
    int points_;
    std::unique_ptr<FightStrategy> fight_strategy_;
    std::unique_ptr<Hand> hand_;
    int kill_count_ = 0;
    int buff_count_;
    int buff_range_;
    int buff_area_;
    int buff_trap_damage_;
    int buff_summons_;
};

#endif // PLAYER_H_
