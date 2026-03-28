#pragma once

#include <cctype>
#include <iostream>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>

#include "Colors.h"
#include "Field.h"
#include "InputManager.h"
#include "LevelManager.h"
#include "exceptions/GameException.h"

#include "systems/AISystem.h"
#include "systems/CollisionSystem.h"
#include "systems/AttackSystem.h"
#include "systems/TargetingSystem.h"
#include "systems/SaveSystem.h"

#include "rendering/RenderingSystem.h"
#include "rendering/ConsoleRenderer.h"

#include "input/ConsoleInputReader.h"
#include "controller/GameController.h"

#include "objects/Player.h"
#include "objects/Enemy.h"
#include "objects/SlowCell.h"
#include "objects/StaticObject.h"

#include "spells/SpellFactory.h"

#include "commands/MoveCommand.h"

enum class GameState {
    MAIN_MENU,
    PLAYING,
    LEVEL_COMPLETE,
    UPGRADE_MENU,
    SPELL_CHOICE,
    GAME_OVER,
    GAME_WIN,
    PAUSED,
    QUIT
};

class Game {
private:
    bool running;
    GameState state;

    // world
    std::unique_ptr<Field> field;
    std::unique_ptr<Player> player;
    LevelManager levelManager;

    // terminal
    struct termios oldSettings;

    // systems
    InputManager inputManager;
    CollisionSystem collisionSystem;
    AISystem aiSystem;
    TargettingSystem targettingSystem;
    std::unique_ptr<AttackSystem> attackSystem;
    std::unique_ptr<RenderingSystem<ConsoleRenderer>> renderSystem;
    SaveSystem saveSystem;

    std::unique_ptr<GameController<ConsoleInputReader>> gameController;

    // spells
    std::vector<std::unique_ptr<ISpell>> spellChoices;
    bool isChoosingSpell = false;
    bool isUpgrading = false;

    int step = 0;

public:
    Game() : running(true), state(GameState::MAIN_MENU) {
        std::srand(std::time(nullptr));
        initTerminal();
        
        renderSystem = std::make_unique<RenderingSystem<ConsoleRenderer>>(
            nullptr, nullptr, targettingSystem);
        
        gameController = std::make_unique<GameController<ConsoleInputReader>>(this);
        gameController->loadConfiguration("controls_alt.cfg");
    }

    ~Game() {
        restoreTerminal();
    }

    void run() {
        try {
            while (running && state != GameState::QUIT) {
                switch (state) {
                    case GameState::MAIN_MENU:
                        showMainMenu();
                        break;
                    case GameState::PLAYING:
                        gameLoop();
                        break;
                    case GameState::LEVEL_COMPLETE:
                        handleLevelComplete();
                        break;
                    case GameState::UPGRADE_MENU:
                        showUpgradeMenu();
                        break;
                    case GameState::SPELL_CHOICE:
                        showSpellChoice();
                        break;
                    case GameState::GAME_OVER:
                        handleGameOver();
                        break;
                    case GameState::GAME_WIN:
                        handleGameWin();
                        break;
                    default:
                        running = false;
                        break;
                }
            }
        } catch (const GameException& e) {
            showError(e.getFullMessage());
            handleGameOver();
        } catch (const std::exception& e) {
            showError(std::string("Unexpected error: ") + e.what());
            handleGameOver();
        }
    }

private:
    void initTerminal();
    void restoreTerminal();

    void initNewGame() {
        levelManager.setLevel(1);
        initLevel();
        state = GameState::PLAYING;
    }

    void initLevel() {
        auto [width, height] = levelManager.getFieldSize();
        
        field = std::make_unique<Field>(width, height);
        
        if (!player) {
            player = std::make_unique<Player>(1, 1);
        } else {
            player->setX(1);
            player->setY(1);
            player->restoreHealth();
        }
        
        field->setPlayer(player.get());
        
        if (!attackSystem) {
            attackSystem = std::make_unique<AttackSystem>(field.get(), player.get());
        } else {
            attackSystem->updateReferences(field.get(), player.get());
        }
        
        if (!renderSystem) {
            renderSystem = std::make_unique<RenderingSystem<ConsoleRenderer>>(
                field.get(), player.get(), targettingSystem);
        } else {
            renderSystem->updateReferences(field.get(), player.get());
        }
        
        levelManager.initializeLevel(*field);
    }


    void gameLoop();
    void displayGameHint();
    bool isUpdateField() { return !targettingSystem.getIsActive(); }

public:
    void handleMove(Direction direction);
    void handleAttack();
    void handleChangeAttackMode();
    void handleQuit();
    void handleConfirm();
    void handleCancel();
    void handleCastSpell();
    void handleSaveGame();
    void handleLoadGame();
    
    Field* getField() { return field.get(); }
    Player* getPlayer() { return player.get(); }
    TargettingSystem& getTargettingSystem() { return targettingSystem; }
    AttackSystem* getAttackSystem() { return attackSystem.get(); }
    CollisionSystem& getCollisionSystem() { return collisionSystem; }
    InputManager& getInputManager() { return inputManager; }
    RenderingSystem<ConsoleRenderer>* getRenderSystem() { return renderSystem.get(); }
    GameController<ConsoleInputReader>* getGameController() { return gameController.get(); }

private:

    void updateGameWorld();
    void updateTowers();

    // UI screens
    void  showMainMenu();
    void showMainMenuScreen();
    void handleMainMenuKb();

    void showUpgradeMenu();
    void showUpgradeMenuScreen();
    bool handleUpgradeMenuKb();

    void handleGameOver();
    void showGameOverScreen();
    void handleGameOverKb();
    
    void handleGameWin();
    void showGameWinScreen();

    void handleLevelComplete();
    void showLevelCompleteScreen();

    void showSpellChoice();
    void showSpellChoiceScreen();
    void handleSpellChoiceKb();

    void showError(const std::string& message);
    void showErrorScreen(const std::string message);

    void upgradeRandomSpell() {
        Hand& hand = player->getHand();
        if (hand.isEmpty()) {
            std::cout << "\n\033[31mNo spells to upgrade!\033[0m\n";
            return;
        }
        
        int index = rand() % hand.getSize();
        ISpell* spell = hand.getSpell(index);
        
        if (spell) {
            int newRange = spell->getRange() + 1;
            int newDamage = spell->getDamage() + 10;
            int newArea = spell->getAreaSize();
            
            auto upgradedSpell = SpellFactory::createSpell(
                spell->getType(), newRange, newDamage, newArea
            );
            
            hand.removeSpell(index);
            hand.addSpell(std::move(upgradedSpell));
            
            std::cout << "\n\033[32mSpell upgraded!\033[0m\n";
        }
    }

    void saveGame() {
        try {
            saveSystem.save(field.get(), player.get(), levelManager.getCurrentLevel());
            renderSystem->clearScreen();
            std::cout << TermColors::GREEN << "Game saved successfully!\n" << TermColors::DEFAULT;
            sleep(1);
        } catch (const GameException& e) {
            renderSystem->clearScreen();
            std::cout << TermColors::RED << "Failed to save: " << e.getFullMessage() << TermColors::DEFAULT << "\n";
            sleep(2);
        }
    }

    void loadGame() {
        try {
            auto saveInfo = saveSystem.getSaveInfo();
            
            field = std::make_unique<Field>(saveInfo.fieldWidth, saveInfo.fieldHeight);
            
            if (!player) {
                player = std::make_unique<Player>(1, 1);
            }
            
            int loadedLevel = saveInfo.level;
            saveSystem.load(field.get(), player.get(), loadedLevel);
            levelManager.setLevel(loadedLevel);
            
            field->setPlayer(player.get());
            
            if (!attackSystem) {
                attackSystem = std::make_unique<AttackSystem>(field.get(), player.get());
            } else {
                attackSystem->updateReferences(field.get(), player.get());
            }
            
            if (!renderSystem) {
                renderSystem = std::make_unique<RenderingSystem<ConsoleRenderer>>(
                    field.get(), player.get(), targettingSystem);
            } else {
                renderSystem->updateReferences(field.get(), player.get());
            }
            
            renderSystem->clearScreen();
            std::cout << TermColors::GREEN << "Game loaded successfully!" << TermColors::DEFAULT << '\n';
            sleep(1);
            
            state = GameState::PLAYING;
            
        } catch (const FileNotFoundException& e) {
            renderSystem->clearScreen();
            std::cout << "\033[31mSave file not found!\033[0m\n";
            sleep(2);
            state = GameState::MAIN_MENU;
        } catch (const GameException& e) {
            renderSystem->clearScreen();
            std::cout << "\033[31mFailed to load: " << e.getFullMessage() << "\033[0m\n";
            sleep(2);
            state = GameState::MAIN_MENU;
        }
    }

};
