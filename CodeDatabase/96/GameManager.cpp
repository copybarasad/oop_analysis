
#include <iostream>
#include <algorithm>
#include <random>
#include "GameManager.h"
#include "SpellContext.h"


GameManager::GameManager(int width, int height, int numEnemies, int numBuildings, int numTowers)
    : playerManager(), enemyManager(), buildingManager(), field(width, height), hand(), towerManager(), allyManager() {
        auto playerPos = field.GetRandomFreePosition();
        playerManager.SetPlayerPosition(playerPos);
        field.SetCellOccupied(playerPos, true);
        for(int i = 0; i < numEnemies; ++i) {
            auto pos = field.GetRandomFreePosition();
            enemyManager.AddEnemyAtPos(100, 10, pos); 
            field.SetCellOccupied(pos, true);
        }
        for(int i = 0; i < numBuildings; ++i) {
            auto pos = field.GetRandomFreePosition();
            buildingManager.AddBuildingAtPos(pos, 5); 
            field.SetCellOccupied(pos, true);
        }
        for(int i = 0; i < numTowers; ++i) {
            auto pos = field.GetRandomFreePosition();
            towerManager.AddTowerAtPos(pos, 3, 15); 
            field.SetCellOccupied(pos, true);
        }
    
}

GameManager::GameManager(dataPlayer p, dataHand h, int width, int height, int hp)
    : enemyManager(), buildingManager(), field(width, height), towerManager(), allyManager(){
        p.health = 100;
        playerManager = PlayerManager(p);
        field.ChangeisOccupiedAtpos(playerManager.GetPlayerPosition());
        std::random_device rd;
        auto vec = h.spells;
        std::mt19937 gen(rd());
        std::shuffle(vec.begin(), vec.end(), gen);
        int s = vec.size() / 2;
        int new_size = std::max(s, 1);
        vec.resize(new_size);
        h.spells=vec;
        hand = Hand(h);
        hand.SellectSpell(0);
        for(int i = 0; i < 5; ++i) {
            auto pos = field.GetRandomFreePosition();
            enemyManager.AddEnemyAtPos(hp, 10, pos); 
            field.SetCellOccupied(pos, true);
        }
        for(int i = 0; i < 3; ++i) {
            auto pos = field.GetRandomFreePosition();
            buildingManager.AddBuildingAtPos(pos, 5); 
            field.SetCellOccupied(pos, true);
        }
        for(int i = 0; i < 2; ++i) {
            auto pos = field.GetRandomFreePosition();
            towerManager.AddTowerAtPos(pos, 3, 15); 
            field.SetCellOccupied(pos, true);
        }

}

GameManager::GameManager(savedata data){
    playerManager = PlayerManager(data.player);
    enemyManager = EnemyManager(data.enemyManager);
    buildingManager = BuildingManager(data.buildingManager);
    towerManager = TowerManager(data.towerManager);
    allyManager = AllyManager(data.allyManager);
    field = Field(data.field);
    hand = Hand(data.hand);
}

GameManager::~GameManager() = default;

void GameManager::UpdateState() {
    std::vector<std::pair<int,int>> allyPos = allyManager.MoveAllys(field);
    for (size_t i = 0; i < allyPos.size(); i += 2) {
        if(!field.IsOccupiedAtPos(allyPos[i + 1])) {
            field.ChangeisOccupiedAtpos(allyPos[i]);
            field.ChangeisOccupiedAtpos(allyPos[i + 1]);
            if (field.GetTrapDamageAtPos(allyPos[i + 1]) > 0) {
                allyManager.TakeAllyDamageAtPos(allyPos[i + 1], field.GetTrapDamageAtPos(allyPos[i + 1]));
                field.SetTrapAtPos(allyPos[i + 1], 0); 
                const auto& allies = allyManager.GetAllies();
                auto it = allies.find(allyPos[i + 1]);
                if (it != allies.end() && it->second.GetHealth() <= 0) {
                    allyManager.RemoveAllyAtPos(allyPos[i + 1]);
                    field.ChangeisOccupiedAtpos(allyPos[i + 1]);
                }
            }
        } else {
            if(enemyManager.IsEnemyAtPos(allyPos[i + 1])) {
                enemyManager.TakeEnemyDamageAtPos(allyPos[i + 1], allyManager.GetAllyDamageAtPos(allyPos[i]));
                const auto& enemies = enemyManager.GetEnemies();
                auto it = enemies.find(allyPos[i + 1]);
                if (it != enemies.end() && it->second.GetHealth() <= 0) {
                    enemyManager.RemoveEnemyAtPos(allyPos[i + 1]);
                    field.ChangeisOccupiedAtpos(allyPos[i + 1]);
                    playerManager.AddPlayerScore(10); 
                }
            } else if(buildingManager.IsBuildingAtPos(allyPos[i + 1])) {
                buildingManager.TakeBuildingDamageAtPos(allyPos[i + 1], allyManager.GetAllyDamageAtPos(allyPos[i]));
                if(!buildingManager.IsBuildiAliveAtPos(allyPos[i + 1])) {
                    buildingManager.RemoveBuildingAtPos(allyPos[i + 1]);
                    field.ChangeisOccupiedAtpos(allyPos[i + 1]);
                    playerManager.AddPlayerScore(20); 
                }
            } else if(towerManager.IsTowerAtPos(allyPos[i + 1])) {
                towerManager.TakeTowerDamageAtPos(allyPos[i + 1], allyManager.GetAllyDamageAtPos(allyPos[i]));
                if(!towerManager.IsTowerAliveAtPos(allyPos[i + 1])) {
                    towerManager.RemoveTowerAtPos(allyPos[i + 1]);
                    field.ChangeisOccupiedAtpos(allyPos[i + 1]);
                    playerManager.AddPlayerScore(20); 
                }
            }
        }
    }

    const std::vector<std::pair<int,int>> enemiesPos = enemyManager.MoveEnemies(field);
    for (size_t i = 0; i + 1 < enemiesPos.size(); i += 2) {
        if(!field.IsOccupiedAtPos(enemiesPos[i + 1])) {
            field.ChangeisOccupiedAtpos(enemiesPos[i]);
            field.ChangeisOccupiedAtpos(enemiesPos[i + 1]);
            if (field.GetTrapDamageAtPos(enemiesPos[i + 1]) > 0) {
                enemyManager.TakeEnemyDamageAtPos(enemiesPos[i + 1], field.GetTrapDamageAtPos(enemiesPos[i + 1]));
                field.SetTrapAtPos(enemiesPos[i + 1], 0); 
                const auto& enemies = enemyManager.GetEnemies();
                auto it = enemies.find(enemiesPos[i + 1]);
                if (it != enemies.end() && it->second.GetHealth() <= 0) {
                    enemyManager.RemoveEnemyAtPos(enemiesPos[i + 1]);
                    field.ChangeisOccupiedAtpos(enemiesPos[i + 1]);
                    playerManager.AddPlayerScore(10); 
                }
            }
        } else {
            if(enemiesPos[i + 1] == playerManager.GetPlayerPosition()) {
                playerManager.TakePlayerDamage(enemyManager.GetEnemyDamageAtPos(enemiesPos[i]));
            } else if(allyManager.IsAllyAtPos(enemiesPos[i + 1])) {
                allyManager.TakeAllyDamageAtPos(enemiesPos[i + 1], enemyManager.GetEnemyDamageAtPos(enemiesPos[i]));
                const auto& allies = allyManager.GetAllies();
                auto it = allies.find(enemiesPos[i + 1]);
                if (it != allies.end() && it->second.GetHealth() <= 0) {
                    allyManager.RemoveAllyAtPos(enemiesPos[i + 1]);
                    field.ChangeisOccupiedAtpos(enemiesPos[i + 1]);
                }
            }
        }   
    }

    auto readySpawners = buildingManager.UpdateBuildings();
    for (const auto& pos : readySpawners) {
        auto spawnPos = field.FindFreeAdjacentPosition(pos);
        if (spawnPos != pos && !field.IsOccupiedAtPos(spawnPos)) {
            enemyManager.AddEnemyAtPos(100, 10, spawnPos);
            field.SetCellOccupied(spawnPos, true);
        }
    }

    int playerScore = playerManager.GetPlayerScore();
    if (playerScore >= 50) {
        AddRandomSpellToHand();
        playerManager.AddPlayerScore(-50);
    }
    //allyPos={};
    allyPos.clear();
    allyPos.push_back(playerManager.GetPlayerPosition());
    for(auto [pos,ally] : allyManager.GetAllies()) {
        allyPos.push_back(pos);
    }
    std::vector<std::pair<int,int>> towerAttacks = towerManager.UpdateTowers(allyPos);
    if (!towerAttacks.empty()) {
        int damage = towerAttacks[0].second;
        for (size_t i = 1; i < towerAttacks.size(); ++i){
            if(playerManager.GetPlayerPosition().first == towerAttacks[i].first && playerManager.GetPlayerPosition().second == towerAttacks[i].second) {
                playerManager.TakePlayerDamage(damage);
            } else if(allyManager.IsAllyAtPos(towerAttacks[i])) {
                allyManager.TakeAllyDamageAtPos(towerAttacks[i], damage);
                const auto& allies = allyManager.GetAllies();
                auto it = allies.find(towerAttacks[i]);
                if (it != allies.end() && it->second.GetHealth() <= 0) {
                    allyManager.RemoveAllyAtPos(towerAttacks[i]);
                    field.ChangeisOccupiedAtpos(towerAttacks[i]);
                }
            }
        }
    }
}


void GameManager::MovePlayer(Direction direction) {
    if (playerManager.IsPlayerStunned()) {
        playerManager.DecreasePlayerStunDuration();
        return;
    }

    auto currentPos = playerManager.GetPlayerPosition();
    std::pair<int,int> newPos = currentPos;
    switch (direction) {
        case Direction::Up:    newPos.second -= 1; break;
        case Direction::Down:  newPos.second += 1; break;
        case Direction::Left:  newPos.first -= 1;  break;
        case Direction::Right: newPos.first += 1;  break;
    }
    if (field.inBounds(newPos) && field.GetCellTypeAt(newPos) != CellType::Wall && !field.IsOccupiedAtPos(newPos)) {
        playerManager.SetPlayerPosition(newPos);
        field.ChangeisOccupiedAtpos(currentPos);
        field.ChangeisOccupiedAtpos(newPos);

        if (field.GetCellTypeAt(newPos) == CellType::Stun) {
            playerManager.SetPlayerStunDuration(1);
        }
    }
    if (field.GetTrapDamageAtPos(newPos) > 0){
        playerManager.TakePlayerDamage(field.GetTrapDamageAtPos(newPos));
        field.SetTrapAtPos(newPos,0);
    }
    
}

void GameManager::PrintPlayerHealth() const {
    std::cout << "Player Health: " << playerManager.GetPlayerHealth() << std::endl;
}

void GameManager::PrintEnemiesHealth() const {
    const auto& enemies = enemyManager.GetEnemies();
    for (const auto& [pos, enemy] : enemies) {
        std::cout << "Enemy at (" << pos.first << ", " << pos.second << ") Health: " << enemy.GetHealth() << std::endl;
    }
}

void GameManager::SwitchAttackMode() {
    playerManager.SwitchPlayerAttackMode();
}

void GameManager::PlayerAttack(Direction direction) {
    auto playerPos = playerManager.GetPlayerPosition();
    int range = playerManager.GetPlayerAttackRange();
    int damage = playerManager.GetPlayerDamage();

    std::pair<int,int> step{0,0};
    switch (direction) {
        case Direction::Up:    step = {0, -1}; break;
        case Direction::Down:  step = {0,  1}; break;
        case Direction::Left:  step = {-1, 0}; break;
        case Direction::Right: step = { 1, 0}; break;
    }

    std::pair<int,int> target = playerPos;
    for (int i = 1; i <= range; ++i) {
        target.first += step.first;
        target.second += step.second;

        if (!field.inBounds(target)) break;
        if (field.GetCellTypeAt(target) == CellType::Wall) break;

        if (enemyManager.IsEnemyAtPos(target)) {
            enemyManager.TakeEnemyDamageAtPos(target, damage);

            const auto& enemies = enemyManager.GetEnemies();
            auto it = enemies.find(target);
            if (it != enemies.end() && it->second.GetHealth() <= 0) {
                enemyManager.RemoveEnemyAtPos(target);
                field.ChangeisOccupiedAtpos(target);
                playerManager.AddPlayerScore(10); 
            }
            break;
        }
    }
}

bool GameManager::IsGameOver() const {
    return !playerManager.IsPlayerAlive();
}

void GameManager::PlayerCastSpell(std::pair<int,int> target) {
    auto playerPos = playerManager.GetPlayerPosition();
    SpellContext ctx{
        playerPos,
        target,
        field,
        enemyManager,
        buildingManager,
        playerManager,
        allyManager,
        towerManager,
        hand
    };
    auto spell = hand.GetCurrentSpellType();
    if (spell == SpellType::none) return;
    hand.UseSpell(ctx);
}

void GameManager::AddRandomSpellToHand() {
    hand.AddRandomSpell();
}

void GameManager::SelectSpellInHand(int index) {
    hand.SellectSpell(index);
}

void GameManager::HandAddSpell(std::shared_ptr<Spell> spell) {
    hand.AddSpell(spell);
}

const Field& GameManager::GetField() const {
    return field;
}

const PlayerManager& GameManager::GetPlayer() const {
    return playerManager;
}

const EnemyManager& GameManager::GetEnemies() const {
    return enemyManager;
}

const BuildingManager& GameManager::GetBuildings() const {
    return buildingManager;
}

const TowerManager& GameManager::GetTowers() const {
    return towerManager;
}

const AllyManager& GameManager::GetAllies() const {
    return allyManager;
}

const Hand& GameManager::GetHand() const {
    return hand;
}

savedata GameManager::GetDataSave(){
    savedata data;
    data.field = field.Save();
    data.player = playerManager.Save();
    data.enemyManager = enemyManager.Save();
    data.buildingManager = buildingManager.Save();
    data.towerManager = towerManager.Save();
    data.allyManager = allyManager.Save();
    data.hand = hand.Save();

    return data;
}

dataPlayer GameManager::GetDataPlayer(){
    return playerManager.Save();
}

dataHand GameManager::GetDataHand(){
    return hand.Save();
}

void GameManager::ImproveAllSpells(){
    hand.ImproveAllSpells();
}