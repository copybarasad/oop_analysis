#include "../../headers/spells/AreaDamageSpell.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/field/GameField.h"
#include "../../headers/objects/GameObject.h"
#include <iostream>
#include <sstream>

using namespace std;

AreaDamageSpell::AreaDamageSpell(int dmg, int rng, string spellName) : damage(dmg), range(rng), name(spellName) {}

string AreaDamageSpell::getName() const { return name; }

string AreaDamageSpell::getDescription() const { 
    return "Deals " + to_string(damage) + " damage in 2x2 area";
}

int AreaDamageSpell::getRange() const { return range; }

bool AreaDamageSpell::canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const {
    if (!field.isValidPosition(targetPos)) return false;
    double distance = casterPos.distanceTo(targetPos);
    return distance <= range && distance > 0;
}

void AreaDamageSpell::cast(const Position& casterPos, const Position& targetPos, GameField& field, vector<shared_ptr<GameObject>>& objects) {        
    int hits = 0;
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            Position areaPos(targetPos.getX() + dx, targetPos.getY() + dy);
            if (field.isValidPosition(areaPos)) {                    
                for (auto& obj : objects) {
                    if (obj->isAlive() && obj->getPosition() == areaPos) {
                        if (dynamic_cast<Enemy*>(obj.get()) || dynamic_cast<EnemyTower*>(obj.get())){
                            obj->takeDamage(damage);
                            hits++;
                            cout << name << " hit target " << damage << " damage!\n";
                        }
                    }
                }
            }
        }
    }
    
    if (hits == 0) {
        cout << name << " missed - no valid targets in area!\n";
    } else {
        cout << name << " hit " << hits << " targets total!\n";
    }
}

string AreaDamageSpell::serialize() const {
    return "AreaDamage " + to_string(damage) + " " + to_string(range) + " " + name;
}