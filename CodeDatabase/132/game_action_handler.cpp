#include "game_action_handler.hpp"
#include <iostream>

GameActionHandler::GameActionHandler(FieldHandler& field_handler): 
    field_handler(field_handler), 
    attack_handler(field_handler), 
    spell_handler(field_handler) 
{}

void GameActionHandler::handle_object_action(
    const Position& pos, 
    const std::set<char>& attack_cells, 
    const std::set<char>& step_cells
) 
{
    GameObject* obj = field_handler.get_obj(pos);

    bool is_alive = true;
    if (auto* entity = dynamic_cast<IEntity*>(obj)) {
        entity->apply_effects();
        is_alive = entity->is_alive();
    }

    if (!is_alive) {
        field_handler.remove_obj(pos);
        return;
    }

    const Field& field = field_handler.get_field();

    bool move_done = false;

    Move move = get_object_move(
        pos, 
        field.filter(attack_cells), 
        field.filter({pos.x - 1, pos.y - 1}, {pos.x + 1, pos.y + 1}, step_cells), 
        field_handler.get_nearest(pos, attack_cells)
    );

    if (move.type == MoveType::MOVE) {
        move_done = handle_movement(move, obj);
    } else if (move.type == MoveType::ATTACK) {
        move_done = handle_attack(move, obj);
    } else if (move.type == MoveType::SPELL) {
        move_done = handle_spell(move, obj);
    } else if (move.type == MoveType::CHANGE_WEAPON) {
        move_done = handle_change_weapon(move, obj);
    }
}

bool GameActionHandler::handle_change_weapon(const Move& move, GameObject* armed) {
    if (IArmed* armed_entity = dynamic_cast<IArmed*>(armed)) {
        armed_entity->change_weapon();
        return true;
    }
    else {
        std::string err = "Данный игровой объект не имеет оружия: ";
        throw std::invalid_argument(err + armed->get_screen_symbol());
    }
}

bool GameActionHandler::handle_movement(const Move& move, GameObject* movable) {
    if (move.to == NO_POSITION || move.to == move.from) {
        std::cout << "Ход не может быть сделан, пропускается для: " << move.from.x << ' ' << move.from.y << std::endl;
        return false;
    }

    GameObject* cell = field_handler.get_obj(move.to);

    if (MovableEntity* movable_entity = dynamic_cast<MovableEntity*>(movable)) {
        if (!movable_entity->is_move_available()) return false;

        if (cell != nullptr) {
            if (Landscape* land_cell = dynamic_cast<Landscape*>(cell)) {
                movable_entity->add_effects(land_cell->step());
            }
        }

        field_handler.move_obj(move.from, move.to);
    }
    else {
        std::string err = "Данный игровой объект не может перемещаться: ";
        throw std::invalid_argument(err + movable->get_screen_symbol());
    }

    return true;
}

bool GameActionHandler::handle_attack(const Move& move, GameObject* attacker) {    
    if (auto* armed = dynamic_cast<IArmed*>(attacker)) {
        Weapon weapon = armed->get_current_weapon();
        return attack_handler.attack(move.to, weapon);
    }
    else {
        std::string err = "Данный игровой объект не имеет оружия: ";
        throw std::invalid_argument(err + attacker->get_screen_symbol());
    }

    return false;
}

bool GameActionHandler::handle_spell(const Move& move, GameObject* caster) {
    if (auto* has_hand = dynamic_cast<IHasHand*>(caster)) {
        if (move.to == NO_POSITION || move.spell_index == -1) {
            std::cout << "Заклинание не может быть применено, ход пропускается для: " << caster->get_screen_symbol() << std::endl;
            return true;
        }

        auto spell = has_hand->get_spell(move.spell_index);

        bool used = spell_handler.cast_spell( 
            move.from, 
            move.to,
            spell
        );

        if (!used) {
            has_hand->add_spell(spell);
        }
        else {
            if (Building* building = dynamic_cast<Building*>(caster)) {
                building->add_spell(spell);
                building->reset_timer();
            }
        }

        if (used) return true;
    }
    else {
        std::string err = "Данный игровой объект не поддерживает заклинания: ";
        throw std::invalid_argument(err + caster->get_screen_symbol());
    }

    return false;
}

Move GameActionHandler::get_object_move(
    const Position& pos, 
    const std::vector<Position>& attack_cells, 
    const std::vector<Position>& step_cells, 
    const Position& nearest
) 
{
    GameObject* obj = field_handler.get_obj(pos);

    if (auto* entity = dynamic_cast<IEntity*>(obj)) {
        return entity->choose_move(step_cells, attack_cells, pos, nearest);
    }
    
    return Move{};
}