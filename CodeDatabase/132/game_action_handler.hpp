#pragma once

#include "attack_handler.hpp"
#include "spell_handler.hpp"
#include "field_handler.hpp"

#include <memory>

class GameActionHandler {
private:
    FieldHandler& field_handler;
    AttackHandler attack_handler;
    SpellHandler spell_handler;

public:
    GameActionHandler(FieldHandler& field_handler);

    void handle_object_action(const Position& pos, 
        const std::set<char>& attack_cells, 
        const std::set<char>& step_cells
    );

private:
    bool handle_movement(const Move& move, GameObject* movable);

    bool handle_attack(const Move& move, GameObject* attacker);

    bool handle_spell(const Move& move, GameObject* caster);

    bool handle_change_weapon(const Move& move, GameObject* armed);

    Move get_object_move(
        const Position& pos, 
        const std::vector<Position>& attack_cells, 
        const std::vector<Position>& step_cells, 
        const Position& nearest
    );
};