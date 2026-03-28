#include "Game.hpp"
#include "exceptions.hpp"
#include "SpellFactory.hpp"
#include "defines.hpp"
#include <iostream>
#include <fstream>
#include <limits>


const std::string SAVE_FILE = "savegame.txt";

void clearInput() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Game::Game() 
    : board(nullptr), current_level(1), is_running(true), 
      enemy_hp_multiplier(1.0), board_width(BOARD_WIDTH), board_height(BOARD_HEIGHT) {

    initLevel(true);
}

Game::~Game() {
    cleanUp();
}

void Game::cleanUp() {
    if (board) {
        delete board;
        board = nullptr;
    }
}

void Game::initLevel(bool new_game) {
    Player* old_player = nullptr;
    
    if (!new_game && board) {
        old_player = board->getPlayer();
    }

    int score = 0;
    std::vector<int> spells_ids;
    int current_damage_bonus = 0;

    if (!new_game && old_player) {
        score = old_player->getScore();
        current_damage_bonus = old_player->getDamage() - (old_player->getAttackType() == Player::AttackType::MELEE ? MELEE_DAMAGE : RANGED_DAMAGE);
        
        size_t keep_count = (old_player->getHand()->getSpellCount() + 1) / 2; 
        for(size_t i=0; i < keep_count; ++i) {
            Spell* s = old_player->getHand()->getSpell(i);
            if(s) spells_ids.push_back(s->getTypeId());
        }
    }

    cleanUp(); 

    board = new GameBoard(board_width, board_height);
    board->initializeBoard(WALL_CHANCE, SLOW_CHANCE);

    Player* p = new Player(1, 1);
    board->addPlayer(p);

    if (new_game) {
        p->getHand()->addSpell(SpellFactory::createRandomSpell());
        p->getHand()->addSpell(SpellFactory::createSpell(SPELL_ID_DIRECT_DAMAGE)); 
    } else {
        p->addScore(score);
        p->upgradeDamage(current_damage_bonus);
        for(int id : spells_ids) {
            p->getHand()->addSpell(SpellFactory::createSpell(id));
        }
    }

    int enemy_count = 2 + current_level;
    int attempts = 0;
    int spawned_count = 0;

    while (spawned_count < enemy_count && attempts < 1000) {
        attempts++;
        int ex = rand() % (board_width - 2) + 1; 
        int ey = rand() % (board_height - 2) + 1;

        if (board->getEntityAt(ex, ey) == nullptr && board->isGround(ex, ey)) {
            if (abs(ex - 1) + abs(ey - 1) > 3) {
                int hp = static_cast<int>(ENEMY_DEFAULT_HEALTH * enemy_hp_multiplier);
                board->addEnemy(new Enemy(ex, ey, hp));
                spawned_count++;
            }
        }
    }
}

void Game::attachLogger(IObserver* observer) {
    if (board && observer) {
        board->addObserver(observer);
    }
}

void Game::movePlayer(int dx, int dy) {
    if (board->movePlayer(dx, dy)) {
        updateWorld();
    }
}

void Game::playerAttack() {
    if (board->playerAttack()) {
        updateWorld();
    }
}

void Game::switchPlayerWeapon() {
    if (board && board->getPlayer()) {
        board->getPlayer()->switchAttackType();
        board->addLogMessage("Weapon switched. Turn spent.");
        updateWorld();
    }
}

void Game::castSpell(int spell_index, int targetX, int targetY) {
    if (board->playerCastSpell(spell_index, targetX, targetY)) {
        updateWorld();
    }
}

void Game::stop() {
    is_running = false;
}


void Game::updateWorld() {
    board->moveAllies();
    board->moveEnemies();
    board->updateBuildings();
    board->updateTowers();
    
    Player* player = board->getPlayer();
    if (player && player->getKillCount() >= KILLS_FOR_NEW_SPELL) {
        player->resetKillCount();
        if (player->getHand()->addSpell(SpellFactory::createRandomSpell())) {
            board->addLogMessage("New spell acquired!");
        }
    }

    if (board->getEnemies().empty() && board->getBuildings().empty() && board->getTowers().empty()) {
        nextLevel();
    }
}

void Game::nextLevel() {
    std::cout << "\n=== LEVEL " << current_level << " COMPLETE! ===\n";
    
    showUpgradeMenu();

    current_level++;
    enemy_hp_multiplier += 0.3;
    board_width += 2;
    board_height += 1;
    if(board_width > 30) board_width = 30;
    if(board_height > 25) board_height = 25;

    initLevel(false); 
}

void Game::showUpgradeMenu() {
    bool choiceF = false;

    while (!choiceF) {
        std::cout << "\n==================================\n";
        std::cout << "    LEVEL COMPLETE! CHOOSE REWARD   \n";
        std::cout << "==================================\n";
        std::cout << "1. Strength Training (Increase Damage +5)\n";
        std::cout << "2. Treasure Hunter (Get 100 Score)\n";
        std::cout << "Enter choice (1 or 2): ";
        
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear(); 
            clearInput(); 
            std::cout << "Invalid input! Please enter a number.\n";
        } else {
            Player* p = board->getPlayer();
            
            if (choice == 1) {
                p->upgradeDamage(5);
                std::cout << ">>> SUCCESS: Melee damage increased! <<<\n";
                choiceF = true; 
            } else if (choice == 2) {
                p->addScore(100); 
                std::cout << ">>> SUCCESS: Bonus score added! <<<\n";
                choiceF = true; 
            } else {
                std::cout << "Please enter strictly 1 or 2.\n";
            }
        }
    }
    clearInput();
}

void Game::saveGame(const std::string& filename) {
    std::ofstream out(filename);
    if (!out.is_open()) throw SaveException("Cannot create save file.");

    out << current_level << "\n";
    out << enemy_hp_multiplier << "\n";
    
    board->save(out);

    if (out.fail()) throw SaveException("Write error.");
    out.close();
    
    board->addLogMessage("=== GAME SAVED SUCCESSFULLY ===");
}

void Game::loadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) throw LoadException("Save file not found.");

    cleanUp();

    try {
        in >> current_level;
        in >> enemy_hp_multiplier;
        
        if (in.fail()) throw LoadException("Header corrupted.");

        int w, h;
        in >> w >> h; 
        
        board_width = w;
        board_height = h;

        board = new GameBoard(w, h);
        board->load(in);

    } catch (...) {
        cleanUp();
        throw; 
    }
    in.close();
    board->addLogMessage("Game loaded!");
}