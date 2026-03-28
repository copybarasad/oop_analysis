#ifndef GAME_H
#define GAME_H

#include <iostream>       
#include <vector>         
#include <memory>         
#include <string>         
#include <fstream>        
#include <random>         
#include <sstream>        
#include <stdexcept>
#include <utility>
#include <cctype>      
#include "Field.h"        
#include "Player.h"       
#include "Enemy.h"        
#include "Ally.h"         
#include "Tower.h"        
#include "Trap.h"         
#include "Hand.h"         
#include "SaveManager.h"  
#include "LoadException.h" 
#include "UpgradeMenu.h"  
#include "Spell.h" 

class ControlConfig;
class GameEventObserver;

class Game {
public:
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(int width, int height, int numEnemies, int handCapacity = 5, int separatorLines = 3, int maxLevels = 5);

    bool run();  // Основной цикл игры 
    const Field& getField() const;  
    
    const Player& getPlayer() const { return player_; }
    const Hand& getHand() const { return hand_; }
    const std::vector<Enemy>& getEnemies() const { return enemies_; }
    const std::vector<Ally>& getAllies() const { return allies_; }
    const std::vector<Tower>& getTowers() const { return towers_; }
    const std::vector<Trap>& getTraps() const { return traps_; }
    int getTurn() const { return turn_; }
    int getLevel() const { return level_; }
    int getMaxLevels() const { return maxLevels_; }
    int getSeparatorLines() const { return separatorLines_; }
    
    // Методы для работы с ошибками (для рендерера)
    std::string getLastErrorMessage() const { return lastErrorMessage_; }
    void clearErrorMessage() { lastErrorMessage_.clear(); }
    
    // Методы для GameLoop
    void initializeTurn() { requestExitProgram_ = false; requestMenuExit_ = false; turn_ = 0; }
    void incrementTurn() { ++turn_; }
    bool isPlayerAlive() const { return player_.isAlive(); }
    void handlePlayerDeath();
    void handleLevelCleared();
    void setEventObserver(GameEventObserver* observer) { eventObserver_ = observer; }
    void alliesTurn();  
    void enemiesTurn();  

    bool isCellEmptyForMove(int x, int y) const;
    int findEnemyIndexAt(int x, int y) const;
    int findTowerIndexAt(int x, int y) const;
    void damageEnemy(int index, int dmg);
    void damageTower(int index, int dmg);
    bool addAlly(int x, int y);
    bool placeTrap(int x, int y, int damage);
    bool allEnemiesDefeated() const;

    void saveToStream(std::ostream& os) const;  
    void loadFromStream(std::istream& is);  
    void saveToFile(const std::string& filename) const;  
    void loadFromFile(const std::string& filename);  

    bool isLevelCleared() const; 
    void advanceToNextLevel();  
    void presentUpgradeMenu(); 
    
    // Установка конфигурации управления
    void setControlConfig(ControlConfig* config);
    // Получить конфигурацию управления (для Command)
    ControlConfig* getControlConfig() const { return controlConfig_; }
    
    // Методы для команд и рендереров
    void handlePlayerMove(std::string full_command, char extra_char);  
    void processMovementOrMeleeAttack(int dx, int dy);
    void setPlayerAttackMode(Player::AttackMode mode);  
    bool buyRandomSpell(); 
    bool useSpell(int index, int tx, int ty);  
    void performRangedAttack(char direction);  
    void requestMenuExit() { requestMenuExit_ = true; }
    void requestProgramExit() { requestExitProgram_ = true; }
    bool shouldExitProgram() const { return requestExitProgram_; }
    bool shouldExitMenu() const { return requestMenuExit_; }

private:
    Field field_;  
    Player player_;
    Hand hand_;  
    int separatorLines_;  
    static std::mt19937 rng_;  

    std::vector<Enemy> enemies_;  
    std::vector<Ally> allies_;  
    std::vector<Tower> towers_;  
    std::vector<Trap> traps_;  

    int turn_;  
    int level_;  
    int maxLevels_;  
    bool requestExitProgram_;  
    bool requestMenuExit_;  

    int baseEnemyHP_;  
    int baseEnemyDamage_;  
    int baseEnemyCount_;  

    void initialize(int numEnemies);  
    
    ControlConfig* controlConfig_;  
    std::pair<char, bool> translateKey(char key) const;  // Преобразование клавиши согласно настройкам 
    GameEventObserver* eventObserver_;  
    std::string lastErrorMessage_;  
};

#endif 
