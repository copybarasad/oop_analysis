#include "GameController.h"
#include "Input.h"
#include "CommandGameControl.h"
#include "Game.h"
#include "GameRenderer.h"
#include <iostream>
#include <string>


template<typename TInputHandler>
GameController<TInputHandler>::GameController(Game* gameInstance)
    : game(gameInstance), 
      inputHandler(std::make_unique<TInputHandler>()),
      isRunning(true) {
}


template<typename TInputHandler>
void GameController<TInputHandler>::runGameLoop() {
    std::cout << "=== НАЧАЛО УРОВНЯ " << game->getCurrentLevel() << " ===" << std::endl;
    std::cout << "Удачи, ковбой " << game->getPlayer().getName() << "!" << std::endl;
    
    while(!isGameOver()){  
        displayGameState();
        processPlayerTurn();
        if(!isRunning){
            return;
        }
        if (isGameOver()) {
            break;
        }
        waitForContinue();
    }
    displayGameResult();
}


template<typename TInputHandler>
bool GameController<TInputHandler>::isGameOver() const {
    return !game->getPlayer().isAlive() || game->getField().getEnemyCount() == 0 || !isRunning;
}

template<typename TInputHandler>
bool GameController<TInputHandler>::isVictory() const {
    return game->getField().getEnemyCount() == 0 && game->getPlayer().isAlive();
}

template<typename TInputHandler>
bool GameController<TInputHandler>::isDefeat() const {
    return !game->getPlayer().isAlive();
}

template<typename TInputHandler>
void GameController<TInputHandler>::displayGameState() const {
    std::cout << "\n=== ХОД " << game->getTurn() << " ===" << std::endl;
    
    game->getGameRender().displayField(game->getFieldRef());
    game->getGameRender().displayPlayer(game->getPlayerRef());
    game->getGameRender().displaySpells(game->getPlayerRef());

    auto& input = *inputHandler;
    
    std::cout << "Управление: " 
              << input.getCharForCommand("MOVE_UP") << "/"
              << input.getCharForCommand("MOVE_LEFT") << "/"
              << input.getCharForCommand("MOVE_DOWN") << "/"
              << input.getCharForCommand("MOVE_RIGHT") 
              << " - движение, "
              << input.getCharForCommand("ATTACK") 
              << " - атака, "
              << input.getCharForCommand("QUIT") 
              << " - выход, "
              << input.getCharForCommand("SWITCH_ATTACK") 
              << " - смена дальности боя" << std::endl;
    std::cout << input.getCharForCommand("USE_SPELL") 
              << " - атака способностями, "
              << input.getCharForCommand("OPEN_SHOP") 
              << " - магазин способностей" << std::endl;
    std::cout << input.getCharForCommand("SAVE") 
              << " - сохранить игру, "
              << input.getCharForCommand("LOAD") 
              << " - загрузить игру" << std::endl;
}

template<typename TInputHandler>
void GameController<TInputHandler>::waitForContinue() const {
    std::cout << "Нажмите Enter чтобы продолжить..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


template<typename TInputHandler>
void GameController<TInputHandler>::showMainMenu() {
    int choice = inputHandler->getMainMenuChoice();
    switch (choice) {
        case 1:
            startNewGame();
            break;
        case 2:
            showLoadMenu();
            break;
        case 3:
            quit();
            break;
    }
}


template<typename TInputHandler>
void GameController<TInputHandler>::showLoadMenu() {
    auto saves = game->getSaveManagerRef().getAvailableSaves();
    if (saves.empty()) {
        std::cout << "Сохраненные игры не найдены." << std::endl;
        showMainMenu();
        return;
    }
    
    int choice = inputHandler->getLoadChoice(saves);
    
    if (choice == 0) {
        showMainMenu();
        return;
    }
    
    if (choice > 0 && choice <= saves.size()) {
        try {
            int loadedTurn, loadedLevel;
            game->getSaveManagerRef().loadGame(saves[choice - 1], game->getPlayerRef(), game->getFieldRef(), loadedTurn, loadedLevel);
            game->getTurnRef() = loadedTurn;
            game->getCurrentLevelRef() = loadedLevel;
            std::cout << "Игра успешно загружена! Уровень: " << loadedLevel << std::endl;
            runGameLoop();
        } catch (const std::exception& e) {
            handleLoadException(e);
            showMainMenu();
        }
    } else {
        std::cout << "Неверный выбор!" << std::endl;
        showMainMenu();
    }
}


template<typename TInputHandler>
void GameController<TInputHandler>::handleLoadException(const std::exception& e) {
    std::cerr << "\n=== ОШИБКА ЗАГРУЗКИ ===" << std::endl;
    std::cerr << "Причина: " << e.what() << std::endl;
    std::cerr << "Возможные причины:" << std::endl;
    std::cerr << "1. Файл сохранения поврежден" << std::endl;
    std::cerr << "2. Файл был изменен вручную" << std::endl;
    std::cerr << "Рекомендуется создать новое сохранение." << std::endl;
}

template<typename TInputHandler>
void GameController<TInputHandler>::startNewGame() {
    std::string playerName = inputHandler->getPlayerName();
    game->getGameRunningRef() = true;
    game->getPlayerRef() = Player(playerName, 0, 0);
    
    int level = inputHandler->getLevelChoice();
    game->getCurrentLevelRef() = level;
    game->initializeLevel(level);
    
    game->getFieldRef().setCellType(0, 0, PLAYER);
    game->getTurnRef() = 1;
    
    runGameLoop();
}

template<typename TInputHandler>
void GameController<TInputHandler>::processPlayerTurn() {
    char input = inputHandler->getPlayerInput();
    bool playerActed = false;
    
    auto command = inputHandler->processInput(input, game->getPlayerRef(), game->getFieldRef(), *game);
    if (command) {
        command->execute();
        playerActed = true;
    }
    
    
    if (playerActed && game->isRunning() && game->getPlayer().isAlive() && 
        game->getField().getEnemyCount() > 0) {
        processEnemyTurns();
        game->getTurnRef()++;
    }
}

template<typename TInputHandler>
void GameController<TInputHandler>::processEnemyTurns() {
    auto& playerRef = game->getPlayerRef();
    auto& fieldRef = game->getFieldRef();
    

    if (playerRef.getHandSpell() && playerRef.getHandSpell()->getGhostJhonny()) {
        playerRef.getHandSpell()->getGhostJhonny()->setCurrentSpawns(fieldRef.arr_teamates.size());
    }
    
    fieldRef.processAllTurns(playerRef);
    
    if (playerRef.getHandSpell() && playerRef.getHandSpell()->getGhostJhonny()) {
        playerRef.getHandSpell()->getGhostJhonny()->setCurrentSpawns(fieldRef.arr_teamates.size());
    }
}

template<typename TInputHandler>
void GameController<TInputHandler>::displayGameResult() {
    if (isVictory()) {
        std::cout << "\nУРОВЕНЬ " << game->getCurrentLevel() << " ПРОЙДЕН!" << std::endl;
        
        if (game->getCurrentLevel() >= 3) {
            game->getGameRender().displayVictory(*game);
            waitForContinue();
            showMainMenu();
        } else {
            char choice = inputHandler->getLevelTransitionChoice();
            if (choice == 'y' || choice == 'Y') {
                advanceToNextLevel();
            } else {
                std::cout << "Возврат в главное меню..." << std::endl;
                showMainMenu();
            }
        }
        
    } else if (isDefeat()) {
        game->getGameRender().displayDefeat(*game);
        waitForContinue();
        showMainMenu();
    }
}

template<typename TInputHandler>
void GameController<TInputHandler>::quit() {
    isRunning = false;
    game->getGameRunningRef() = false;
    std::cout << "Выход из игры..." << std::endl;
}

template<typename TInputHandler>
void GameController<TInputHandler>::advanceToNextLevel() {
    auto& playerRef = game->getPlayerRef();
    auto& fieldRef = game->getFieldRef();
    
    game->getCurrentLevelRef()++;
    std::cout << "\n=== ПЕРЕХОД НА УРОВЕНЬ " << game->getCurrentLevel() << " ===" << std::endl;

    showLevelUpMenu();
    playerRef.setHealth(playerRef.getMaxHealth());
    std::cout << "Здоровье восстановлено!" << std::endl;
    
    removeHalfOfSpells();
    
    game->initializeLevel(game->getCurrentLevel());
    
    if (playerRef.getHandSpell() && playerRef.getHandSpell()->getGhostJhonny()) {
        playerRef.getHandSpell()->getGhostJhonny()->setCurrentSpawns(0);
    }
    
    playerRef.setPosition(0, 0);
    fieldRef.setCellType(0, 0, PLAYER);

    game->getTurnRef() = 1;
    
    std::cout << "Удачи на уровне " << game->getCurrentLevel() << "!" << std::endl;
    waitForContinue();
    
    runGameLoop();
}

template<typename TInputHandler>
void GameController<TInputHandler>::removeHalfOfSpells() {
    auto& playerRef = game->getPlayerRef();
    
    if (playerRef.getHandSpell()) {
        int totalSpells = playerRef.getHandSpell()->getCntTotalSpells();
        if (totalSpells == 0) {
            return;
        }
        int spellsToRemove = totalSpells / 2;
        playerRef.removeRandomSpells(spellsToRemove);
    }
}

template<typename TInputHandler>
void GameController<TInputHandler>::showLevelUpMenu() {
    auto& playerRef = game->getPlayerRef();
    
    int upgradesAvailable = MAX_UPGRADES_PER_LEVEL;
    
    game->getGameRender().displayFinishLevel(*game);
    
    for (int i = 0; i < upgradesAvailable; i++) {

        std::cout << "\n--- Улучшение " << (i + 1) << " из " << upgradesAvailable << " ---" << std::endl;

        game->getGameRender().displayUpgradeChoice(*game);
        
        int choice = inputHandler->getUpgradeChoice();
        
        switch (choice) {
            case 1: upgradeMeleeDamage(); break;
            case 2: upgradeRangedDamage(); break;
            case 3: upgradeAttackRange(); break;
        }
    }
    
    std::cout << "\nВсе улучшения применены!" << std::endl;
}

template<typename TInputHandler>
void GameController<TInputHandler>::upgradeMeleeDamage() {
    auto& playerRef = game->getPlayerRef();
    
    int currentDamage = playerRef.getMeleeDamage();
    int newDamage = currentDamage + MELEE_DAMAGE_PER_UPDATE;
    playerRef.setMeleeDamage(newDamage);
    
    if (playerRef.getAttackType() == MELEE) {
        playerRef.setDamage(playerRef.getMeleeDamage());
    }
    
    std::cout << "Ближний урон улучшен: " << currentDamage << " -> " << newDamage << std::endl;
}

template<typename TInputHandler>
void GameController<TInputHandler>::upgradeRangedDamage() {
    auto& playerRef = game->getPlayerRef();
    
    int currentDamage = playerRef.getRangedDamage();
    int newDamage = currentDamage + RANGED_DAMAGE_PER_UPDATE;
    playerRef.setRangedDamage(newDamage);
    
    if (playerRef.getAttackType() == RANGED) {
        playerRef.setDamage(playerRef.getRangedDamage());
    }
    
    std::cout << "Дальний урон улучшен: " << currentDamage << " -> " << newDamage << std::endl;
}

template<typename TInputHandler>
void GameController<TInputHandler>::upgradeAttackRange() {
    auto& playerRef = game->getPlayerRef();
    
    int currentRange = playerRef.getAttackRange();
    int newRange = currentRange + ATTACK_RANGE_PER_UPDATE;
    playerRef.setAttackRange(newRange);
    
    std::cout << "Дальность атаки улучшена: " << currentRange << " -> " << newRange << std::endl;
}


template class GameController<Input>;