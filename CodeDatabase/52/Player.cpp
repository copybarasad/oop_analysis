#include "Player.h"
#include "GameField.h"
#include "SpellFactory.h"
#include "Enemy.h"

void Player::Update(GameField& field) {
    if (HandRef().Size() == 0)
        HandRef().Add(SpellFactory::MakeRandom());
}


void Player::SwitchMode() {
    if (mode_ == AttackMode::Melee)
        mode_ = AttackMode::Ranged;
    else
        mode_ = AttackMode::Melee;
}

int Player::AttackRange() const noexcept {
    return (mode_ == AttackMode::Melee ? 1 : rangedRange_);
}

void Player::PerformAttack(GameField& field) {
    auto [px, py] = field.PlayerCell();
    int range = AttackRange();

    if (mode_ == AttackMode::Melee) {
        static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
        for (auto& d : dirs) {
            int x = px + d[0];
            int y = py + d[1];
            if (auto* e = field.EnemyAt(x,y)) {
                e->TakeDamage(Damage());
                return;
            }
        }
        return;
    }

    static const int dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    for (auto& d : dirs) {
        for (int step=1; step<=range; ++step) {
            int x = px + d[0]*step;
            int y = py + d[1]*step;
            if (!field.Inside(x,y)) break;
            if (!field.CellAt(x,y).isWalkable()) break;
            if (auto* e = field.EnemyAt(x,y)) {
                e->TakeDamage(Damage());
                return;
            }
        }
    }
}