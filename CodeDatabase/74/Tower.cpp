#include "Tower.h"
#include <cmath>
#include <iostream>
#include "Logger.h"

Tower::Tower(Coords pos, int range, int damage, int skip)
    : pos(pos), range(range), skip(skip),
    spell(damage, range) {
}

bool Tower::tryAttack(Player& player, int move) {
    if (move % skip != 0) return false;

    int dx = std::abs(player.getCoords().x - pos.x);
    int dy = std::abs(player.getCoords().y - pos.y);

    if (dx <= range && dy <= range) {
        player.takeDamage(spell.getDamage());
        Logger::log() << "Башня (" << pos.x << "," << pos.y << ") попала по игроку!\n";
        return true;
    }

    return false;
}

json Tower::serialize() const {
    json j;
    j["x"] = pos.x;
    j["y"] = pos.y;
    j["range"] = range;
    j["damage"] = spell.getDamage();
    j["skip"] = skip;

    return j;
}

void Tower::deserialize(const json& j) {
    pos.x = j.at("x").get<int>();
    pos.y = j.at("y").get<int>();
    range = j.at("range").get<int>();
    int dmg = j.at("damage").get<int>();
    skip = j.at("skip").get<int>();
    spell = DamageSpell(dmg, range);
}