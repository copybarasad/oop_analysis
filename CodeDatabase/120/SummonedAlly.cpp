
#include "GameLogic.h"
SummonedAlly::SummonedAlly(Position pos) : Entity(pos, 10) {}
void SummonedAlly::Act(GameWorld& world, GameLogic& logic) {
  Position p = GetPosition();
  auto& enemies = world.GetEnemies();

  Enemy* target = nullptr;
  int minDist = std::numeric_limits<int>::max();

  for (auto& e : enemies) {
    if (e && e->IsAlive()) {
      int dist = std::abs(p.x - e->GetPosition().x) +
                 std::abs(p.y - e->GetPosition().y);
      if (dist < minDist) {
        minDist = dist;
        target = e.get();
      }
    }
  }

  if (!target) return;

  Position tpos = target->GetPosition();

  if (minDist == 1) {
    target->TakeDamage(damage);
    logic.events.Notify(DamageEvent{"Ally",damage,tpos.x,tpos.y});
    if (!target->IsAlive()) logic.events.Notify(GameMessageEvent{"Enemy slain by ally."});
    return;
  }

  Position newPos = p;
  int dx = tpos.x - p.x;
  int dy = tpos.y - p.y;

  if (dx != 0)
    newPos.x += (dx > 0 ? 1 : -1);
  else if (dy != 0)
    newPos.y += (dy > 0 ? 1 : -1);

  if (!world.GetMap().IsInside(newPos.x, newPos.y)) return;
  if (world.GetMap().IsWalkable(newPos.x, newPos.y) &&
      !world.IsOccupied(newPos.x, newPos.y)) {
    SetPosition(newPos);
    logic.events.Notify(MoveEvent{"Ally",newPos.x,newPos.y});
  }
}
