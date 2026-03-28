
#include "game_objects.h"
#include <stdexcept>

GameObject::GameObject(int X, int Y, int hp, std::string combat, int RANGED, int froz) {
    x = X;
    y = Y;
    HP = hp;
    type_combat = combat;
    ranged = RANGED;
    flag_life = true;
    frozen = froz;
}

void GameObject::move(int nx, int ny) {
    if (nx > 1 || nx < -1 || ny > 1 || ny < -1) {
        throw std::runtime_error("Invalid movement");
    } 
    x += nx;
    y += ny;
}

nlohmann::json GameObject::toJson() const {
    return {
        {"x", x},
        {"y", y},
        {"health", HP},
        {"combat_type", type_combat},
        {"range", ranged},
        {"frozen", frozen},
        {"alive", flag_life}
    };
}
void GameObject::fromJson(const nlohmann::json& j) {
    x = j["x"];
    y = j["y"];
    HP = j["health"];
    type_combat = j["combat_type"];
    ranged = j["range"];
    frozen = j["frozen"];
    flag_life = j["alive"];
} 

int GameObject::getX() { return x; }
int GameObject::getY() { return y; }
std::string GameObject::getTypeCombat() { return type_combat; }
void GameObject::setTypeCombat(std::string types) { type_combat = types; }
int GameObject::getFrozen() { return frozen; }
void GameObject::setFrozen(int n) { frozen = n; }

void GameObject::reduceFrozen() { if (frozen > 0) frozen--; }
int GameObject::getRange() { return ranged; }
void GameObject::takeDamage(int received_damage) { 
    HP -= received_damage;
    if (HP <= 0) flag_life = false;
}
int GameObject::getHP() { return HP; }
