#include "SummonSpell.h"

#include "Game.h"
#include "GameField.h"
#include "Cell.h"
#include "Player.h"
#include "Ally.h"

#include <iostream>
#include <stdexcept>

SummonSpell::SummonSpell(int ally_health, int ally_damage, int cost)
    : ally_health_(ally_health),
      ally_damage_(ally_damage),
      cost_(cost) {
    EnsureInvariants();
}

std::string SummonSpell::GetName() const {
    return "Summon";
}

int SummonSpell::GetCost() const {
    return cost_;
}

bool SummonSpell::CanCast(const Game &game,
                          std::size_t /*x*/,
                          std::size_t /*y*/) const {
    std::size_t px = 0, py = 0;
    if (!game.GetPlayerPosition(px, py)) {
        return false;
    }

    const GameField &field = game.GetField();
    GameField::Size size = field.GetSize();

    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (int i = 0; i < 4; ++i) {
        int nx = static_cast<int>(px) + dirs[i][0];
        int ny = static_cast<int>(py) + dirs[i][1];

        if (nx < 0 || ny < 0 ||
            nx >= static_cast<int>(size.width) ||
            ny >= static_cast<int>(size.height)) {
            continue;
        }

        std::size_t sx = static_cast<std::size_t>(nx);
        std::size_t sy = static_cast<std::size_t>(ny);

        if (field.GetTerrain(sx, sy) != Cell::Terrain::kBlocked &&
            field.GetOccupant(sx, sy) == Cell::Occupant::kNone) {
            return true;
        }
    }
    return false;
}

bool SummonSpell::Cast(Game &game,
                       std::size_t /*x*/,
                       std::size_t /*y*/) {
    Player &player = game.GetPlayer();
    GameField &field = game.GetField();
    GameField::Size size = field.GetSize();

    std::size_t px = 0, py = 0;
    if (!game.GetPlayerPosition(px, py)) {
        return false;
    }

    int stacks = player.ConsumeBuffStacks();
    int allies_to_summon = 1 + stacks;
    if (allies_to_summon <= 0) {
        allies_to_summon = 1;
    }

    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int summoned = 0;

    for (int i = 0; i < 4 && summoned < allies_to_summon; ++i) {
        int nx = static_cast<int>(px) + dirs[i][0];
        int ny = static_cast<int>(py) + dirs[i][1];

        if (nx < 0 || ny < 0 ||
            nx >= static_cast<int>(size.width) ||
            ny >= static_cast<int>(size.height)) {
            continue;
        }

        std::size_t sx = static_cast<std::size_t>(nx);
        std::size_t sy = static_cast<std::size_t>(ny);

        if (field.GetTerrain(sx, sy) == Cell::Terrain::kBlocked) {
            continue;
        }
        if (field.GetOccupant(sx, sy) != Cell::Occupant::kNone) {
            continue;
        }

        // создаём союзника
        game.GetAllies().emplace_back(ally_health_, ally_damage_);
        std::size_t ally_index = game.GetAllies().size() - 1;

        field.SetOccupant(sx, sy, Cell::Occupant::kAlly,
                          static_cast<int>(ally_index));
        ++summoned;
    }

    if (summoned == 0) {
        std::cout << "No space to summon allies.\n";
        return false;
    }

    std::cout << "Summoned " << summoned << " ally(ies).\n";
    return true;
}

void SummonSpell::EnsureInvariants() const {
    if (ally_health_ <= 0 || ally_damage_ <= 0) {
        throw std::logic_error("SummonSpell ally stats must be > 0");
    }
    if (cost_ < 0) {
        throw std::logic_error("SummonSpell cost must be >= 0");
    }
}
