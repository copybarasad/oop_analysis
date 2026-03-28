#ifndef GAME_H
#define GAME_H

#include "Command.h"
#include "GameStatus.h"  
#include "GameController.h"
#include "GameSaveSystem.h"
#include "LevelGenerator.h"
#include <memory>

// Основной класс игры, реализующий игровую логику
class Game {
private:
    std::unique_ptr<GameController> currentLevel;
    GameStatus currentStatus;
    int currentLevelNumber;
    int playerMaxHealth;
    std::unique_ptr<GameSaveSystem> saveSystem;
    std::unique_ptr<LevelGenerator> levelGenerator;
    bool gameRunning;
    
    void initLevel(int level);
    bool load();
    bool save();
    void handleTransition();
    
    // Вспомогательные методы для обработки команд
    bool handlePlayingCommand(CmdType cmdType);
    bool handleGameOverCommand(CmdType cmdType);
    bool handleLevelCompleteCommand(CmdType cmdType);
    
public:
    Game();
    ~Game() = default;
    
    // Основные методы
    void update();
    bool execute(const Command& command);
    bool isRunning() const { return gameRunning; }
    bool processInput(char input);
    
    // Геттеры
    GameStatus getStatus() const { return currentStatus; }
    int getLevel() const { return currentLevelNumber; }
    int getScore() const {
        return currentLevel ? currentLevel->getPlayer().getScore() : 0;
    }
    const GameController* getController() const { return currentLevel.get(); }
    
    // Сеттеры
    void setStatus(GameStatus status) { currentStatus = status; }
    void setRunning(bool running) { gameRunning = running; }
    
    // Методы для рендереров
    const Player& getPlayer() const;
    const GameField& getField() const;
    
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(Game&&) = delete;
};

#endif