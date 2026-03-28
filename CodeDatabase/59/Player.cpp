#include "Player.h"
#include <string>
#include <format>

Player::Player(int damage, Coordinates position, int maxHealth)
    : Being(maxHealth, maxHealth, damage, position), bow_damage(damage) {}

Player::Player(int damage, int bow_damage, Coordinates position, int maxHealth)
    : Being(maxHealth, maxHealth, damage, position), bow_damage(bow_damage) {}

std::string Player::getHud() const {
    return std::format("Здоровье: {}/{}, Урон: {}, Очки: {}, Оружие: {}",
                     (int)health, maxHealth,
                     (bow_equipped ? bow_damage : damage),
                     points, (bow_equipped ? "Лук" : "Меч"));
}

float Player::attack() { return bow_equipped ? (float)bow_damage : (float)damage; }

void Player::toggleBow() { bow_equipped = !bow_equipped;
}
void Player::setBow(bool on) { bow_equipped = on; }
bool Player::isBowEquipped() const { return bow_equipped; }

void Player::changePoints(int diff) { points += diff;
}
int Player::getPoints() const { return points; }

json Player::toJson() const {
    json j = serializeBase();
    j["points"] = points;
    j["bow_equipped"] = bow_equipped;
    j["bow_damage"] = bow_damage;
    return j;
}

void Player::fromJson(const json& j) {
    deserializeBase(j);
    j.at("points").get_to(points);
    j.at("bow_equipped").get_to(bow_equipped);
    j.at("bow_damage").get_to(bow_damage);
}