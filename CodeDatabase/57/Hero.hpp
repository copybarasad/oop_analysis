#pragma once
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include "./Systems/InterfaceGameLogger.hpp"
#include <vector>
#include "./Field/Cell.hpp"

class SpellHand;

enum Weapons {
    melee,
    ranged
};

enum Vector_range_attack {
    forward, 
    backward, 
    left, 
    right
};

class Hero : public Entity {
private:
    SpellHand* spellHand;
    Weapons weapon = melee;
    int total_kill = 0;
    int DAMAGE_RANG_ATTACK = this->atack / 2;
    int RADIUS_RANG_ATTACK = 4;

public:
    Hero(int startX, int startY, int offsetX, int offsetY, int level = 1);
    virtual ~Hero();
    
    void Switching_weapons(InterfaceGameLogger& logger_consol);
    Weapons get_weapon() { return weapon; }
    void hero_attack_range(std::vector<std::vector<Cell*>>& cells, Vector_range_attack vector, InterfaceGameLogger& logger_consol);
    bool should_attack_on_collision() override { return weapon == melee; }
    void drawSpellHand(sf::RenderWindow& window);
    SpellHand& get_SpellHand() { return *spellHand; }
    void onEnemyKilled(InterfaceGameLogger& logger, int count_kill);
    void move_towards_target(Field&, InterfaceGameLogger&) override {}
    int get_total_kills() { return total_kill; }
    void set_weapon(Weapons new_weapon) { weapon = new_weapon; }
    void setTotalKill(int CountKill) { total_kill = CountKill; }
    void update_damage_rang() { this->DAMAGE_RANG_ATTACK = this->atack / 2;}
    int get_damage_rang() { return DAMAGE_RANG_ATTACK; }
    int get_radius_rang() { return RADIUS_RANG_ATTACK; }
    int get_damage_mele() { return atack; }
    void UpgradeEnhancementSpellHero(int new_level_spells = 1);
    int getLevelEnhancementSpellHero();
};