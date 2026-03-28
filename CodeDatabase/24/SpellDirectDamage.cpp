#include "SpellDirectDamage.hpp"
#include "Field.hpp"
#include "Configs.hpp"
#include <iostream>

SpellDirectDamage::SpellDirectDamage(int dmg, int rng)
    : damage(dmg), range(rng) {}

bool SpellDirectDamage::cast(Field& field, const Pos& casterPos, const Pos& target) {
    // Проверка радиуса (евклидово расстояние)
    if (casterPos.distanceTo(target) > range) {
        std::cout << "Spell out of range." << std::endl;
        return false;
    }

    if (!field.isValidPosition(target)) {
        std::cout << "Invalid target position." << std::endl;
        return false;
    }

    // Если есть враг на целевой клетке — наносим урон, иначе заклинание не используется
    Enemy* e = field.getEnemyAt(target);
    if (!e) {
        std::cout << "No target at that position. Spell wasted." << std::endl;
        return false;
    }

    int before = e->getHealth();
    e->takeDamage(damage);
    int after = e->getHealth();

    std::cout << "Direct spell deals " << damage << " damage to enemy (HP: " << before << " -> " << after << ")." << std::endl;

    // Если враг убит — добавляем очки игроку
    if (!e->isAlive()) {
        field.getPlayer().addScore(Config::Settings::PLAYER_SCORE_PER_KILL);
        std::cout << "Enemy destroyed. +" << Config::Settings::PLAYER_SCORE_PER_KILL << " score." << std::endl;
    }

    // Удаляем трупы сразу
    field.removeDeadEnemies();

    return true;
}

std::string SpellDirectDamage::name() const {
    return "Direct Damage";
}

std::unique_ptr<ISpell> SpellDirectDamage::clone() const {
    return std::make_unique<SpellDirectDamage>(*this);
}