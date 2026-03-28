#include "GameSession.h"
#include "DirectDamageSpell.h"
#include "AoESpell.h"
#include "TrapSpell.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <dirent.h>
#include <random>
#include <cctype>

GameSession::GameSession() 
    : gameVisualizer(std::make_unique<Visualizer>())
    , sessionRunning(true) {}

void GameSession::run() {
    bool inMainMenu = true;
    
    while (sessionRunning && inMainMenu) {
        gameVisualizer->renderMainMenu();
        
        ConsoleInputHandler menuInputHandler;
        menuInputHandler.setMode(true);
        
        auto command = menuInputHandler.getCommand();
        
        if (command && command->isNextLevel()) {
            if (startNewGame()) {
                inMainMenu = false;  
            }
        }
        else if (command && command->isLoadGame()) {
            const auto& cmd = command->template get<LoadGameCommand>();
            if (!cmd.filename.empty() && loadGame(cmd.filename)) {
                inMainMenu = false; 
            }
        }
        else if (command && command->isQuit()) {
            std::cout << "Exiting game.\n";
            sessionRunning = false;
            return;
        }
        else {
            std::cout << "Invalid input. Please try again.\n";
        }
    }

    while (sessionRunning && gameController && gameController->isGameRunning()) {
        if (gameController->isSaveRequested()) {
            saveGame(gameController->getSaveFilename());
        }
        
        if (gameController->isLoadRequested()) {
            std::string filename = gameController->getLoadFilename();
            if (!filename.empty()) {
                auto oldController = std::move(gameController);
                if (loadGame(filename)) {
                    continue;  
                } else {
                    gameController = std::move(oldController);
                    std::cout << "Failed to load game. Continuing current game.\n";
                }
            }
        }
        
        if (gameController->isRestartRequested()) {
            gameController->clearRestartRequest();
            startLevel(currentLevel);
            continue;
        }
        
        if (gameController->isNextLevelRequested()) {
            gameController->clearNextLevelRequest();
            nextLevel();
            continue;
        }
        
        gameController->processInput();
        
        auto gameManager = gameController->getGameManager();
        if (!gameManager) {
            std::cout << "Game manager is null. Exiting.\n";
            sessionRunning = false;
            break;
        }

        gameManager->processTowerAttacks();
        gameManager->moveEnemies();
        gameManager->updateTowers();
        
        gameVisualizer->renderGame(*gameManager);

        if (gameManager->isGameWon()) {
            std::cout << "\n🎉 Level " << currentLevel << " Complete! 🎉\n";
            nextLevel();
        } else if (gameManager->isGameOver()) {
            gameOverMenu();
        }
    }
    
    if (sessionRunning) {
        gameController.reset();
        run();  
    } else {
        std::cout << "Thanks for playing!\n";
    }
}

bool GameSession::startNewGame() {
    currentLevel = 1;
    return startLevel(1);
}

bool GameSession::startLevel(int level) {
    try {
        int width = 15 + level * 2;
        int height = 15 + level * 2;
        int walls = 20 + level * 3;

        auto gameManager = std::make_unique<GameManager>(width, height, walls);
        
        auto player = std::make_unique<Player>(100, 30, 10, 0, 0, 3);
        gameManager->placePlayer(std::move(player));

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> disX(3, width - 1);
        std::uniform_int_distribution<> disY(3, height - 1);

        for (int i = 0; i < 4; ++i) {
            int hp = 30 + level * 15;
            int dmg = 10 + level * 5;
            int x = disX(gen);
            int y = disY(gen);
            gameManager->addEnemy(std::make_unique<Enemy>(hp, dmg, x, y));
        }

        if (level >= 2) {
            gameManager->addTower(std::make_unique<EnemyTower>(50 + level*20, 15 + level*5, width-2, height-2));
        }

        gameController = std::make_unique<Controller>(std::move(gameManager));
        gameController->setMenuMode(false);
        
        std::cout << "Level " << level << " started!\n";
        gameVisualizer->renderGame(*gameController->getGameManager());
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to start level: " << e.what() << std::endl;
        return false;
    }
}

void GameSession::nextLevel() {
    auto gameManager = gameController->getGameManager();
    std::unique_ptr<Player> currentPlayer;
    
    if (gameManager) {
        currentPlayer = gameManager->getPlayerPtr();
    }
    
    currentLevel++;
    
    int width = 15 + currentLevel * 2;
    int height = 15 + currentLevel * 2;
    int walls = 20 + currentLevel * 3;
    
    auto newGameManager = std::make_unique<GameManager>(width, height, walls);
    
    if (currentPlayer) {
        SpellHand& hand = currentPlayer->getSpellHand();
        size_t removeCount = hand.size() / 2;
        std::random_device rd;
        std::mt19937 gen(rd());
        
        for (size_t i = 0; i < removeCount && hand.size() > 0; ++i) {
            std::uniform_int_distribution<> dis(0, static_cast<int>(hand.size()) - 1);
            int idx = dis(gen);
            hand.useSpell(idx);
        }
        currentPlayer->restoreHealth();
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(3, width - 1);
    std::uniform_int_distribution<> disY(3, height - 1);

    for (int i = 0; i < 4; ++i) {
        int hp = 30 + currentLevel * 15;
        int dmg = 10 + currentLevel * 5;
        int x = disX(gen);
        int y = disY(gen);
        newGameManager->addEnemy(std::make_unique<Enemy>(hp, dmg, x, y));
    }

    if (currentLevel >= 2) {
        newGameManager->addTower(std::make_unique<EnemyTower>(50 + currentLevel*20, 15 + currentLevel*5, width-2, height-2));
    }

    if (currentPlayer) {
        currentPlayer->setPosition(0, 0);
        newGameManager->placePlayer(std::move(currentPlayer));
    }

    gameController = std::make_unique<Controller>(std::move(newGameManager));
    gameController->setMenuMode(false);
    
    showUpgradeMenu();
    gameVisualizer->renderGame(*gameController->getGameManager());
}

void GameSession::showUpgradeMenu() {
    if (!gameController || !gameController->getGameManager()) return;
    
    Player* p = gameController->getGameManager()->getPlayer();
    if (!p) return;

    gameVisualizer->renderUpgradeMenu(*p);
    
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == '2') {
        p->increaseMeleeDamage(10);
        std::cout << "Melee damage upgraded to " << p->getMeleeDamage() << "!\n";
    }
    else if (choice == '3') {
        p->increaseRangedDamage(5);
        std::cout << "Ranged damage upgraded to " << p->getRangedDamage() << "!\n";
    }
    else if (choice == '1') {
        p->takeDamage(-20);
        std::cout << "HP upgraded to " << p->getHealth() << "!\n";
    }
    else if (choice == '4') {
        SpellHand& hand = p->getSpellHand();
        if (hand.size() < 3) {
            hand.addSpell(std::make_unique<DirectDamageSpell>(20));
            std::cout << "Spell slot added! (" << hand.size() << "/3)\n";
        } else {
            std::cout << "Max slots reached!\n";
        }
    }
    else {
        std::cout << "No upgrade applied.\n";
    }
}

void GameSession::saveGame(const std::string& filename) {
    if (!gameController || !gameController->getGameManager()) {
        std::cout << "No game to save.\n";
        return;
    }

    try {
        std::ofstream file(filename);
        if (!file) throw std::runtime_error("Cannot create file");

        auto gameManager = gameController->getGameManager();
        
        file << "GAME_SAVE\n";
        file << "8\n";
        file << currentLevel << "\n";

        for (int i = 0; i < gameManager->getGridRef().getHeight(); ++i) {
            for (int j = 0; j < gameManager->getGridRef().getWidth(); ++j) {
                if (gameManager->getGridRef().getCell(j, i).getType() == CellType::Impassable) {
                    file << j << " " << i << "\n";
                }
            }
        }
        file << "-1 -1\n";

        Player* p = gameManager->getPlayer();
        if (p) {
            file << p->getPosition().first << " " << p->getPosition().second << " "
                 << p->getHealth() << " " << p->getScore() << " "
                 << (p->getCombatMode() == CombatMode::Melee ? "0" : "1") << " "
                 << p->getMeleeDamage() << " " << p->getRangedDamage() << "\n";

            file << p->getSpellHand().size() << "\n";
            for (size_t i = 0; i < p->getSpellHand().size(); ++i) {
                file << p->getSpellHand().getSpellName(i) << "\n";
            }
        } else {
            file << "0 0 0 0 0 0 0\n0\n";
        }

        file << gameManager->getEnemyCount() << "\n";
        for (const auto& e : gameManager->getEnemies()) {
            if (e) {
                file << e->getPosition().first << " " << e->getPosition().second << " " 
                     << e->getHealth() << " " << e->getDamage() << "\n";
            }
        }

        file << gameManager->getTowers().size() << "\n";
        for (const auto& t : gameManager->getTowers()) {
            if (t) {
                file << t->getPosition().first << " " << t->getPosition().second << " " 
                     << t->getHealth() << " " << t->getDamage() << "\n";
            }
        }

        std::cout << "Game saved to " << filename << "!\n";
    } catch (const std::exception& e) {
        std::cerr << "Save failed: " << e.what() << std::endl;
    }
}

bool GameSession::loadGame(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file) {
            std::cout << "Cannot open file: " << filename << "\n";
            return false;
        }

        std::string line;
        std::getline(file, line);
        if (line != "GAME_SAVE") {
            std::cout << "Invalid save format!\n";
            return false;
        }

        int version;
        file >> version;
        file >> currentLevel;

        int width = 15 + currentLevel * 2;
        int height = 15 + currentLevel * 2;
        auto gameManager = std::make_unique<GameManager>(width, height, 0);

        int x, y;
        while (file >> x >> y) {
            if (x == -1 && y == -1) break;
            if (x >= 0 && x < width && y >= 0 && y < height) {
                gameManager->getGridRef().updateCell(x, y, CellType::Impassable);
            }
        }

        int hp, score, modeInt, meleeDmg, rangedDmg;
        file >> x >> y >> hp >> score >> modeInt >> meleeDmg >> rangedDmg;
        CombatMode mode = (modeInt == 0) ? CombatMode::Melee : CombatMode::Ranged;

        auto player = std::make_unique<Player>(hp, meleeDmg, rangedDmg, x, y, 3);
        player->setCurrentCombatMode(mode);
        player->setScore(score);

        int spellCount;
        file >> spellCount;
        std::getline(file, line);  

        while (player->getSpellHand().size() > 0) {
            player->getSpellHand().useSpell(0);
        }

        for (int i = 0; i < spellCount; ++i) {
            std::getline(file, line);
            if (line == "DirectDamage") player->getSpellHand().addSpell(std::make_unique<DirectDamageSpell>(20));
            else if (line == "AreaOfEffect") player->getSpellHand().addSpell(std::make_unique<AoESpell>(15));
            else if (line == "Trap") player->getSpellHand().addSpell(std::make_unique<TrapSpell>(30));
        }

        size_t eCount;
        file >> eCount;
        for (size_t i = 0; i < eCount; ++i) {
            int ex, ey, ehp, edmg;
            file >> ex >> ey >> ehp >> edmg;
            gameManager->addEnemy(std::make_unique<Enemy>(ehp, edmg, ex, ey));
        }

        size_t tCount;
        file >> tCount;
        for (size_t i = 0; i < tCount; ++i) {
            int tx, ty, thp, tdmg;
            file >> tx >> ty >> thp >> tdmg;
            gameManager->addTower(std::make_unique<EnemyTower>(thp, tdmg, tx, ty));
        }

        gameManager->placePlayer(std::move(player));
        gameController = std::make_unique<Controller>(std::move(gameManager));
        gameController->setMenuMode(false);
        
        std::cout << "Game loaded successfully!\n";
        gameVisualizer->renderGame(*gameController->getGameManager());
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Load error: " << e.what() << std::endl;
        return false;
    }
}

void GameSession::gameOverMenu() {
    bool validChoice = false;
    
    while (!validChoice && sessionRunning) {
        gameVisualizer->renderGameOver();
        
        char choice;
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (choice == '1') {
            startLevel(currentLevel);
            validChoice = true;
        } else if (choice == '2') {
            std::cout << "Returning to main menu...\n";
            gameController->stopGame();
            validChoice = true;
        } else {
            std::cout << "Invalid choice! Please enter 1 or 2.\n";
        }
    }
}