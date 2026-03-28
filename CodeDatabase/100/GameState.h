#pragma once
#include <vector>
#include "../game_objects/GameField.h"
#include "../game_objects/Player.h"
#include "../game_objects/Character.h"
#include "../game_objects/EnemyBuilding.h"
#include "../game_objects/EnemyTower.h"
#include "../game_objects/Ally.h"
#include "../game_objects/PlayerHand.h"
#include <string>

class GameState {
private:
    GameField field;
    Player player;
    PlayerHand hand;
    std::vector<Character*> characters;
    std::vector<EnemyBuilding*> buildings;
    std::vector<std::string> messageLog;
    std::vector<EnemyTower*> towers;
    std::vector<Ally*> allies;
    bool gameRunning;
    int lastTrapDamage = 20; 
    int level = 1;

    int turnsSinceLastSpell;
    const int SPELL_DROP_INTERVAL = 7;

    void copyFrom(const GameState& other);

public:
    // Конструктор
    GameState(int fieldWidth, int fieldHeight, int playerHealth, int playerDamage);
    ~GameState();

    GameState(const GameState& other);                  
    GameState(GameState&& other) noexcept;              
    GameState& operator=(const GameState& other);       
    GameState& operator=(GameState&& other) noexcept;

    // Геттеры
    Player& getPlayer();
    const GameField& getField() const;
    const PlayerHand& getHand() const;
    const std::vector<Character*>& getAllCharacters() const;
    const std::vector<EnemyBuilding*>& getAllBuildings() const; 
    const std::vector<std::string>& getMessageLog() const; 
    const std::vector<EnemyTower*>& getAllTowers() const;
    const std::vector<Ally*>& getAllAllies() const; 
    bool isGameRunning() const;
    bool hasAliveEnemies() const;

    void setGameRunning(bool running);
    
    // Добавить персонажа / здание / башню / союзника
    void addCharacter(Character* character);
    void addBuilding(EnemyBuilding* building);
    void addTower(EnemyTower* tower); 
    void addAlly(Ally* ally);
    
    // Установить ловушку в клетку
    void setCellTrap(int x, int y, bool hasTrap);

    // Установить / Получить урон ловушки
    void setLastTrapDamage(int damage); 
    int getLastTrapDamage() const; 

    // Работа с логами
    void addMessage(const std::string& message);

    void clearMessageLog();

    bool addSpellToHand(std::shared_ptr<Spell> spell);
    bool useSpellFromHand(size_t spellIndex, int targetX, int targetY);
    bool upgradeSpellInHand(size_t spellIndex);
    bool removeSpellFromHand(size_t spellIndex);
    
    void incrementTurnsSinceLastSpell();
    void resetTurnsSinceLastSpell();
    bool shouldGiveSpell() const;

    void clearGameState();
    void clearHand();
    
    int getLevel() const;
    void setLevel(int lvl);
};