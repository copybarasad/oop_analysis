#include "../../headers/objects/Trap.h"
#include <sstream>
#include <iostream>

using namespace std;

Trap::Trap(const Position& pos, int dmg) : position(pos), damage(dmg), active(true) {}

Position Trap::getPosition() const { return position; }

bool Trap::isAlive() const { return active; }

void Trap::takeDamage(int) {}

void Trap::trigger(shared_ptr<GameObject> target) {
    if (active && target->isAlive()) {
        target->takeDamage(damage);
        cout << "Trap was activated! Dealt " << damage << " damage!\n";
        active = false;
    }
}

string Trap::serialize() const {
    return "Trap " + position.toString() + " " + to_string(damage) + " " + (active ? "1" : "0");
}

shared_ptr<Trap> Trap::deserialize(const string& data) {
    stringstream ss(data);
    string type;
    ss >> type;
    if (type != "Trap") throw invalid_argument("Not a Trap");
    
    string posStr;
    int damage;
    bool active;
    if(!(ss >> posStr >> damage >> active)) {
        throw invalid_argument("Invalid trap data format");
    }
    
    Position pos = Position::fromString(posStr);
    auto trap = make_shared<Trap>(pos, damage);
    trap->active = active;
    return trap;
}