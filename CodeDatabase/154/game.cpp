#include "game.h"
#include "player.h"
#include "enemy.h"
#include "building.h"
#include "trap_spell.h"
#include "area_spell.h"
#include "direct_damage_spell.h"
#include "tower.h"
#include <iostream>
#include <random>
#include <sstream>

Game::Game(int rows, int cols): field_(rows, cols), nextIdCounter_(1) {
    inputHandler_.loadFromFile("keybindings.cfg");
    int r = field_.rows()/2;
    int c = field_.cols()/2;
    Player* player = new Player(createId(), 50, 5, 3, 0);
    player->hand().addRandomSpell();

    field_.placePlayer(player, r, c);
    if (r-3 >=0 && c+3 < field_.cols()) {
        
        Tower* tower = new Tower(createId(), 20, 6, 4, 1);
        field_.placeBuilding(tower, r-3, c+3);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> rowDist(0, field_.rows()-1);
    std::uniform_int_distribution<int> colDist(0, field_.cols()-1);
    for (int i = 0; i < (field_.rows() * field_.cols())/40 + 3; ++i) {
        int rr = rowDist(gen);
        int cc = colDist(gen);
        if (rr == r && cc == c) continue;
        Enemy* enemy = new Enemy(createId(), 5, 2);
        field_.placeEnemy(enemy, rr, cc);
    }
    for (int i = 0; i < 2; ++i) {
        int rr = rowDist(gen);
        int cc = colDist(gen);
        if (rr == r && cc == c) continue;
        Building* building = new Building(createId(), 10, 15);
        field_.placeBuilding(building, rr, cc);
    }
}

int Game::createId() { return nextIdCounter_++; }

void Game::run() {
    while (!isGameOver()) {
        field_.print();
        
        // Build prompt with actual key bindings
        const auto& bindings = inputHandler_.getKeyBindings();
        char moveUpKey = ' ', moveDownKey = ' ', moveLeftKey = ' ', moveRightKey = ' ';
        char castKey = ' ', saveKey = ' ';
        
        for (const auto& pair : bindings) {
            if (pair.second == CommandType::MoveUp) moveUpKey = pair.first;
            else if (pair.second == CommandType::MoveDown) moveDownKey = pair.first;
            else if (pair.second == CommandType::MoveLeft) moveLeftKey = pair.first;
            else if (pair.second == CommandType::MoveRight) moveRightKey = pair.first;
            else if (pair.second == CommandType::CastSpell) castKey = pair.first;
            else if (pair.second == CommandType::SaveGame) saveKey = pair.first;
        }
        
        std::cout << "Move (" << moveUpKey << "/" << moveLeftKey << "/" << moveDownKey << "/" << moveRightKey 
                  << "), cast spell (" << castKey << "): ";
        
        char ch;
        if (!(std::cin >> ch)) break;
        
        // Get the actual command for the pressed key
        auto it = bindings.find(ch);
        if (it == bindings.end()) {
            continue;
        }
        CommandType cmdType = it->second;
        
        if (cmdType == CommandType::CastSpell) {
            int idx, tr, tc;
            std::cout << "Cast spell index and target row col: ";
            if (!(std::cin >> idx >> tr >> tc)) break;
            Player* p = field_.getPlayerById(field_.getPlayerId());
            bool ok = false;
            if (p) ok = p->hand().playSpell(static_cast<size_t>(idx), p->getId(), field_, tr, tc);
            if (!ok) {
                std::cout << "Spell failed or invalid\n";
            }
            continue;
        }
        
        Direction dir = Direction::None;
        if (cmdType == CommandType::MoveUp) dir = Direction::Up;
        else if (cmdType == CommandType::MoveDown) dir = Direction::Down;
        else if (cmdType == CommandType::MoveLeft) dir = Direction::Left;
        else if (cmdType == CommandType::MoveRight) dir = Direction::Right;
        
        if (dir != Direction::None) {
            field_.movePlayer(dir);
            field_.updateEntities();
        }
    }
    std::cout << "Game over" << std::endl;
}

bool Game::isGameOver() const {
    int pid = field_.getPlayerId();
    if (pid < 0) return true;
    Player* p = field_.getPlayerById(pid);
    if (!p) return true;
    return !p->isAlive();
}
