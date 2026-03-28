#include "SpellAreaDamage.hpp"
#include "Field.hpp"
#include "Configs.hpp"
#include <iostream>

SpellAreaDamage::SpellAreaDamage(int dmg, int rng)
    : damage(dmg), range(rng) {}

bool SpellAreaDamage::cast(Field& field, const Pos& casterPos, const Pos& target) {
    // Вычисляем центр области
    double centerX = target.getX() + 0.5;
    double centerY = target.getY() + 0.5;
    
    // Расчет расстояния до центра
    double dx = casterPos.getX() - centerX;
    double dy = casterPos.getY() - centerY;
    double distance = std::sqrt(dx * dx + dy * dy);
    
    // Проверка радиуса до центра области
    if (distance > range) {
        std::cout << "Area spell out of range." << std::endl;
        return false;
    }

    if (!field.isValidPosition(target)) {
        std::cout << "Invalid target position." << std::endl;
        return false;
    }

    // Зона 2x2: (x,y), (x+1,y), (x,y+1), (x+1,y+1)
    for (int dx = 0; dx < 2; ++dx) {
        for (int dy = 0; dy < 2; ++dy) {
            Pos p(target.getX() + dx, target.getY() + dy);
            if (!field.isValidPosition(p)) continue;
            Enemy* e = field.getEnemyAt(p);
            if (e && e->isAlive()) {
                int before = e->getHealth();
                e->takeDamage(damage);
                int after = e->getHealth();
                std::cout << "Area spell hits enemy at (" << p.getX() << "," << p.getY()
                          << ") for " << damage << " (HP: " << before << " -> " << after << ")." << std::endl;
                if (!e->isAlive()) {
                    field.getPlayer().addScore(Config::Settings::PLAYER_SCORE_PER_KILL);
                    std::cout << "Enemy destroyed. +" << Config::Settings::PLAYER_SCORE_PER_KILL << " score." << std::endl;
                }
            }
        }
    }

    // Всегда считается использованной даже если урон врагам не нанесён
    field.removeDeadEnemies();
    std::cout << "Area spell applied to 2x2 area at (" << target.getX() << "," << target.getY() << ")." << std::endl;
    return true;
}

std::string SpellAreaDamage::name() const {
    return "Area Damage 2x2";
}

std::unique_ptr<ISpell> SpellAreaDamage::clone() const {
    return std::make_unique<SpellAreaDamage>(*this);
}
