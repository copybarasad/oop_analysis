#include "game_controller.hpp"
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <cctype>

GameController::GameController() 
    : saveSystem("savegame.dat"),
      selectedSpellIndex(-1), 
      isCastingSpell(false),
      playerTurnCompleted(true) {
    gameState = std::make_unique<GameState>();
}

char GameController::getCharacter() const {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void GameController::initializeNewGame() {
    gameState = std::make_unique<GameState>();
    gameState->initializeNewGame();
    selectedSpellIndex = -1;
    isCastingSpell = false;
    playerTurnCompleted = true;
}

void GameController::processPlayerTurn() {
    if (!gameState || !gameState->isGameActive()) return;
    
    Player* player = gameState->getPlayer();
    if (!player) return;
    
    player->startTurn();
    playerTurnCompleted = false;
    
    bool turnCompleted = false;
    while (!turnCompleted && gameState->isGameActive()) {
        renderer.renderGameState(*gameState);
        std::cout << "\nКоманда (M - меню паузы): ";
        char input = getCharacter();
        
        if (input == 'm' || input == 'M') {
            std::cout << "\nИгра приостановлена...\n";
            return;
        }
        
        if (input == ' ') {
            char direction = getCharacter();
            handleAttack(direction);
            turnCompleted = true;
        }
        else if (isCastingSpell) {
            handleSpellDirection(input);
            if (!isCastingSpell) {
                turnCompleted = true;
            }
        }
        else {
            switch (tolower(input)) {
                case 'w': case 'a': case 's': case 'd':
                    handleMove(input);
                    turnCompleted = true;
                    break;
                    
                case 'q':
                    handleWeaponSwitch();
                    turnCompleted = true;
                    break;
                    
                case 'b':
                    handleSpellPurchase();
                    turnCompleted = true;
                    break;
                    
                case '1': case '2': case '3': case '4': case '5': 
                case '6': case '7': case '8': case '9':
                    handleSpellSelection(input - '0');
                    break;
                    
                case 'e':
                    handleSkipTurn();
                    turnCompleted = true;
                    break;
                    
                case '0':
                    gameState->endGame();
                    return;
                    
                default:
                    std::cout << "Неизвестная команда.\n";
                    usleep(1000000);
                    break;
            }
        }
    }
    
    if (player && turnCompleted) {
        player->endTurn();
        playerTurnCompleted = true;
    }
    
    isCastingSpell = false;
    selectedSpellIndex = -1;
}

void GameController::processEnemyTurn() {
    if (!gameState || !gameState->isGameActive()) return;
    
    std::cout << "\nХод врагов...\n";
    usleep(500000);
    
    gameState->processEnemyTurns();
}

void GameController::nextTurn() {
    if (gameState) {
        gameState->nextTurn();
    }
}

void GameController::saveGame(const std::string& filename) const {
    SaveGame tempSystem(filename);
    if (tempSystem.save(*gameState)) {
        std::cout << "Игра сохранена в файл: " << filename << "\n";
    } else {
        std::cout << "Не удалось сохранить игру.\n";
    }
}

bool GameController::loadGame(const std::string& filename) {
    std::cout << "DEBUG: GameController::loadGame начата\n";
    SaveGame tempSystem(filename);
    if (tempSystem.load(*gameState)) {
        selectedSpellIndex = -1;
        isCastingSpell = false;
        playerTurnCompleted = true;
        std::cout << "DEBUG: GameController::loadGame успешна\n";
        std::cout << "DEBUG: isGameActive = " << isGameActive() << "\n";
        return true;
    }
    std::cout << "DEBUG: GameController::loadGame не удалась\n";
    return false;
}

bool GameController::isGameActive() const {
    return gameState && gameState->isGameActive();
}

bool GameController::isGameOver() const {
    return gameState && gameState->isDefeat();
}

bool GameController::isLevelComplete() const {
    return gameState && gameState->isVictory();
}

bool GameController::isPlayerTurnCompleted() const {
    return playerTurnCompleted;
}

int GameController::getCurrentLevel() const {
    return gameState ? gameState->getCurrentLevel() : 0;
}

int GameController::getCurrentTurn() const {
    return gameState ? gameState->getCurrentTurn() : 0;
}

int GameController::getScore() const {
    return gameState ? gameState->getScore() : 0;
}

void GameController::handleMove(char direction) {
    Player* player = gameState->getPlayer();
    if (!player || !player->canMove()) {
        std::cout << "Нельзя двигаться сейчас\n";
        return;
    }
    
    int targetX = player->getPositionX();
    int targetY = player->getPositionY();
    
    switch (tolower(direction)) {
        case 'w': targetY--; break;
        case 's': targetY++; break;
        case 'a': targetX--; break;
        case 'd': targetX++; break;
        default: 
            std::cout << "Неверное направление\n";
            return;
    }
    
    if (player->tryMove(targetX, targetY, *gameState->getMap())) {
        std::cout << "Переместились в (" << targetX << ", " << targetY << ")\n";
    } else {
        std::cout << "Нельзя переместиться в (" << targetX << ", " << targetY << ")\n";
    }
}

void GameController::handleAttack(char direction) {
    Player* player = gameState->getPlayer();
    if (!player || !player->canAttack()) {
        std::cout << "Нельзя атаковать сейчас\n";
        return;
    }
    
    int dirX = 0, dirY = 0;
    switch (tolower(direction)) {
        case 'w': dirY = -1; break;
        case 's': dirY = 1; break;
        case 'a': dirX = -1; break;
        case 'd': dirX = 1; break;
        default: 
            std::cout << "Неверное направление\n";
            return;
    }
    
    int damage = player->performDirectionalAttack(dirX, dirY, 
                                                 *gameState->getMap(), 
                                                 gameState->getEnemies(),
                                                 gameState->getBuildings());
    if (damage > 0) {
        std::cout << "Попадание! " << damage << " урона\n";
    } else {
        std::cout << "Промах!\n";
    }
}

void GameController::handleSpellSelection(int spellNum) {
    Player* player = gameState->getPlayer();
    if (!player || !player->getSpellHand()) {
        std::cout << "Нет доступа к заклинаниям\n";
        return;
    }
    
    selectedSpellIndex = spellNum - 1;
    if (selectedSpellIndex >= player->getSpellHand()->getSize()) {
        std::cout << "Нет заклинания на позиции " << spellNum << "\n";
        selectedSpellIndex = -1;
        return;
    }
    
    const SpellCard* spell = player->getSpellHand()->getSpell(selectedSpellIndex);
    if (spell && player->canCastSpell()) {
        isCastingSpell = true;
        std::cout << "Выбрано " << spell->getName() 
                  << ". Выберите направление (W/A/S/D) или C для отмены.\n";
    } else {
        std::cout << "Нельзя кастовать заклинание сейчас\n";
        selectedSpellIndex = -1;
    }
}

void GameController::handleSpellDirection(char direction) {
    Player* player = gameState->getPlayer();
    if (!player || !player->canCastSpell() || !isCastingSpell || selectedSpellIndex < 0) {
        std::cout << "Нельзя кастовать заклинание сейчас\n";
        isCastingSpell = false;
        selectedSpellIndex = -1;
        return;
    }
    
    if (tolower(direction) == 'c') {
        std::cout << "Каст заклинания отменен.\n";
        isCastingSpell = false;
        selectedSpellIndex = -1;
        return;
    }
    
    int dirX = 0, dirY = 0;
    
    switch (tolower(direction)) {
        case 'w': dirY = -1; break;
        case 's': dirY = 1; break;
        case 'a': dirX = -1; break;
        case 'd': dirX = 1; break;
        default: 
            std::cout << "Неверное направление\n";
            return;
    }
    
    const SpellCard* spell = player->getSpellHand()->getSpell(selectedSpellIndex);
    if (spell) {
        spell->castAsProjectile(gameState.get(), dirX, dirY);
        player->getSpellHand()->removeSpell(selectedSpellIndex);
        player->resetSpellFlag();
    }
    
    isCastingSpell = false;
    selectedSpellIndex = -1;
}

void GameController::handleWeaponSwitch() {
    Player* player = gameState->getPlayer();
    if (player) {
        player->switchWeapon();
        std::cout << "Переключились на " 
                  << (player->getCurrentWeapon() == WeaponType::SWORD ? "Меч" : "Лук") 
                  << "\n";
    }
}

void GameController::handleSpellPurchase() {
    Player* player = gameState->getPlayer();
    if (player && player->getSpellHand()) {
        player->getSpellHand()->buyRandomSpell();
    }
}

void GameController::handleSkipTurn() {
    std::cout << "Ход пропущен\n";
}