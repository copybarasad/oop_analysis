#include "Player.h"
#include <format>

float Player::attack() {
    return far_mode ? damage / 2 : damage;
}


json Player::to_json() {
    json j = Entity::to_json();
    j["points"] = points;
    j["far_mode"] = far_mode;
    j["far_attack_reach"] = far_attack_reach;
    j["coins"] = coins;
    j["hand"] = hand.to_json();
    return j;
}

bool Player::from_json(json& j) {
    if (!Entity::from_json(j)) return false;
    try {
        points = j.at("points");
        far_mode = j.at("far_mode");
        far_attack_reach = j.at("far_attack_reach");
        coins = j.at("coins");
        return hand.from_json(j.at("hand"));
    } catch (...) { return false; }
}
