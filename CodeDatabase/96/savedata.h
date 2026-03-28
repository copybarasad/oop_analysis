#pragma once
#include "dataField.h"
#include "dataPlayer.h"
#include "dataEnemyManager.h"
#include "dataBuildingManager.h"
#include "dataTowerManager.h"
#include "dataAllyManager.h"
#include "dataHand.h"

struct savedata{
    int currentLevel;
    int improvehealth;
    int improvehand;
    dataField field;
    dataPlayer player;
    dataEnemyManager enemyManager;
    dataBuildingManager buildingManager;
    dataTowerManager towerManager;
    dataAllyManager allyManager;
    dataHand hand;
};