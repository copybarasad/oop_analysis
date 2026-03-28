#include "SummonSpell.h"


SummonSpell::SummonSpell(std::string name, int count, int range = 2)
    : name_(std::move(name))
    , count_(count) 
    , range_(range)
{}

std::string SummonSpell::GetName() const 
{ 
    return name_; 
}

void SummonSpell::ApplyBuffs(BuffState const& state) 
{
	count_ += state.summonCountBonus;
}

bool SummonSpell::Apply(Player& caster, Game& game, Position target)
{
    Position playerPos = caster.GetPosition();

    // Проверяем близость — союзника можно ставить в радиусе 1 от игрока
    int dx = std::abs(target.x - playerPos.x);
    int dy = std::abs(target.y - playerPos.y);

    if (dx > range_ || dy > range_) 
    {
        std::cout << "You can't summon ally!\n";
        return false;
    }
    if (!game.IsInBounds(target) || game.IsOccupied(target)) 
    {
        std::cout << "Invalid summon location!\n";
        return false;
    }
    for (int i = 0; i < count_; ++i) 
    {
        game.SpawnAlly(target);
    }
    std::cout << "You have summoned ally at (" << target.x << ", " << target.y << ")\n";

    return true;
}
