#ifndef GAME_H
#define GAME_H

#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>
#include "field.h"
#include "player.h"

class Game {
private:
    std::unique_ptr<Field> field;
    int current_turn;
    bool game_over;
    int level;

public: 
    Game(int width, int height);  
    
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);
    

    int getPlayerHP() const;
    int getPlayerMaxHP() const;
    int getPlayerMana() const;
    int getPlayerPoints() const;
    std::vector<std::string> getPlayerSpells() const;
    void printGameState() const;

    int getCntEnemyWithGame();
    int getCntBuildingWithGame();
    void spellUp(int spell_numer);
    bool isGameOver();
    int getLevel() const;
    Player* getPlayer();
    int getEnemyCount() const;
    int getBuildingCount() const;
    
    bool movePlayer(int dx, int dy);
    bool attack(int dx, int dy);

    int getCurrentTurn();
    int getLevel();

    void incrementTurn();

    bool searchSpellPlayer(std::string spell_name);
    bool useSpell(std::string spell_name, int dx, int dy);
    void DelSpell(std::string spell_name);
    void castSpell(const std::string& spell, int x, int y);
    void switchToMelee();
    void switchToRanged();
    void levelUpPlayer();
    
    int getHpPlayer();
    void restorePlayerHP();
    Field* getField();

    static std::unique_ptr<Game> loadGame(std::string& filename);
    bool saveGame(std::string& filename);

    void enemy_move();
    void building_move(); 
    void spell_tower_move();
    void newField(bool increase_size);
    
    bool isLevelComplete() const;
    void completeLevel();
};

#endif