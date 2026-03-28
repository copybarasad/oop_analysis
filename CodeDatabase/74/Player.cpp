#include "Player.h"
#include "Logger.h"
#include <iostream>

Player::Player() : Entity(), mode(AttackMode::Melee), damage(PLAYER_MELEE_DAMAGE), aimPos({ 0, 0 }), hand(PLAYER_HAND_SIZE) {}

Player::Player(int health, int damage, int x, int y)
    : Entity(health, damage, x, y),
    damage(damage),
    mode(AttackMode::Melee),
    aimPos({ x, y }),
    hand(PLAYER_HAND_SIZE),
    maxHealth(health)
{
}

AttackMode Player::getAttackMode() const {
    return mode;
}

void Player::switchAttackMode(AttackMode newMode) {
    mode = newMode;
}

Coords Player::getAimPos() const {
    return aimPos;
}

void Player::moveAimPos(int dx, int dy) {
    aimPos.x += dx;
    aimPos.y += dy;
}

int Player::getDamage() const {
    auto it = attackDamage.find(mode);
    if (it != attackDamage.end())
        return it->second;
    return 0;
}

Hand& Player::getHand() {
    return hand;
}

const Hand& Player::getHand() const {
    return hand;
}

void Player::log() const {
    Logger::log() << "[Player] { x: " << pos.x
        << ", y: " << pos.y
        << " }, HP: " << health
        << ", Dmg: " << getDamage()
        << ", Mode: " << (mode == AttackMode::Melee ? "Melee" : "Ranged")
        << "\n";
}

json Player::serialize() const {
    json j;
    j["x"] = pos.x;
    j["y"] = pos.y;
    j["health"] = health;
    j["damage"] = damage;
    j["mode"] = static_cast<int>(mode);
    j["hand"] = hand.serialize();
    j["maxHealth"] = maxHealth;
    return j;
}

void Player::deserialize(const json& j) {
    pos.x = j.at("x").get<int>();
    pos.y = j.at("y").get<int>();
    health = j.at("health").get<int>();
    damage = j.at("damage").get<int>();
    maxHealth = j.at("maxHealth").get<int>();
    mode = static_cast<AttackMode>(j.at("mode").get<int>());
    hand.deserialize(j.at("hand"));
}
