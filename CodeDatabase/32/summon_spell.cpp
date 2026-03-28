#include "summon_spell.hpp"
#include "enhancementspell.hpp"
#include "game.hpp"
#include "player.hpp"
#include "field.hpp"
#include "cell.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include "constants.hpp"
SummonSpell::SummonSpell(int health, int damage)
    : health_(health), damage_(damage) {}

std::string SummonSpell::GetName() const noexcept {
    return "Призыв союзника";
}

bool SummonSpell::UseSpell(Game& game) {
    auto& player = game.GetPlayer();
    auto& field = game.GetField();

    int px = player.GetX();
    int py = player.GetY();

    int summonCount = EnhancementSpell::ApplySummonBonus(player, 1);

    std::vector<std::pair<int, int>> directions = {
        {px + SUMMON_OFFSET, py},
        {px - SUMMON_OFFSET, py},
        {px, py + SUMMON_OFFSET},
        {px, py - SUMMON_OFFSET}
    };

    int summoned = 0;

    for (auto [nx, ny] : directions) {
        if (nx < FIELD_MIN || ny < FIELD_MIN || nx >= field.width() || ny >= field.height())
            continue;

        auto& cell = field.At(nx, ny);
        if (cell.type() == CellType::kEmpty) {
            game.AddAlly(Ally("Союзник", health_, damage_, nx, ny));
            cell.SetType(CellType::kAlly);
            std::cout << "✨ Призван союзник в (" << nx << ", " << ny << ")!\n";
            summoned++;
            if (summoned >= summonCount) break;
        }
    }

    if (summoned == 0) {
        std::cout << "❌ Нет свободного места рядом с игроком для призыва союзника!\n";
        return false;
    }

    return true;
}

void SummonSpell::Upgrade() {
    damage_ += 2;
    health_ += 10;
    std::cout << "🧙 Призванные союзники стали сильнее!\n";
}