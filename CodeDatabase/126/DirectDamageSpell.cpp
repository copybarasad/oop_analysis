#include "../../headers/spells/DirectDamageSpell.h"
#include "../../headers/objects/Enemy.h"
#include "../../headers/objects/EnemyTower.h"
#include "../../headers/field/GameField.h"
#include "../../headers/objects/GameObject.h"
#include <iostream>
#include <sstream>

using namespace std;

DirectDamageSpell::DirectDamageSpell(int dmg, int rng, string spellName) : damage(dmg), range(rng), name(spellName) {}

string DirectDamageSpell::getName() const { return name; }

string DirectDamageSpell::getDescription() const { 
    return "Deals " + to_string(damage) + " damage to a single target";
}

int DirectDamageSpell::getRange() const { return range; }

bool DirectDamageSpell::canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const {
    if (!field.isValidPosition(targetPos)) return false;
    double distance = casterPos.distanceTo(targetPos);
    return distance <= range && distance > 0;
}

void DirectDamageSpell::cast(const Position& casterPos, const Position& targetPos, GameField& field, vector<shared_ptr<GameObject>>& objects) {        
    if (!field.isValidPosition(targetPos)) {
        cout << name << " failed - target position is outside the field!\n";
        return;
    }
    
    for (auto& obj : objects) {
        if (obj->isAlive() && obj->getPosition() == targetPos) {
            if (dynamic_cast<Enemy*>(obj.get()) || dynamic_cast<EnemyTower*>(obj.get())) {
                obj->takeDamage(damage);
                cout << name << " hit ";

                if (dynamic_cast<Enemy*>(obj.get())) {
                    cout << "enemy";
                } else if (dynamic_cast<EnemyTower*>(obj.get())) {
                    cout << "tower";
                }

                cout << " for " << damage << " damage!\n";
                return;
            }
        }
    }
    
    cout << name << " missed - no valid target (enemy or tower)\n";
}

string DirectDamageSpell::serialize() const {
    return "DirectDamage " + to_string(damage) + " " + to_string(range) + " " + name;
}