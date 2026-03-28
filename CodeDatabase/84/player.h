#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "spellhand.h"

class Player : public Entity {
private:
    int score_;
    int experience_;
    int enemies_defeated_;
    int mana_;
    int max_mana_;
    Weapon* current_weapon_;
    SpellHand spell_hand_;

public:
    Player(int health = 100, int x = 0, int y = 0, int hand_size = 3, int mana = 50);

    void add_score(int points);
    void add_experience(int exp);
    void add_enemy_defeated();
    int get_score() const;
    int get_experience() const;
    int get_enemies_defeated() const;
    int get_mana() const;
    int get_max_mana() const;

    bool switch_to_melee();
    bool switch_to_ranged();

    const Weapon* get_current_weapon() const;
    bool attack(Entity* target) const;
    void heal(int amount);
    
    bool learn_spell(std::unique_ptr<Spell> spell);
    bool cast_spell(int spell_index, Field& field, int target_x, int target_y);
    void print_spells() const;
    const SpellHand& get_spell_hand() const;
    bool can_cast_spell(int mana_cost) const;
    void spend_mana(int amount);
    void restore_mana(int amount);
    
    void learn_random_starting_spell();
    bool acquire_random_spell();
    bool can_buy_spell() const;
    bool buy_spell();

    void save(std::ostream& os) const override;
    void load(std::istream& is) override;

    void prepare_for_next_level();

    void upgrade_health(int amount);
    void upgrade_mana(int amount);
    void upgrade_weapon_damage(int amount);
};

#endif