#include "Game.h"
#include "CommandParser.h"  
#include "HealSpell.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include "CommandParser.h"

Game::Game() 
    : currentStatus(GameStatus::MENU), 
      currentLevelNumber(1), 
      playerMaxHealth(100),
      saveSystem(new GameSaveSystem()),
      levelGenerator(new LevelGenerator()),
      gameRunning(true) {}

// Выполнение команды от обработчика ввода
bool Game::execute(const Command& command) {
    try {
        CmdType cmdType = command.getType();
        
        switch (currentStatus) {
            case GameStatus::MENU:
                switch (cmdType) {
                    case CmdType::MENU_SELECT_1:
                        currentLevelNumber = 1;
                        playerMaxHealth = 100;
                        initLevel(currentLevelNumber);
                        currentStatus = GameStatus::PLAYING;
                        return true;
                    case CmdType::MENU_SELECT_2:
                        if (load()) {
                            currentStatus = GameStatus::PLAYING;
                            return true;
                        } else {
                            currentLevelNumber = 1;
                            playerMaxHealth = 100;
                            initLevel(currentLevelNumber);
                            currentStatus = GameStatus::PLAYING;
                            return true;
                        }
                    case CmdType::MENU_SELECT_3:
                        currentStatus = GameStatus::QUIT;
                        gameRunning = false;
                        return true;
                    default:
                        return false;
                }
                break;
                
            case GameStatus::PLAYING:
                return handlePlayingCommand(cmdType);
                
            case GameStatus::GAME_OVER:
                return handleGameOverCommand(cmdType);
                
            case GameStatus::LEVEL_COMPLETE:
                return handleLevelCompleteCommand(cmdType);
        }
    } catch (const std::exception& e) {
        return false;
    }
    
    return false;
}

// Обработка команд в игровом режиме
bool Game::handlePlayingCommand(CmdType cmdType) {
    if (cmdType == CmdType::QUIT_GAME) {
        currentStatus = GameStatus::MENU;
        return true;
    } else if (cmdType == CmdType::SAVE_GAME) {
        save();
        return true;
    } else if (cmdType == CmdType::LOAD_GAME) {
        load();
        return true;
    } else {
        if (!currentLevel) return false;
        
        bool action = false;
        bool enemyTurn = false;
        
        switch (cmdType) {
            case CmdType::MOVE_UP:
                action = currentLevel->movePlayer(0, -1);
                enemyTurn = action;
                break;
            case CmdType::MOVE_DOWN:
                action = currentLevel->movePlayer(0, 1);
                enemyTurn = action;
                break;
            case CmdType::MOVE_LEFT:
                action = currentLevel->movePlayer(-1, 0);
                enemyTurn = action;
                break;
            case CmdType::MOVE_RIGHT:
                action = currentLevel->movePlayer(1, 0);
                enemyTurn = action;
                break;
            case CmdType::SWITCH_MODE:
                action = currentLevel->switchCombatMode();
                break;
            case CmdType::USE_SPELL_1:
                action = currentLevel->useSpell(0);
                enemyTurn = action && !currentLevel->isAwaitingSpellTarget();
                break;
            case CmdType::USE_SPELL_2:
                action = currentLevel->useSpell(1);
                enemyTurn = action && !currentLevel->isAwaitingSpellTarget();
                break;
            default:
                return false;
        }
        
        if (enemyTurn) {
            currentLevel->handleEnemyTurns();
        }
        return action;
    }
}

// Обработка команд в состоянии Game Over
bool Game::handleGameOverCommand(CmdType cmdType) {
    switch (cmdType) {
        case CmdType::MENU_SELECT_1:
            currentLevelNumber = 1;
            playerMaxHealth = 100;
            initLevel(currentLevelNumber);
            currentStatus = GameStatus::PLAYING;
            return true;
        case CmdType::MENU_SELECT_2:
            currentStatus = GameStatus::MENU;
            return true;
        default:
            return false;
    }
}

// Обработка команд при завершении уровня
bool Game::handleLevelCompleteCommand(CmdType cmdType) {
    switch (cmdType) {
        case CmdType::MENU_SELECT_1:
            handleTransition();
            currentStatus = GameStatus::PLAYING;
            return true;
        case CmdType::MENU_SELECT_2:
            if (save()) {
                currentStatus = GameStatus::MENU;
            }
            return true;
        default:
            return false;
    }
}

// Обработка символьного ввода 
bool Game::processInput(char input) {
    // Преобразуем символ в команду
    CommandParser parser;
    Command cmd = parser.parse(currentStatus, input);
    return execute(cmd);
}

// Обновление состояния игры
void Game::update() {
    if (!currentLevel || currentStatus != GameStatus::PLAYING) {
        return;
    }
    
    try {
        currentLevel->update();
        
        GameState levelState = currentLevel->getGameState();
        if (levelState == GameState::VICTORY) {
            currentStatus = GameStatus::LEVEL_COMPLETE;
        } else if (levelState == GameState::PLAYER_DIED) {
            currentStatus = GameStatus::GAME_OVER;
        }
    } catch (const std::exception& e) {
    }
}

// Получение ссылки на игрока
const Player& Game::getPlayer() const { 
    static Player dummy(100, 15, 10);
    return currentLevel ? currentLevel->getPlayer() : dummy;
}

// Получение ссылки на игровое поле
const GameField& Game::getField() const {
    static GameField dummy(15, 15);
    return currentLevel ? currentLevel->getField() : dummy;
}

// Инициализация уровня
void Game::initLevel(int level) {
    try {
        uint32_t seed = LevelGenerator::generateSeed(level);
        levelGenerator->setSeed(seed);
        
        int size = 15 + (level - 1);
        currentLevel.reset(new GameController(size, size));
        levelGenerator->generateLevel(*currentLevel, level);
        
        Player& player = currentLevel->getPlayer();
        player.setMaxHealthPublic(playerMaxHealth);
        player.setHealthPublic(playerMaxHealth);
        
    } catch (const std::exception& e) {
        throw; 
    }
}

// Сохранение игровых данных
bool Game::save() {
    if (!currentLevel) {
        return false;
    }
    
    try {
        return saveSystem->saveGame(*currentLevel, currentLevelNumber, playerMaxHealth);
    } catch (const std::exception& e) {
        return false;
    }
}

// Загрузка игровых данных
bool Game::load() {
    int loadedLevel;
    int loadedMaxHealth;
    
    std::unique_ptr<GameController> tempLevel(new GameController(15, 15));
    
    if (!saveSystem->loadGame(*tempLevel, loadedLevel, loadedMaxHealth)) {
        return false;
    }
    
    currentLevel = std::move(tempLevel);
    currentLevelNumber = loadedLevel;
    playerMaxHealth = loadedMaxHealth;
    
    return true;
}

// Обработка перехода между уровнями
void Game::handleTransition() {
    std::vector<std::string> savedSpells;
    if (currentLevel) {
        const Player& player = currentLevel->getPlayer();
        for (int i = 0; i < player.getHand().getSpellCount(); ++i) {
            const Spell* spell = player.getHand().getSpell(i);
            if (spell) {
                savedSpells.push_back(spell->getName());
            }
        }
    }
    
    if (currentLevel) {
        playerMaxHealth = currentLevel->getPlayer().getMaxHealth();
    }
    
    currentLevelNumber++;
    playerMaxHealth += 10;
    
    initLevel(currentLevelNumber);
    
    if (currentLevel && !savedSpells.empty()) {
        Player& player = currentLevel->getPlayer();
        Hand& hand = player.getHand();
        
        while (hand.getSpellCount() > 0) {
            hand.removeSpell(0);
        }
        
        if (!savedSpells.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(0, savedSpells.size() - 1);
            
            std::string spellName = savedSpells[dist(gen)];
            std::unique_ptr<Spell> spell;
            
            if (spellName == "Heal") {
                spell.reset(new HealSpell());
            } else if (spellName == "Firebolt") {
                spell.reset(new DirectDamageSpell());
            } else if (spellName == "Fireball") {
                spell.reset(new AreaDamageSpell());
            } else if (spellName == "Trap") {
                spell.reset(new TrapSpell());
            }
            
            if (spell) {
                player.addSpell(std::move(spell));
            }
        }
    }
    
    if (currentLevel) {
        Player& player = currentLevel->getPlayer();
        player.setHealthPublic(playerMaxHealth);
        player.setMaxHealthPublic(playerMaxHealth);
    }
}