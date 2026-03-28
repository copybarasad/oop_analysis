#include "tower.hpp"
EnemyTower::EnemyTower(int hp,int dmg, Point pos,int range):Actor(hp,dmg,pos),range_(range),cooldown_(0){}
int EnemyTower::Range() const noexcept { return range_; }
bool EnemyTower::OnCooldown() const noexcept { return cooldown_>0; }
void EnemyTower::Tick(){ if (cooldown_>0) --cooldown_; }
void EnemyTower::SetCooldown(int cd){ cooldown_=cd; }
int EnemyTower::Cooldown() const noexcept { return cooldown_; }
