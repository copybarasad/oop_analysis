#ifndef GAME_H
#define GAME_H

#include "Field.h"
#include "EntityManager.h"
#include "GameTypes.h"
#include "Spells.h"
#include "SaveManager.h"
#include "Logger.h"
#include <string>

class Game : public IObservable {
public:
    Game(int field_width, int field_height);
    
    void startGame();
    bool isRunning() const { return game_running_; }
    void checkGameStatus();

    const Field& getField() const { return field_; }
    EntityManager& getEntityManager() { return entity_manager_; }
    const EntityManager& getEntityManager() const { return entity_manager_; }
    GameState getCurrentState() const { return current_state_; }
    Point getCastingCursor() const { return casting_cursor_; }
    void setCastingCursor(const Point& pos) { casting_cursor_ = pos; }
    bool isCellFree(const Point& pos) const;

    void movePlayer(int dx, int dy);
    void enterCastingMode();
    void exitCastingMode();
    void selectSpell(int index);
    void castSelectedSpell();
    void selectLevelUpBonus(int choice);
    void printLevelUpOptions(); 
    void buySpell();
    void quit();
    void endPlayerTurn();
    
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    
    void nextLevel();
    void processLevelUp();

    void notifyLog(const std::string& msg);

private:
    void initializeGame();
    void updateGameState();
    bool isGameOver() const;
    bool hasWon() const;
    void printHelp() const;
    
    Field field_;
    EntityManager entity_manager_;
    SaveManager save_manager_;
    
    bool game_running_;
    GameState current_state_;
    Point casting_cursor_;
    int selected_spell_idx_ = -1;
    int current_level_ = 1;
};

#endif