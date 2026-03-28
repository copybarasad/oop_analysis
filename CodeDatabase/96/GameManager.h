#pragma once
#include <vector>
#include <memory>
#include "Field.h"
#include "EnemyManager.h"
#include "BuildingManager.h"
#include "PlayerManager.h"
#include "Hand.h"
#include "TowerManager.h"
#include "AllyManager.h"
#include "struct/savedata.h"

class GameManager {
private:
    PlayerManager playerManager;
    EnemyManager enemyManager;
    BuildingManager buildingManager;
    TowerManager towerManager;
    AllyManager allyManager;
    Field field;
    Hand hand;

    
public:
    GameManager(int width, int height, int numEnemies, int numBuildings, int numTowers);
    GameManager(dataPlayer p, dataHand h, int width, int height, int hp);
    GameManager(savedata data);
    ~GameManager();


	void UpdateState();
    void MovePlayer(Direction direction);
    void PrintPlayerHealth() const;
    void PrintEnemiesHealth() const;
    void SwitchAttackMode();
    void PlayerAttack(Direction direction);
    bool IsGameOver() const;

    void PlayerCastSpell(std::pair<int,int> target);
    void AddRandomSpellToHand();
    void SelectSpellInHand(int index);
    void HandAddSpell(std::shared_ptr<Spell> spell);
    const Field& GetField() const;
    const PlayerManager& GetPlayer() const;
    const EnemyManager& GetEnemies() const;
    const BuildingManager& GetBuildings() const;
    const TowerManager& GetTowers() const;
    const AllyManager& GetAllies() const;
    const Hand& GetHand() const;
    int GetMurders() {return playerManager.GetMurders();}
    void ImproveAllSpells();
    savedata GetDataSave();
    dataPlayer GetDataPlayer();
    dataHand GetDataHand();
};