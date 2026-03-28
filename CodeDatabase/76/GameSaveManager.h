#pragma once  
#include "Player.h"
#include "Enemy.h"
#include "PlayingField.h"
#include "SpellHand.h"
#include "SpellCard.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "EnemyTower.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

class Game;

class GameSaveManager 
{
private:
    Game* game;

public:
    GameSaveManager(Game* gameInstance);
    
    bool SaveGame(const string& filename);
    bool LoadGame(const string& filename);
    void ShowSaveMenu();
    void ShowLoadMenu();
    bool ShowLoadMenuAtStart();

private:
    string SerializeGameState();
    bool DeserializeGameState(const string& data);
    void ClearGameState();
    vector<string> GetSaveFiles();
};

