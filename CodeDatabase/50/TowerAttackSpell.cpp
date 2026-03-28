#include "TowerAttackSpell.h"
#include "GameField.h"
#include "Player.h"

TowerAttackSpell::TowerAttackSpell(double damage, int range, int x, int y)
    : damage(damage), range(range), x(x), y(y) {}

bool TowerAttackSpell::canCast(const Field& field, const Player& player) const {
    // башня всегда знает игрока, поэтому просто возвращаем true
    return player.isAlive();
}

void TowerAttackSpell::cast(Field& field, Player& player) {
    int px = player.getX();
    int py = player.getY();

    int dx = px - getTowerX();
    int dy = py - getTowerY();

    if (abs(dx) <= getRange() && abs(dy) <= getRange()) {
        std::cout << "Башня (" << getTowerX() << ", " << getTowerY()
                  << ") атакует игрока заклинанием TowerAttack!\n";
        player.takeDamage(getDamage());
        std::cout << "Игрок получил " << getDamage() << " урона!\n";
    } else {
        std::cout << "Башня (" << getTowerX() << ", " << getTowerY()
                  << ") не может достать игрока — вне радиуса.\n";
    }
}
