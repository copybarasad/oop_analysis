#pragma once
#include "entity.hpp"
#include <string>
#include <vector>
#include <iostream>
#include "fieldcell.hpp"
#include "hand.hpp"

class Field;
class EntityManager;
class Fireball;
class Sunstrike;
class TrapSpell;
class AllySpell;
class UpgradeSpell;

enum Weapon { SWORD, BOW };
enum Damage { sword = 100, bow = 50 };
enum Distance { low = 1, high = 6 };

class Hero : public Entity {
public:
    Weapon weapon = SWORD;
    int mana = 0;
    int max_health = 150;
    int sword_damage = 100;
    int bow_damage = 50;
    int upgrade_level = 0;
    Hand handle;
    bool skip_turn = false;
    Distance atack_distance = low;
    
    Hero();
    Hero(const Hero&) = default;
    Hero& operator=(const Hero&) = default;
    Hero(Hero&&) = default;
    Hero& operator=(Hero&&) = default;
    
    void swap_weapon();
    int get_hp() const;
    bool canMoveTo(const std::vector<std::vector<FieldCell>>& box, int x, int y) const;
    std::string get_weapon_name() const;
    int move(std::vector<std::vector<FieldCell>>& box, int& x, int& y, char action);
    int get_mana() const;
    void waste_mana(int cost);
    void take_mana(int cost);
    void get_spell();
    void show_spell_shop();
    bool use_spell(int spell_index, Field& field, EntityManager& roster, int tx, int ty);
    std::unique_ptr<Spell> create_spell_by_id(int id);
    int get_current_damage() const { return (weapon == SWORD) ? sword_damage : bow_damage;}
};