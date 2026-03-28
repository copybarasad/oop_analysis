#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"
#include "Constants.h"
#include "SpellHand.h"
#include <memory>

class GameField;

class Player : public Entity {
public:
    Player(int startX, int startY, int health = 100, int max_mana = 50, int count_spells = 5)
        : Entity(health, startX, startY, WeaponType::SWORD),
            score_(0),
            level_(0),
            score_for_next_level_(100),
            gold_(25),
            mana_(max_mana),
            max_mana_(max_mana),
            spell_hand_(std::make_unique<SpellHand>(count_spells)) {}

    ~Player() = default;
    
    int get_score() const { return score_; }
    int get_score_for_next_lvl() const { return score_for_next_level_; }
    int get_level() const { return level_; }
    int get_gold() const { return gold_; }
    int get_mana() const { return mana_; }
    int get_max_mana() const { return max_mana_; }
    SpellHand& get_spell_hand() { return *spell_hand_; }

    void set_level(int level) { level_ = level; }
    void set_score_for_next_level(int score_for_next_level) { score_for_next_level_ = score_for_next_level; }
    void set_score(int score) { score_ = score; }
    void set_damage_multiplier(float dmg_mlt) { damage_multiplier_ = dmg_mlt; }

    bool add_score(int points);
    void upgrade_health(int amount);
    void upgrade_mana(int amount);
    void upgrade_damage(float multiplier);
    bool upgrade_spell(int spell_index);
    
    void show_level_up_menu();
    bool show_spell_upgrade_menu();

    void add_gold(int amount) { gold_ += amount; }
    bool spend_gold(int amount);
    void add_mana(int amount);

    bool is_on_slow_cell(GameField& game_field);
    
    bool move(GameField& game_field, int dx, int dy);
    bool attack(GameField& game_field, int dx, int dy);
    bool cast_spell(GameField& field, int spell_index, int target_x = -1, int target_y = -1);
    void switch_weapon(WeaponType newWeaponType);
    void restore_mana_full() { mana_ = max_mana_; }
    
    void spell_shop();
    void update(GameField& game_field);

    void show_stats() const override;

    std::string serialize() const;
    
private:
    int level_;
    int score_;
    int score_for_next_level_;
    int gold_;
    int mana_;
    int max_mana_;
    float damage_multiplier_ = 1.0f;
    std::unique_ptr<SpellHand> spell_hand_;
};

#endif