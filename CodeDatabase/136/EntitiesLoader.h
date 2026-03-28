#ifndef ENTITIESLOADER_H
#define ENTITIESLOADER_H

#include "BeastManager.h"
#include "BuildingManager.h"
#include "GameField.h"
#include "GameController.h"
#include "SpellFactory.h"
#include <fstream>

class EntitiesLoader {
private:
    std::ifstream& file;
    GameField& field;
    BeastManager& beastManager;
    BuildingManager& buildingManager;
    GameController& gameController;

public:
    EntitiesLoader(std::ifstream& file, GameField& field, BeastManager& beastManager, 
                  BuildingManager& buildingManager, GameController& controller);
    bool loadEntitiesData();
    
private:
    bool loadBeasts();
    bool loadBuildings();
    bool loadTowers();
    bool loadSpells();
    bool validateBeastData(int health, int damage) const;
    bool validateBuildingData(int health) const;
    bool validateTowerData(int range, int health) const;
};

#endif