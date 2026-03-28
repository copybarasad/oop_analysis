#include "direct_damage_spell.h"
#include "game_controller.h"
#include "enemy.h"

DirectDamageSpell::DirectDamageSpell(const std::string& name,
    const std::string& description,
    int damage,
    int range,
    GameController* gameController)
    : DamageSpell(name, description, damage, range, gameController) {
}

bool DirectDamageSpell::applyEffect() {
    if (!isValidTarget(targetPosition)) {
        return false;
    }

    auto& enemies = const_cast<std::vector<Enemy>&>(gameController->getEnemies());
    for (auto& enemy : enemies) {
        if (enemy.getPosition() == targetPosition && enemy.isAlive()) {
            enemy.takeDamage(spellDamage);
            return true;
        }
    }
    return false;
}

uint8_t DirectDamageSpell::getType() const {
    return 1;  // 1 = прямой урон
}

void DirectDamageSpell::save(std::ostream& out) const {
    DamageSpell::save(out);  // сохраняем всё из базового класса
}

void DirectDamageSpell::load(std::istream& in) {
    DamageSpell::load(in);   // загружаем всё из базового класса
}