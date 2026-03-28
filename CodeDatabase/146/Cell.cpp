#include "Cell.h"
#include "Player.h"
#include "Enemy.h"
#include "Building.h"
#include "Summon.h"
#include "Tower.h"
#include "EnemyBuilding.h"

CellType Cell::get_type()
{
    return type;
}

std::string Cell::is_empty()
{
    if (!object.first && !object.second) {
        return "Empty";
    }
    else if (object.first && !object.second) {
        Human* human = object.first;

        // Используем dynamic_cast для проверки типа
        if (dynamic_cast<Player*>(human)) {
            return "Player";
        }
        else if (dynamic_cast<Enemy*>(human)) {
            return "Enemy";
        }
        else if (dynamic_cast<Summon*>(human)) {
            return "Summon";
        }
    }
    else if (object.second) {
        Building* building = object.second;

        if (dynamic_cast<Tower*>(building)) {
            return "Tower";
        }
        else if (dynamic_cast<EnemyBuilding*>(building)) {
            return "EnemyBuilding";
        }
    }
    return "Unknown";
}

void Cell::set_human(Human* human)
{
    if (this->is_empty() == "Empty") {
        object.first = human;
    }
}

void Cell::set_building(Building* tower)
{
    if(this->is_empty() == "Empty") {
        object.second = tower;
    }
}

void Cell::set_type(CellType new_type)
{
    type = new_type;
}

Human* Cell::get_human()
{
    return object.first;
}

Building* Cell::get_building()
{
    return object.second;
}

bool Cell::player_or_enemy()
{
    Human* human = object.first;
    if (dynamic_cast<Player*>(human)) {
        return true;  // Player
    }
    else if (dynamic_cast<Enemy*>(human)) {
        return false; // Enemy
    }
    return false;
}


Object* Cell::get_object()
{
    std::string type = is_empty();
    if (type == "Player" || type == "Enemy" || type == "Summon") {
        return static_cast<Object*>(get_human());
    }
    else if (type == "EnemyBuilding" || type == "Tower") {
        return static_cast<Object*>(get_building());
    }
    return nullptr;
}

void Cell::set_trap(Trap* new_trap)
{
    trap = new_trap;
}

Trap* Cell::get_trap()
{
    return trap;
}

void Cell::clear_trap()
{
    delete trap;
    trap = nullptr;
}


void Cell::clear() {
    object.first = nullptr;
    object.second = nullptr;
}

std::string Cell::get_type_string() {
    switch (type) {
    case CellType::BASIC: return "Basic";
    case CellType::BLOCKED: return "Blocked";
    case CellType::SLOW: return "Slow";
    default: return "Unknown";
    }
}