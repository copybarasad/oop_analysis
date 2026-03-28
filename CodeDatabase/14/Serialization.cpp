#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "exceptions.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "EnemyBuilding.hpp"
#include "EnemyTower.hpp"
#include "ally.hpp"
#include "trap.hpp"
#include "GameBoard.hpp"
#include "SpellFactory.hpp"


void Entity::save(std::ostream& out) const {
    out << x_coordinate << " " << y_coordinate << " ";
}

void Entity::load(std::istream& in) {
    in >> x_coordinate >> y_coordinate;
}


void Player::save(std::ostream& out) const {
    Entity::save(out);
    out << health << " " << score << " " << (int)current_attack_type << " " 
        << is_slowed << " " << kill_count << " " << enhancement_stacks << " " << damage_bonus << " ";
    
    out << player_hand->getSpellCount() << " ";
    for(size_t i = 0; i < player_hand->getSpellCount(); ++i) {
        out << player_hand->getSpell(i)->getTypeId() << " ";
    }
}

void Player::load(std::istream& in) {
    Entity::load(in);
    int type;
    in >> health >> score >> type >> is_slowed >> kill_count >> enhancement_stacks >> damage_bonus;
    current_attack_type = static_cast<AttackType>(type);

    if (in.fail()) throw LoadException("Failed to read Player stats");

    size_t count;
    in >> count;
    
    while(player_hand->getSpellCount() > 0) {
        player_hand->useSpell(0);
    }

    for(size_t i = 0; i < count; ++i) {
        int id;
        in >> id;
        Spell* s = SpellFactory::createSpell(id);
        if (s) {
            player_hand->addSpell(s);
        } else {
            throw LoadException("Unknown spell ID: " + std::to_string(id));
        }
    }
}


void Enemy::save(std::ostream& out) const {
    Entity::save(out);
    out << health << " " << damage << " ";
}

void Enemy::load(std::istream& in) {
    Entity::load(in);
    in >> health >> damage;
}

void Ally::save(std::ostream& out) const {
    Entity::save(out);
    out << health << " " << damage << " ";
}

void Ally::load(std::istream& in) {
    Entity::load(in);
    in >> health >> damage;
}


void EnemyTower::save(std::ostream& out) const {
    Entity::save(out);
    out << health << " " << damage << " " << range << " " << cooldown << " " << current_cooldown << " ";
}

void EnemyTower::load(std::istream& in) {
    Entity::load(in);
    in >> health >> damage >> range >> cooldown >> current_cooldown;
}

void EnemyBuilding::save(std::ostream& out) const {
    Entity::save(out);
    out << health << " " << turns_to_spawn << " " << initial_turns_to_spawn << " ";
}

void EnemyBuilding::load(std::istream& in) {
    Entity::load(in);
    in >> health >> turns_to_spawn >> initial_turns_to_spawn;
}

void Trap::save(std::ostream& out) const {
    Entity::save(out);
    out << damage << " ";
}

void Trap::load(std::istream& in) {
    Entity::load(in);
    in >> damage;
}


void GameBoard::save(std::ostream& out) const {
    out << width << " " << height << "\n";

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            out << (int)grid[i][j].getType() << " ";
        }
        out << "\n";
    }

    if (player) {
        out << "PLAYER "; 
        player->save(out);
        out << "\n";
    } else {
        out << "NO_PLAYER\n";
    }

    out << enemies.size() << "\n";
    for (auto* e : enemies) { e->save(out); out << "\n"; }

    out << towers.size() << "\n";
    for (auto* t : towers) { t->save(out); out << "\n"; }

    out << buildings.size() << "\n";
    for (auto* b : buildings) { b->save(out); out << "\n"; }
    
    out << allies.size() << "\n";
    for (auto* a : allies) { a->save(out); out << "\n"; }

    out << traps.size() << "\n";
    for (auto* tr : traps) { tr->save(out); out << "\n"; }
}

void GameBoard::load(std::istream& in) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int t; 
            in >> t;
            if(in.fail()) throw LoadException("Grid data corrupted at " + std::to_string(j) + "," + std::to_string(i));
            grid[i][j].setType((Cell::CellType)t);
        }
    }

    std::string tag;
    in >> tag;
    if (tag == "PLAYER") {
        if (!player) player = new Player(0, 0);
        try {
            player->load(in);
        } catch (...) {
            throw;
        }
        addPlayer(player);
    } else {
        throw LoadException("Missing PLAYER tag or data corrupted");
    }

    auto loadVectorSafe = [&](auto& /*vec*/, auto creator, auto adder, std::string entityName) {
        size_t count; 
        if (!(in >> count)) {
            throw LoadException("Failed to read count for " + entityName);
        }
        
        for (size_t i = 0; i < count; ++i) {
            try {
                auto* obj = creator();
                obj->load(in);
                
                if (in.fail()) {
                    delete obj; 
                    throw LoadException("Stream broke while reading " + entityName);
                }
                
                adder(obj);
            } 
            catch (const std::exception& e) {
                throw LoadException("Error loading " + entityName + " #" + std::to_string(i) + ": " + e.what());
            }
        }
    };

    loadVectorSafe(enemies,   [](){ return new Enemy(0,0); },         [&](Enemy* e){ addEnemy(e); },         "Enemies");
    loadVectorSafe(towers,    [](){ return new EnemyTower(0,0); },    [&](EnemyTower* t){ addTower(t); },    "Towers");
    loadVectorSafe(buildings, [](){ return new EnemyBuilding(0,0,1);},[&](EnemyBuilding* b){ addBuilding(b);},"Buildings");
    loadVectorSafe(allies,    [](){ return new Ally(0,0); },          [&](Ally* a){ addAlly(a); },           "Allies");
    loadVectorSafe(traps,     [](){ return new Trap(0,0,0); },        [&](Trap* t){ addTrap(t); },           "Traps");
}