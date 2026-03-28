#include "Ally.h"
#include "Game.h"
#include <cmath>
#include <iostream>

Ally::Ally(std::string id, Position pos, int hp, int attack)
    : Entity(std::move(id), pos, hp), attack_(attack)
{
}

int Ally::GetAttack() const
{
    return attack_;
}

void Ally::OnTurn(Game& game)
{
    if (!IsAlive())
        return;

    // ищем ближайшего врага
    Enemy* closest = nullptr;
    int bestDist = 99999;

    for (auto& eptr : game.GetEnemies())
    {
        if (!eptr->IsAlive()) continue;

        int dx = eptr->GetPosition().x - GetPosition().x;
        int dy = eptr->GetPosition().y - GetPosition().y;
        int dist = std::abs(dx) + std::abs(dy);

        if (dist < bestDist)
        {
            bestDist = dist;
            closest = eptr.get();
        }
    }

    if (!closest) return;

    Position a = GetPosition();
    Position e = closest->GetPosition();

    int dist = std::abs(a.x - e.x) + std::abs(a.y - e.y);

    // если рядом враг — тогда кего атакуем
    if (dist == 1)
    {
        std::cout << GetId() << " attacks " << closest->GetId()
                  << " for " << attack_ << "\n";

        closest->TakeDamage(attack_);

        return;
    }

    // двигаемся на 1 клетку к врагу
    Position newPos = a;

    if (a.x < e.x) newPos.x++;
    else if (a.x > e.x) newPos.x--;

    if (a.y < e.y) newPos.y++;
    else if (a.y > e.y) newPos.y--;

    // проверка границ и занятости, иначе не идем
    if (game.IsInBounds(newPos) && !game.IsOccupied(newPos))
    {
        SetPosition(newPos);
        std::cout << GetId() << " moves to (" << newPos.x << "," << newPos.y << ")\n";
    }
}
