#include "Spells.h"
#include "Game.h"
#include "Player.h"
#include "Enhancements.h"
#include "Trap.h"
#include "Ally.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include <cmath>
#include <iostream>
#include <sstream>

DirectDamageSpell::DirectDamageSpell(int damage, int range) : base_damage_(damage), base_range_(range) {}

bool DirectDamageSpell::cast(Game& game, Entity& caster, const Point& target) {
    int damage = base_damage_;
    int range = base_range_;
    Player* player_caster = dynamic_cast<Player*>(&caster);

    if (player_caster) {
        auto& enhancements = player_caster->getEnhancements();
        damage += enhancements.extra_damage;
        range += enhancements.extra_range;
    }

    Point start_pos = caster.getPosition();
    int dx = target.x - start_pos.x;
    int dy = target.y - start_pos.y;

    if (std::sqrt(dx*dx + dy*dy) > range) {
        if (player_caster) game.notifyLog("Target is out of range.");
        return false;
    }

    Entity* target_entity = game.getEntityManager().getEntityAt(target);
    
    if (player_caster) {
        if (target_entity && (dynamic_cast<Enemy*>(target_entity) || dynamic_cast<EnemyBuilding*>(target_entity) || dynamic_cast<EnemyTower*>(target_entity))) {
            std::stringstream ss;
            ss << "You cast Direct Damage for " << damage << " damage!";
            game.notifyLog(ss.str());
            
            if (target_entity->takeDamage(damage)) {
                game.notifyLog("You defeated an enemy and gained 10 score!");
                player_caster->addScore(10);
            }
            player_caster->getEnhancements().reset();
            return true;
        } else {
            game.notifyLog("Invalid target.");
            return false;
        }
    } 
    else {
        if (target_entity && (dynamic_cast<Player*>(target_entity) || dynamic_cast<Ally*>(target_entity))) {
            std::stringstream ss;
            ss << "An enemy casts a spell on you for " << damage << " damage!";
            game.notifyLog(ss.str());
            target_entity->takeDamage(damage);
            return true;
        }
    }
    return false;
}
std::string DirectDamageSpell::getName() const { return "Direct Damage"; }
int DirectDamageSpell::getCost() const { return 15; }
SpellType DirectDamageSpell::getType() const { return SpellType::kDirect; }

AreaDamageSpell::AreaDamageSpell(int damage, int area_size) : base_damage_(damage), base_area_size_(area_size) {}

bool AreaDamageSpell::cast(Game& game, Entity& caster, const Point& target) {
    Player* player_caster = dynamic_cast<Player*>(&caster);
    if (!player_caster) return false;

    auto& enhancements = player_caster->getEnhancements();
    int current_area = base_area_size_ + enhancements.extra_area;
    int total_damage = base_damage_ + enhancements.extra_damage;
    int kills = 0;
    
    std::stringstream ss;
    ss << "Casting Area Damage spell at (" << target.x << ", " << target.y << ") with size " << current_area;
    game.notifyLog(ss.str());

    for (int y = target.y; y < target.y + current_area; ++y) {
        for (int x = target.x; x < target.x + current_area; ++x) {
            Entity* entity = game.getEntityManager().getEntityAt({x, y});
            if (entity && (dynamic_cast<Enemy*>(entity) || dynamic_cast<EnemyBuilding*>(entity) || dynamic_cast<EnemyTower*>(entity))) {
                if(entity->takeDamage(total_damage)) {
                    kills++;
                }
            }
        }
    }
    
    if (kills > 0) {
        player_caster->addScore(10 * kills);
        std::stringstream ss_kills;
        ss_kills << "You defeated " << kills << " enemy/enemies and gained " << 10 * kills << " score!";
        game.notifyLog(ss_kills.str());
    }

    enhancements.reset();
    return true;
}
std::string AreaDamageSpell::getName() const { return "Area Damage"; }
int AreaDamageSpell::getCost() const { return 25; }
SpellType AreaDamageSpell::getType() const { return SpellType::kArea; }

TrapSpell::TrapSpell(int damage) : base_damage_(damage) {}

bool TrapSpell::cast(Game& game, Entity& caster, const Point& target) {
    Player* player_caster = dynamic_cast<Player*>(&caster);
    if (!player_caster) return false;

    if (game.getEntityManager().getEntityAt(target) || game.getEntityManager().getTrapAt(target)) {
        game.notifyLog("Cannot place a trap here, the cell is occupied.");
        return false;
    }
    auto& enhancements = player_caster->getEnhancements();
    int total_damage = base_damage_ + enhancements.extra_trap_damage;
    game.getEntityManager().addTrap(std::make_unique<Trap>(target.x, target.y, total_damage));
    game.notifyLog("A trap has been set.");
    enhancements.reset();
    return true;
}
std::string TrapSpell::getName() const { return "Trap"; }
int TrapSpell::getCost() const { return 20; }
SpellType TrapSpell::getType() const { return SpellType::kTrap; }

bool SummonSpell::cast(Game& game, Entity& caster, const Point& target) {
    Player* player_caster = dynamic_cast<Player*>(&caster);
    if (!player_caster) return false;

    auto& enhancements = player_caster->getEnhancements();
    int summon_count = 1 + enhancements.extra_summons;
    int spawned_count = 0;

    for (int i = 0; i < summon_count; ++i) {
        bool spawned_this_iteration = false;
        for (int dy = -1; dy <= 1 && !spawned_this_iteration; ++dy) {
            for (int dx = -1; dx <= 1 && !spawned_this_iteration; ++dx) {
                if (dx == 0 && dy == 0) continue;
                Point spawn_pos = {caster.getPosition().x + dx, caster.getPosition().y + dy};
                if (game.isCellFree(spawn_pos)) {
                    game.getEntityManager().addAlly(std::make_unique<Ally>(spawn_pos.x, spawn_pos.y, 60, 10));
                    spawned_this_iteration = true;
                    spawned_count++;
                }
            }
        }
    }

    if (spawned_count > 0) {
        std::stringstream ss;
        ss << "Summoned " << spawned_count << " ally/allies.";
        game.notifyLog(ss.str());
        enhancements.reset();
        return true;
    } else {
        game.notifyLog("No space to summon an ally.");
        return false;
    }
}
std::string SummonSpell::getName() const { return "Summon Ally"; }
int SummonSpell::getCost() const { return 30; }
SpellType SummonSpell::getType() const { return SpellType::kSummon; }

bool EnhancementSpell::cast(Game& game, Entity& caster, const Point& target) {
    Player* player_caster = dynamic_cast<Player*>(&caster);
    if (!player_caster) return false;

    auto& enhancements = player_caster->getEnhancements();
    enhancements.extra_damage += 10;
    enhancements.extra_range += 1;
    enhancements.extra_area += 1;
    enhancements.extra_trap_damage += 15;
    enhancements.extra_summons += 1;
    game.notifyLog("Your next spell is enhanced!");
    return true;
}
std::string EnhancementSpell::getName() const { return "Enhance"; }
int EnhancementSpell::getCost() const { return 10; }
SpellType EnhancementSpell::getType() const { return SpellType::kEnhance; }