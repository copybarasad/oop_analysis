#include "Game.hpp"
#include <iostream>
#include <fstream>






void printWelcomeAscii() {
    std::cout << "\033[1;35m"; 
    std::cout << R"(
  /$$$$$$   /$$$$$$  /$$$$$$/$$$$  /$$$$$$$$
 /$$__  $$ /$$__  $$| $$_  $$_  $$| $$_____/
| $$  \__/| $$  \ $$| $$ \ $$ \ $$| $$      
| $$ /$$$$| $$$$$$$$| $$ | $$ | $$| $$$$$   
| $$|_  $$| $$__  $$| $$ | $$ | $$| $$__/   
| $$  \ $$| $$  | $$| $$ | $$ | $$| $$      
|  $$$$$$/| $$  | $$| $$ | $$ | $$| $$$$$$$$
 \______/ |/  |/|/ |/ |__/|________/
)" << "\033[0m" << "\n";
}


void Game::run() {
    printWelcomeAscii();
    bool running = true;
    while (running) {
        std::cout << "\n=== MAIN MENU ===\n";
        std::cout << "1. New Game\n2. Load Game\n3. Exit\n> ";
        int choice = getInput();
        
        try {
	        switch(choice){
		        case 1:
		            startNewGame();
		            runLevel();
		            break;
	    
		        case 2:
		            loadGame();
			    runLevel();
		            break;
                
                case 3:
                    std::cout << "Bye!\n";
                    running = false;
                    break;
                default:
                    std::cout << "Unknown option\n!";
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}

void Game::runLevel()
{
    while (currentLevel_ <= 2) {
        std::cout << "\n--- Starting Level " << currentLevel_ << " ---\n";  
        if(!world_){
            createNewLevel(currentLevel_);
        }    
        bool won = loopLevel();

        if (won) {
            if (currentLevel_ == 2) {
            std::cout << "\n###################################\n";
            std::cout << "# CONGRATULATIONS! YOU WON THE GAME! #\n";
            std::cout << "###################################\n";
            } else {
                std::cout << "Level " << currentLevel_ << " Complete!\n";
                upgradePlayer();      
                auto oldSleeve = world_->getPlayer().moveSleeve();
                carriedSleeve_ = std::make_unique<Sleeve>(std::move(oldSleeve));        
                    currentLevel_++;
            }
        }else{
            return;
        }
    }
}



int Game::getInput()
{
    int inp;
    while(!(std::cin >> inp)){
	std::cin.clear();
	std::cin.ignore(10000,'\n');
	std::cout << "invalid input!\n";
    }
    return inp;
}


void Game::startNewGame(){
    currentLevel_ =1;
    playerMaxHp_ = 100;
    savedDamage_ = 10;
    carriedSleeve_ = nullptr;
}

void Game::createNewLevel(int lvl) {
    int size;
    savedTurnCount_ = 1;
    
    if (lvl == 1) {
        size = 10;
    } else {
        size = 10 + (lvl * 5); 
	if (size > 25) size = 25;
    }
    
    Grid grid(size, size);
    
    Player player(playerMaxHp_, savedDamage_, {0,0}, ctx_.logger_);
    
    if (carriedSleeve_) {
        player.setSleeve(std::move(*carriedSleeve_));
        carriedSleeve_ = nullptr; 
        ctx_.logger_.info("Spell cards carried over from previous level.");
    }
    
    MainEnemy boss(100 + (lvl * 50), 5 + (lvl * 5), {size-1, size-1}, ctx_.logger_);

    world_ = std::make_unique<GameWorld>(std::move(grid), std::move(player), std::move(boss));
    
    
    ctx_.displayManager.showMap(*world_);
}

bool Game::loopLevel() {
    TurnManager turnManager(*world_, ctx_,savedTurnCount_);
    
    bool levelRunning = true;
    while(levelRunning) {
        TurnStatus status = turnManager.runOneTurn();
	savedTurnCount_ = turnManager.getTurnCount();

        if (status == TurnStatus::SaveAndExit) {
            saveGame();
            return false; 
        }
        else if (status == TurnStatus::PlayerWon) {
            return true; 
        }
        else if (status == TurnStatus::PlayerDied) {
            std::cout << "Game Over. You died.\n";
            return false; 
        }
    }
    return false;
}

void Game::upgradePlayer() {
    std::cout << "\n*** LEVEL UP! Choose Upgrade: ***\n";
    std::cout << "1. Max HP (+20)\n2. Damage (+5)\n> ";
    int ch;
    while(true) {
        std::cin >> ch;
        if(std::cin.fail()) {
            std::cin.clear(); std::cin.ignore(10000,'\n');
            continue;
        }
        if (ch == 1) { playerMaxHp_ += 20; break; }
        if (ch == 2) { savedDamage_ += 5; break; }
    }
}

void Game::saveGame() {
    std::ofstream file("savegame.dat");
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open save file for writing!");
    }

    file << currentLevel_ << " " << playerMaxHp_ << " " << savedDamage_ <<" " << savedTurnCount_ << "\n";
    
    world_->getGrid().save(file);
    world_->getPlayer().save(file);
    world_->getMainEnemy().save(file);
    
    const auto& enemies = world_->getEnemies();
    file << enemies.size() << "\n";
    for (const auto& enemy : enemies) {
        enemy->save(file);
    }
    
    if (auto* tower = world_->getConstruction()) {
        file << "1\n"; 
        tower->save(file);
    } else {
        file << "0\n";
    }
    
    if (file.fail()) {
        throw std::runtime_error("Error occurred while writing save data.");
    }
    std::cout << "[Game] Game saved successfully!\n";
}

void Game::loadGame() {
    std::ifstream file("savegame.dat");
    if (!file.is_open()) {
        throw std::runtime_error("No save file found!");
    }

    file >> currentLevel_ >> playerMaxHp_ >> savedDamage_ >> savedTurnCount_;
    
    Grid grid(10, 10); 
    grid.load(file);
    
    Player player(0, 0, {0,0}, ctx_.logger_); 
    player.load(file);
    
    MainEnemy boss(0, 0, {0,0}, ctx_.logger_);
    boss.load(file);
    
    world_ = std::make_unique<GameWorld>(std::move(grid), std::move(player), std::move(boss));
    
    size_t enemyCount;
    file >> enemyCount;
    for (size_t i = 0; i < enemyCount; ++i) {
        auto enemy = std::make_unique<Enemy>(0, 0, Position{0,0}, ctx_.logger_);
        enemy->load(file);
        world_->addEnemy(std::move(enemy));
    }
    
    int hasTower;
    file >> hasTower;
    if (hasTower == 1) {
        auto tower = std::make_unique<EnemyConstruction>(0, Position{0,0}, world_->getGrid(), ctx_.logger_);
        tower->load(file);
        world_->setConstruction(std::move(tower));
    } else {
        world_->setConstruction(nullptr); 
    }

    if (file.fail()) {
        throw std::runtime_error("Save file is corrupted.");
    }
    std::cout << "[Game] Game loaded successfully!\n";
}
