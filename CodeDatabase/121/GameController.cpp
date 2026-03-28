#include "GameController.h"
#include "SaveSystem.h"
#include "GameExceptions.h"
#include <iostream>
#include <limits>
#include <random>
#include <algorithm>

GameController::GameController() 
    : gameConfig(),
      textSystem(std::make_shared<TextSystem>()),
      inputSystem(),
      actionSystem(textSystem),
      renderSystem(textSystem),
      gameManager(inputSystem, actionSystem),
      gameVisualizer(renderSystem),
      currentLevel(nullptr),
      upgradeSystem(textSystem),
      spellShop(textSystem, gameConfig),
      saveSystem(textSystem, "savegame.json"),
      currentState(GameState::MAIN_MENU),
      currentLevelNumber(1),
      turnCount(0),
      gameRunning(true) {}

void GameController::handleCriticalError(const std::string& errorMessage) {
    std::cerr << textSystem->getCriticalErrorTitle() << std::endl;
    std::cerr << errorMessage << std::endl;
    std::cout << textSystem->getPressEnterToExit();
    std::cin.get();
    gameRunning = false;
}

void GameController::createFallbackLevel() {
    try {
        LevelType levelType = LevelType::FOREST;
        if (currentLevelNumber == 2) levelType = LevelType::CASTLE;
        else if (currentLevelNumber >= 3) levelType = LevelType::DUNGEON;
        
        currentLevel = std::make_unique<LevelController>(currentLevelNumber, levelType, gameConfig);
        std::cout << textSystem->getLevelCreatedMessage() << std::endl;
    } catch (const LevelGenerationException& e) {
        std::cout << textSystem->getCriticalLevelError() << std::endl;
        currentState = GameState::EXIT;
    }
}

void GameController::run() {
    try {
        while (gameRunning) {
            switch (currentState) {
                case GameState::MAIN_MENU:
                    mainMenu();
                    break;
                case GameState::PLAYING:
                    gameLoop();
                    break;
                case GameState::LEVEL_COMPLETE:
                    handleLevelComplete();
                    break;
                case GameState::GAME_OVER:
                    handleGameOver();
                    break;
                case GameState::UPGRADING:
                    showUpgradeMenu();
                    break;
                case GameState::SHOPPING:
                    showSpellShop();
                    break;
                case GameState::EXIT:
                    gameRunning = false;
                    break;
            }
        }
    } catch (const GameException& e) {
        handleCriticalError(e.what());
    } catch (const std::exception& e) {
        handleCriticalError(textSystem->getStandardErrorTitle() + ": " + e.what());
    } catch (...) {
        handleCriticalError(textSystem->getUnknownErrorTitle());
    }
}

void GameController::mainMenu() {
    while (currentState == GameState::MAIN_MENU) {
        std::cout << "=== СУперИграПупер ===" << std::endl;
        std::cout << "1. Новая игра" << std::endl;
        std::cout << "2. Выйти" << std::endl;
        std::cout << "3. Загрузить игру" << std::endl;  
        std::cout << "Выберите действие: ";
        
        std::string input;
        std::cin >> input;
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (input == "1") {
            startNewGame();
        } else if (input == "2") {
            currentState = GameState::EXIT;
        } else if (input == "3") {
            loadGame();  
        } else {
            std::cout << textSystem->getInvalidChoice() << std::endl;
        }
    }
}

void GameController::startNewGame() {
    currentLevelNumber = 1;
    LevelType startLevelType = LevelType::FOREST;
    
    try {
        currentLevel = std::make_unique<LevelController>(currentLevelNumber, startLevelType, gameConfig);
        turnCount = 0;
        currentState = GameState::PLAYING;
        std::cout << textSystem->getNewGameMessage(currentLevelNumber) << std::endl;
        
    } catch (const LevelGenerationException& e) {
        std::cout << e.what() << std::endl;
        std::cout << textSystem->getTryAgainMessage() << std::endl;
        currentState = GameState::MAIN_MENU;
    } catch (const ConfigException& e) {
        std::cout << e.what() << std::endl;
        std::cout << textSystem->getRestartGameMessage() << std::endl;
        currentState = GameState::EXIT;
    }
}

void GameController::loadGame() {
    try {
        if (!currentLevel) {
            createFallbackLevel();
        }
        
        saveSystem.loadGame(gameConfig, *currentLevel, currentLevelNumber, currentState, turnCount);
        
    } catch (const SaveLoadException& e) {
        std::cout << textSystem->getLoadErrorMessage(e.what()) << std::endl;
        createFallbackLevel();
    }
}

void GameController::saveGame() {
    try {
        if (!currentLevel) return;
        
        saveSystem.saveGame(*currentLevel, currentLevelNumber, currentState, turnCount);
        std::cout << textSystem->getSaveSuccessMessage() << std::endl;
        
    } catch (const SaveLoadException& e) {
        std::cout << textSystem->getSaveErrorMessage(e.what()) << std::endl;
    }
}

void GameController::gameLoop() {
    while (currentState == GameState::PLAYING && gameRunning) {
        turnCount++;
        
        try {
            gameVisualizer.renderFullGame(*currentLevel, turnCount);
            gameVisualizer.renderCommands();
            
            processPlayerTurn();
            if (currentState != GameState::PLAYING) break;
            
            if (currentLevel->isCompleted()) {
                currentState = GameState::LEVEL_COMPLETE;
                break;
            }
            
            if (currentLevel->isFailed()) {
                currentState = GameState::GAME_OVER;
                break;
            }
            
            processAITurn();
            
            if (currentLevel->isFailed()) {
                currentState = GameState::GAME_OVER;
                break;
            }
            
        } catch (const AIException& e) {
            gameVisualizer.renderMessage("Предупреждение ИИ: " + std::string(e.what()));
        } catch (const FieldException& e) {
            gameVisualizer.renderMessage("Ошибка поля: " + std::string(e.what()));
            currentState = GameState::GAME_OVER;
            break;
        } catch (const CombatException& e) {
            gameVisualizer.renderMessage("Ошибка боя: " + std::string(e.what()));
        }
    }
}

void GameController::processPlayerTurn() {
    auto& player = currentLevel->getPlayer();
    player.startNewTurn();
    
    if (player.getSlowed()) {
        if (player.applyFrozenEffect()) {
            gameVisualizer.renderMessage("Игрок заморожен и пропускает ход!");
            return;
        }
    }
    
    bool turnCompleted = false;
    
    while (!turnCompleted && currentState == GameState::PLAYING && player.isAlive()) {
        Command command = gameManager.getPlayerCommand();
        
        switch (command) {
            case Command::SHOP:
                currentState = GameState::SHOPPING;
                turnCompleted = true;
                break;
                
            case Command::EXIT_GAME:
                currentState = GameState::EXIT;
                turnCompleted = true;
                break;
                
            case Command::SAVE_GAME:  
                saveGame();
                gameVisualizer.renderMessage("Игра сохранена!");
                break;
                
            case Command::LOAD_GAME:  
                loadGame();
                if (currentState != GameState::PLAYING) {
                    turnCompleted = true;
                }
                break;
                
            case Command::SHOW_SPELLS:
                actionSystem.showSpells(*currentLevel);
                break;
                
            case Command::UNKNOWN:
                gameVisualizer.renderMessage("Неверная команда!");
                break;
                
            default:
                turnCompleted = gameManager.processCommand(command, *currentLevel);
                break;
        }
        
        if (currentLevel->isCompleted()) {
            currentState = GameState::LEVEL_COMPLETE;
            turnCompleted = true;
        } else if (currentLevel->isFailed()) {
            currentState = GameState::GAME_OVER;
            turnCompleted = true;
        }
    }
}

void GameController::processAITurn() {
    try {
        currentLevel->processEnemyTurns();
        currentLevel->processBuildingTurns();
        currentLevel->processTowerTurns();
    } catch (const AIException& e) {
        throw AIException("Ошибка обработки ходов ИИ: " + std::string(e.what()));
    }
}

void GameController::handleLevelComplete() {
    std::cout << textSystem->getLevelCompleteMessage(currentLevelNumber) << std::endl;
    currentState = GameState::UPGRADING;
}

void GameController::showSpellShop() {
    if (!currentLevel) return;
    
    bool inShop = true;
    
    while (inShop && currentState == GameState::SHOPPING) {
        auto& player = currentLevel->getPlayer();
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        spellShop.showShop(player);
        
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            continue;
        }
        
        bool isNumber = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isNumber = false;
                break;
            }
        }
        
        if (!isNumber) {
            std::cout << textSystem->getInvalidChoice() << std::endl;
            continue;
        }
        
        int choice = std::stoi(input);
        
        if (choice == 0) {
            inShop = false;
        } else if (choice >= 1 && choice <= 3) {
            spellShop.purchaseSpell(player, choice - 1);
            gameVisualizer.renderFullGame(*currentLevel, turnCount);
        } else {
            std::cout << textSystem->getInvalidChoice() << std::endl;
        }
    }
    
    currentState = GameState::PLAYING;
}

void GameController::showUpgradeMenu() {
    if (!currentLevel) return;

    auto& oldPlayer = currentLevel->getPlayer();
    
    int currentHealth = oldPlayer.getHealth();
    int currentDamage = oldPlayer.getDamage();
    int currentScore = oldPlayer.getScore();
    BattleMode currentBattleMode = oldPlayer.getBattleMode();
    
    auto& oldHand = oldPlayer.getHand();
    std::vector<std::shared_ptr<Spell>> savedSpells;
    
    size_t totalSpells = oldHand.getSpellCount();
    
    if (totalSpells > 1) {
        size_t spellsToKeep = totalSpells / 2;
        
        std::cout << "При переходе на следующий уровень " << totalSpells 
                  << " заклинаний сокращается до " << spellsToKeep << std::endl;
        
        std::vector<size_t> spellIndices;
        for (size_t i = 0; i < totalSpells; i++) {
            spellIndices.push_back(i);
        }
        
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(spellIndices.begin(), spellIndices.end(), g);
        
        for (size_t i = 0; i < spellsToKeep; i++) {
            size_t spellIndex = spellIndices[i];
            if (const Spell* spell = oldHand.getSpell(spellIndex)) {
                if (dynamic_cast<const DirectDamageSpell*>(spell)) {
                    savedSpells.push_back(std::make_shared<DirectDamageSpell>());
                    std::cout << "✓ Сохранено заклинание: Магический выстрел" << std::endl;
                } else if (dynamic_cast<const AreaDamageSpell*>(spell)) {
                    savedSpells.push_back(std::make_shared<AreaDamageSpell>());
                    std::cout << "✓ Сохранено заклинание: Огненный шар" << std::endl;
                } else if (dynamic_cast<const TrapSpell*>(spell)) {
                    savedSpells.push_back(std::make_shared<TrapSpell>());
                    std::cout << "✓ Сохранено заклинание: Ловушка" << std::endl;
                }
            }
        }
    } else if (totalSpells == 1) {
        std::cout << "При переходе на следующий уровень 1 заклинание будет утеряно." << std::endl;
    } else {
        std::cout << "У вас нет заклинаний для сохранения." << std::endl;
    }
    
    bool upgradeSelected = false;
    
    while (!upgradeSelected) {
        upgradeSystem.showUpgradeMenu(oldPlayer);
        
        int choice;
        std::cin >> choice;
        
        if (choice == 0) {
            upgradeSelected = true;
        } else if (choice >= 1 && choice <= 3) {
            UpgradeType selectedUpgrade;
            switch(choice) {
                case 1: selectedUpgrade = UpgradeType::HEALTH_BOOST; break;
                case 2: selectedUpgrade = UpgradeType::DAMAGE_BOOST; break;
                case 3: selectedUpgrade = UpgradeType::SPELL_POWER; break;
                default: selectedUpgrade = UpgradeType::HEALTH_BOOST; break;
            }
            
            int oldHealth = oldPlayer.getHealth();
            int oldDamage = oldPlayer.getDamage();
            int oldScore = oldPlayer.getScore();
            
            upgradeSystem.applyUpgrade(oldPlayer, selectedUpgrade);
            
            if (oldPlayer.getScore() < oldScore) {
                upgradeSelected = true;
                currentHealth = oldPlayer.getHealth();
                currentDamage = oldPlayer.getDamage();
                currentScore = oldPlayer.getScore();
            } else {
                oldPlayer.setHealth(oldHealth);
                oldPlayer.setDamage(oldDamage);
                oldPlayer.setScore(oldScore);
                std::cout << textSystem->getInvalidChoice() << std::endl;
            }
        } else {
            std::cout << textSystem->getInvalidChoice() << std::endl;
        }
    }
    
    currentLevelNumber++;
    LevelType nextType = LevelType::FOREST;
    if (currentLevelNumber == 2) nextType = LevelType::CASTLE;
    else if (currentLevelNumber >= 3) nextType = LevelType::DUNGEON;
    
    try {
        currentLevel = std::make_unique<LevelController>(currentLevelNumber, nextType, gameConfig);
        
        auto& newPlayer = currentLevel->getPlayer();
        newPlayer.setHealth(currentHealth);
        newPlayer.setDamage(currentDamage);
        newPlayer.setScore(currentScore);
        newPlayer.setBattleMode(currentBattleMode);
        newPlayer.setSlowed(false);
        newPlayer.setCanAct(true);
        
        auto& newHand = newPlayer.getHand();
        
        while (newHand.getSpellCount() > 0) {
            newHand.removeSpell(0);
        }
        
        for (auto& spell : savedSpells) {
            if (newHand.hasSpace()) {
                newHand.addSpell(spell);
            }
        }
        
        std::cout << "На новом уровне у вас " << newHand.getSpellCount() << " заклинаний" << std::endl;
        
        currentState = GameState::PLAYING;
        std::cout << "Начался уровень " << currentLevelNumber << "!" << std::endl;
        
    } catch (const LevelGenerationException& e) {
        std::cout << textSystem->getNextLevelError(e.what()) << std::endl;
        currentState = GameState::GAME_OVER;
    }
}

void GameController::handleGameOver() {
    if (currentLevel) {
        bool victory = currentLevel->isCompleted();
        gameVisualizer.renderGameOver(victory, currentLevel->getPlayer());
    }
    
    bool choiceMade = false;
    while (!choiceMade) {
        std::cout << textSystem->getGameOverMenu() << std::endl;
        
        std::string input;
        std::cin >> input;
        
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (input == "1") {
            startNewGame();
            choiceMade = true;
        } else if (input == "2") {
            currentState = GameState::EXIT;
            choiceMade = true;
        } else if (input == "3") {
            loadGame();
            choiceMade = true;
        } else {
            std::cout << textSystem->getInvalidChoice() << std::endl;
        }
    }
}