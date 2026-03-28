#pragma once
#include "Enemy.h"

class Boss : public Enemy {
public:
    enum class Phase { IDLE, ATTACK_1, SUMMON };

    Boss(int x, int y, int difficulty);
    void update(float dt) override;
    // FIX: Boss is 3x3 tiles big
    int getSize() const override { return 3; }

private:
    Phase m_phase;
    float m_phaseTimer;
    int m_difficulty;

    void spawnMinions();
};
