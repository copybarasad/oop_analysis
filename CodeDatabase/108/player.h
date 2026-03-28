#ifndef OOP_PLAYER_H
#define OOP_PLAYER_H

#include "entity.h"
#include "player_hand.h"

enum class CombatMode { MELEE, RANGED };

class Player : public Entity {
    int score;

    CombatMode combat_mode;
    int melee_damage;
    int ranged_damage;
    int melee_range;
    int ranged_range;

    bool is_slowed;
    int slow_cooldown;

    // NEW (lb2)
    SpellHand spell_hand;

public:

    explicit Player(std::string name, const Position& start_position, int hp = 200, int melee_dmg = 15, int range_dmg = 10);

    void move(Direction direction, GameState& game_state);
    bool attack_nearest_target(GameState& game_state);                  

    void switch_combat_mode();
    void add_score(int points);

    void apply_slow_effect();
    void update_effects();


    // getters
    int get_score() const;
    CombatMode get_combat_mode() const;
    int get_current_damage() const;
    int get_attack_range() const;
    int get_melee_damage() const;
    int get_ranged_damage() const;
    int get_melee_range() const;
    int get_ranged_range() const;
    bool get_is_slowed_info() const;

    bool can_attack_target(const Position& target_position) const;


    //setters for loading
    void set_score(int new_score);
    void set_melee_damage(int melee_dmg);
    void set_ranged_damage(int ranged_dmg);
    void set_combat_mode(CombatMode mode);
    void set_melee_range(int new_range);
    void set_ranged_range(int new_range);



    // ИСПРАВЛЕНО: методы дублируют поведение руки, можно вызывать через руку просто через get_spell_hand() -> разделение ответственности
    // void add_spell(ISpell* spell);
    // void cast_spell(int index, GameState& game_state);
    // size_t get_spell_count() const;
    // std::string get_spell_name(int index) const;


    // ДОБАВЛЕНО (вместо) + вызываем через этот метод:
    SpellHand& get_spell_hand();
    const SpellHand& get_spell_hand() const;


    // ВЫНЕСЕНО В РУКУ, тк логически принадлежит ей + у игрока и так много всего
    // bool can_cast_spell(int index, GameState& game_state) const;
    // void load_spells_from_names(const std::vector<std::string>& spell_names);
};

#endif //OOP_PLAYER_H