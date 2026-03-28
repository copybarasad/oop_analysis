#include "trap.hpp"
#include "enhancementspell.hpp"
#include "game.hpp"
#include "player.hpp"
#include "field.hpp"
#include "cell.hpp"
#include <iostream>
#include <cmath>
#include "constants.hpp"

TrapSpell::TrapSpell(int damage, int radius)
    : damage_(damage), radius_(radius) {}

std::string TrapSpell::GetName() const noexcept {
    return "Ловушка";
}

bool TrapSpell::UseSpell(Game& game) {
    auto& player = game.GetPlayer();
    auto& field = game.GetField();

    int effectiveDamage = EnhancementSpell::ApplyDamageBonus(player, damage_);

    int px = player.GetX();
    int py = player.GetY();

    std::cout << "Выберите направление установки ловушки (w/a/s/d): ";
    char dir;
    std::cin >> dir;

    int tx = px, ty = py;
    switch (dir) {
        case DIR_UP:    ty -= radius_; break;
        case DIR_DOWN:  ty += radius_; break;
        case DIR_LEFT:  tx -= radius_; break;
        case DIR_RIGHT: tx += radius_; break;
        default:
            std::cout << "Некорректное направление!\n";
            return false;
    }

    if (tx < FIELD_MIN || ty < FIELD_MIN || tx >= field.width() || ty >= field.height()) {
        std::cout << "📍 Ловушку нельзя установить за пределами поля.\n";
        return false;
    }

    auto& cell = field.At(tx, ty);
    if (cell.type() != CellType::kEmpty) {
        std::cout << "📍 Здесь нельзя установить ловушку!\n";
        return false;
    }

    cell.SetType(CellType::kTrap);
    game.AddTrap(tx, ty, effectiveDamage);

    std::cout << "🕳️ Ловушка установлена в (" << tx << ", " << ty << ")"
              << " с уроном " << effectiveDamage << "\n";

    return true;
}

void TrapSpell::Upgrade() {
    damage_ += 5;
    std::cout << "🪤 Ловушка теперь наносит больше урона! Новое значение: " 
              << damage_ << "\n";
}