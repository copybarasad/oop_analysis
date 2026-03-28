#include "trap_spell.h"
#include "field.h"
#include "player.h"
#include "cell.h"
#include "cell_type.h"
#include <iostream>
#include <algorithm>

TrapSpell::TrapSpell(int damage) : damage_(damage) {}

bool TrapSpell::Cast(Field& field, Player& player, const std::pair<int,int>& target,
                    std::vector<Enemy>& /*enemies*/, std::vector<EnemyTower>& /*towers*/, std::vector<EnemyBase>& /*bases*/) {
    auto [pr, pc] = player.GetPosition();
    auto [tr, tc] = target;
    if (!field.IsInBounds(tr, tc)) {
        std::cout << "Target coordinates (" << tr << "," << tc << ") are out of bounds!\n";
        return false;
    }

    int dist = std::max(std::abs(tr - pr), std::abs(tc - pc));
    if (dist > 2) {
        std::cout << "Target is too far! Maximum trap placement range: 2 tiles\n";
        return false;
    }

    Cell &cell = field.CellAt(tr, tc);
    if (cell.GetType() != CellType::Empty && cell.GetType() != CellType::Slow) {
        std::cout << "Cannot place trap on occupied cell\n";
        return false;
    }

    int actual_damage = damage_;
    if (player.GetEnhancementStacks() > 0) {
        actual_damage *= (1 << player.GetEnhancementStacks());
        player.ConsumeEnhancementStack();
        std::cout << "Trap empowered! Damage multiplied to " << actual_damage << "!\n";
    }
    cell.SetType(CellType::Trap);
    cell.SetTrapDamage(actual_damage);
    std::cout << "Trap placed at (" << tr << ',' << tc << ")\n";
    return true;
}

std::string TrapSpell::Name() const { return "Trap"; }
std::string TrapSpell::Description() const {
    return "Places a trap that deals " + std::to_string(damage_) + " damage when an enemy steps on it";
}