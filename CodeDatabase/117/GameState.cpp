#include "GameState.h"
#include "../LogSystem/Notifier.h"
#include "../LogSystem/DeathEvent.h"

GameState::GameState(Field field, Hand hand, Player player): field(field), player(player), hand(hand) {}

Hand& GameState::getHand(){
    return this->hand;
}

void GameState::setHand(Hand hand){
    this->hand = hand;
}

Field& GameState::getField(){
    return this->field;
}

Player& GameState::getPlayer(){
    return this->player;
}

std::vector<Enemy>& GameState::getEnemies(){
    return this->enemies;
}

std::vector<EnemyBuilding>& GameState::getBuilds(){
    return this->builds;
}

std::vector<Ally>& GameState::getAllies(){
    return this->allies;
}

std::vector<Trap>& GameState::getTraps(){
    return this->traps;
}

std::vector<EnemyTower>& GameState::getTowers(){
    return this->towers;
}

void GameState::deleteDead(){
    for(int i = this->enemies.size() - 1; i >= 0; --i){
        Enemy& enemy = this->enemies[i];
        if(!enemy.isAlive()){
            this->movSys.deleteFromField(enemy, this->field);
            this->enemies.erase(this->enemies.begin() + i);
            DeathEvent enemyDeathEvent = DeathEvent("Enemy");
            Notifier::getInstance().notify(enemyDeathEvent);
        }
    }

    for(int i = this->builds.size() - 1; i >= 0; --i){
        EnemyBuilding& build = this->builds[i];
        if(!build.isAlive()){
            this->movSys.deleteFromField(build, this->field);
            this->builds.erase(this->builds.begin() + i);
            DeathEvent enemyBuildDeathEvent = DeathEvent("Enemy Building");
            Notifier::getInstance().notify(enemyBuildDeathEvent);
        }
    }

    for(int i = this->towers.size() - 1; i >= 0; --i){
        EnemyTower& tower = this->towers[i];
        if(!tower.isAlive()){
            this->movSys.deleteFromField(tower, this->field);
            this->towers.erase(this->towers.begin() + i);
            DeathEvent towerDeathEvent = DeathEvent("Enemy Tower");
            Notifier::getInstance().notify(towerDeathEvent);
        }
    }

    for(int i = this->traps.size() - 1; i >= 0; --i){
        Trap& trap = this->traps[i];
        if(!trap.isAlive()){
            this->movSys.deleteFromField(trap, this->field);
            this->traps.erase(this->traps.begin() + i);
        }
    }

    for(int i = this->allies.size() - 1; i >= 0; --i){
        Ally& ally = this->allies[i];
        if(!ally.isAlive()){
            this->movSys.deleteFromField(ally, this->field);
            this->allies.erase(this->allies.begin() + i);
            DeathEvent allyDeathEvent = DeathEvent("Ally");
            Notifier::getInstance().notify(allyDeathEvent);
        }
    }
}

bool GameState::isGameOver() const{
    return !(this->player.isAlive());
}

bool GameState::isWin() const{
    return (towers.empty() && builds.empty() && enemies.empty());
}

int GameState::findEnemy(std::pair<int, int> position) const{
    if(!this->field.isValidPos(position)){
        return -1;
    }
    for(int i = 0; i < this->enemies.size(); ++i){
        if(this->enemies[i].getPos() == position){
            return i;
        }
    }
    return -1;
}

int GameState::findEnemyBuild(std::pair<int, int> position) const{
    if(!this->field.isValidPos(position)){
        return -1;
    }
    for(int i = 0; i < this->builds.size(); ++i){
        if(this->builds[i].getPos() == position){
            return i;
        }
    }
    return -1; 
}

int GameState::findAlly(std::pair<int, int> position) const{
    if(!this->field.isValidPos(position)){
        return -1;
    }
    for(int i = 0; i < this->allies.size(); ++i){
        if(this->allies[i].getPos() == position){
            return i;
        }
    }
    return -1; 
}

int GameState::findTower(std::pair<int, int> position) const{
    if(!this->field.isValidPos(position)){
        return -1;
    }
    for(int i = 0; i < this->towers.size(); ++i){
        if(this->towers[i].getPos() == position){
            return i;
        }
    }
    return -1; 
}

Character* GameState::findCharacter(std::pair<int, int> position){
    if(!this->field.isValidPos(position)){
        return nullptr;
    }
    int index = this->findEnemy(position);
    if(index >= 0){
        return &(this->enemies[index]);
    }
    index = this->findEnemyBuild(position);
    if(index >= 0){
        return &(this->builds[index]);
    }
    index = this->findAlly(position);
    if(index >= 0){
        return &(this->allies[index]);
    }
    index = this->findTower(position);
    if(index >= 0){
        return &(this->towers[index]);
    }
    if(this->player.getPos() == position){
        return &(this->player);
    }
    return nullptr;
}