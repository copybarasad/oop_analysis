#include "summon_spell.h"
#include "field.h"
#include "player.h"
#include "cell.h"
#include "cell_type.h"
#include <iostream>

SummonSpell::SummonSpell(int count) : ally_count_(count) {}

bool SummonSpell::Cast(Field& field, Player& player, const std::pair<int,int>& /*target*/, 
                      std::vector<Enemy>& /*enemies*/, std::vector<EnemyTower>& /*towers*/, std::vector<EnemyBase>& /*bases*/) {
    auto [pr, pc] = player.GetPosition();
    int summoned = 0;
    const int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    int actual_count = ally_count_;
    if (player.GetEnhancementStacks() > 0) {
        actual_count += player.GetEnhancementStacks();
        player.ConsumeEnhancementStack();
        std::cout << "Spell empowered! Number of ally increased to " << actual_count << "!\n";
    }

    for (int i = 0; i < 8 && summoned < actual_count; ++i) {
        int r = pr + dirs[i][0];
        int c = pc + dirs[i][1];
        if (!field.IsInBounds(r,c)) continue;
        Cell &cell = field.CellAt(r,c);
        if (cell.GetType() == CellType::Empty) {
            cell.SetType(CellType::Ally);
            ++summoned;
            std::cout << "Ally summoned at position (" << r << "," << c << ")\n";
        }
    }
    if (summoned > 0) {
        std::cout << "Successfully summoned " << summoned << " ally" << (summoned > 1 ? "s" : "") << "!\n";
    } else {
        std::cout << "No suitable locations found for summoning allies.\n";
    }
    return summoned > 0;
}

std::string SummonSpell::Name() const { return "Summon"; }
std::string SummonSpell::Description() const {
    return "Summons allies next to player";
}

void SummonSpell::SetCount(int count) { ally_count_ = count; }
int SummonSpell::GetCount() const {return ally_count_; }
