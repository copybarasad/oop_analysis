#include "Spell.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "Constants.h"
#include <cmath>

namespace {

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    
public:
    DirectDamageSpell(int damage = DIRECT_SPELL_DAMAGE, int range = DIRECT_SPELL_RANGE) 
        : damage(damage), range(range) {}
    
    std::string name() const override { return "Direct"; }
    bool canTargetCell() const override { return true; }
    bool cast(GameField& field, Player& player, int tx, int ty) override {
        int px = player.getX(), py = player.getY();
        int dist = std::abs(px - tx) + std::abs(py - ty);
        if (dist > range) return false;

        auto& enemies = field.getEnemies();
        auto eit = enemies.find({tx,ty});
        if (eit != enemies.end()) { 
            eit->second->takeDamage(damage); 
            return true; 
        }

        auto& buildings = field.getBuildings();
        auto bit = buildings.find({tx,ty});
        if (bit != buildings.end()) { 
            bit->second->takeDamage(damage); 
            return true; 
        }
        auto& towers = field.getTowers();
        auto tit = towers.find({tx,ty});
        if (tit != towers.end()) { 
            tit->second->takeDamage(damage); 
            return true; 
        }
        
        if (tx == px && ty == py) {
            player.takeDamage(damage);
            return true;
        }
        
        return false;
    }
};

class AreaDamageSpell : public Spell {
public:
    std::string name() const override { return "AoE"; }
    bool canTargetCell() const override { return true; }
    bool cast(GameField& field, Player& player, int tx, int ty) override {
        int px = player.getX(), py = player.getY();
        int dist = std::abs(px - tx) + std::abs(py - ty);
        if (dist > AOE_SPELL_RANGE) return false;
        for (int dy = 0; dy < 2; ++dy) {
            for (int dx = 0; dx < 2; ++dx) {
                int cx = tx + dx, cy = ty + dy;
                if (!field.isPositionValid(cx, cy)) continue;
                auto& enemies = field.getEnemies();
                auto eit = enemies.find({cx,cy}); if (eit != enemies.end()) eit->second->takeDamage(AOE_SPELL_DAMAGE);
                auto& buildings = field.getBuildings();
                auto bit = buildings.find({cx,cy}); if (bit != buildings.end()) bit->second->takeDamage(AOE_SPELL_DAMAGE);
            }
        }
        return true;
    }
};

class TrapSpell : public Spell {
public:
    std::string name() const override { return "Trap"; }
    bool canTargetCell() const override { return true; }
    bool cast(GameField& field, Player& player, int tx, int ty) override {
        int px = player.getX(), py = player.getY();
        int dist = std::abs(px - tx) + std::abs(py - ty);
        if (dist > AOE_SPELL_RANGE) return false;
        if (!field.isPositionValid(tx, ty)) return false;
        if (field.getCellType(tx, ty) == CellType::IMPASSABLE) return false;
        field.placeTrap(tx, ty, TRAP_DAMAGE);
        return true;
    }
};

}

Spell* makeDirectSpell(int damage = DIRECT_SPELL_DAMAGE, int range = DIRECT_SPELL_RANGE) { 
    return new DirectDamageSpell(damage, range); 
}
Spell* makeAreaSpell() { return new AreaDamageSpell(); }
Spell* makeTrapSpell() { return new TrapSpell(); }


