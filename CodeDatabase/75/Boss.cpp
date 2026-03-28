#include "Boss.h"
#include "GameField.h"
#include "Bullet.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Boss::Boss(int x, int y, int difficulty)
    : Enemy(x, y, Enemy::Type::BOSS, "DEMON LORD", 500 + difficulty*100, 20 + difficulty*2),
      m_phase(Phase::IDLE), m_phaseTimer(0.0f), m_difficulty(difficulty) {
}

void Boss::update(float dt) {
    // Call Base Class update
    Enemy::update(dt);

    m_phaseTimer += dt; 
    
    if (m_phase == Phase::IDLE) {
        if (m_phaseTimer > 2.0f) { m_phase = Phase::ATTACK_1; m_phaseTimer = 0; }
    } 
    else if (m_phase == Phase::ATTACK_1) {
        if (m_phaseTimer > 5.0f) { m_phase = Phase::SUMMON; m_phaseTimer = 0; }
    }
    else if (m_phase == Phase::SUMMON) {
        if (m_phaseTimer > 1.0f) { spawnMinions(); m_phase = Phase::IDLE; m_phaseTimer = 0; }
    }
}

void Boss::spawnMinions() {
    for(int i=0; i<2; ++i) {
        int mx = m_x + (rand()%5 - 2); int my = m_y + (rand()%5 - 2);
        Enemy::spawn(mx, my, Enemy::Type::MELEE, m_difficulty);
    }
}