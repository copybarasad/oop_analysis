#include "CharacterFactory.h"

std::unique_ptr<Player> CharacterFactory::createPlayer(const Position &startPosition, int health, int meleeDmg, int rangedDmg) {
    return std::make_unique<Player>(startPosition, health, meleeDmg, rangedDmg);
}

std::unique_ptr<Enemy> CharacterFactory::createEnemy(const Position &startPosition, int health, int damage) {
    return std::make_unique<Enemy>(startPosition, health, damage);
}

std::unique_ptr<Ally> CharacterFactory::createAlly(const Position &pos, int health, int damage) {
    return std::make_unique<Ally>(pos, health, damage);
}