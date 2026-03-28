#include "EnemyBase.h"
#include <random>
#include "Config.h"

EnemyBase::EnemyBase(std::pair<int, int> pos, int spawnDelay){
    pos_ =  pos;
    spawnDelay_ = spawnDelay;
    currentDelay_ = spawnDelay;
}

void EnemyBase::onTurn(std::vector<Enemy>&enemies, Board& board, Player& player){
    currentDelay_ -= 1;
    if (currentDelay_ > 0){
        return;
    }

    currentDelay_ = spawnDelay_;

    std::pair<int, int> dirs[4] = {{1,0}, {-1, 0}, {0, 1}, {0, -1}};
    for ( int i = 0; i < 4; ++i){
        std::pair <int, int> newPos = {pos_.first + dirs[i].first, pos_.second + dirs[i].second};
        if (newPos.first < 0 || newPos.second < 0 || newPos.first >= board.width() || newPos.second >= board.height()) continue;
        if (player.position() == newPos) continue;

        Cell& c = board.at(newPos);
        if(!c.isPassable()) continue;

        bool busy = false;
        for (int j = 0; j < enemies.size(); ++j){
            if (enemies[j].isAlive() && enemies[j].position() == newPos){
                busy = true;
                break;
            }
        }
        if (busy) continue;
        enemies.push_back(Enemy(ENEMY_HP, ENEMY_DAMAGE, newPos));
        break;
    }
}


std::pair<int, int> EnemyBase::position(){
    return pos_;
}


int EnemyBase::hp() const{
    return hp_;
}

bool EnemyBase::isAlive() const{
    return hp_ > 0;
}

void EnemyBase::takeDamage(int dmg){
    if(dmg < 0){
        return;
    }
    hp_ -= dmg;
    if(hp_ < 0){
        hp_ = 0;
    }
}

int EnemyBase::spawnDelay() const{
    return spawnDelay_;
}

int EnemyBase::currentTimer() const{
    return currentDelay_;
}

void EnemyBase::setHp(int hp){
    hp_ = hp;
    if (hp_ < 0) hp_ = 0;
}

void EnemyBase::setCurrentTimer(int value){
    currentDelay_ = value;
    if(currentDelay_ < 0) currentDelay_ = 0;
}