#include "Game.h"
#include "Exceptions.h"
#include <iostream>
#include <sstream>
#include <cmath>

Game::Game(int field_width, int field_height)
    : field_(field_width, field_height), 
      game_running_(true),
      current_state_(GameState::kNormal)
{}

void Game::startGame() {
    initializeGame();
    notifyLog("Game Started.");
}

void Game::notifyLog(const std::string& msg) {
    notifyObservers(msg);
}

void Game::initializeGame() {
    current_level_ = 1;
    entity_manager_.clearAll();
    entity_manager_.createPlayer(1, 1, 100, 15, 10);
    casting_cursor_ = entity_manager_.getPlayer()->getPosition();
    
    field_.resize(20, 15); 
    
    entity_manager_.addEnemy(std::make_unique<Enemy>(15, 10, 50, 5));
    entity_manager_.addEnemy(std::make_unique<Enemy>(5, 8, 50, 5));
    entity_manager_.addBuilding(std::make_unique<EnemyBuilding>(18, 13, 100, 20));
    entity_manager_.addTower(std::make_unique<EnemyTower>(10, 5, 80, 8, 5, 3));

    field_.populateField(15, 10, entity_manager_.getPlayer()->getPosition());
    
    notifyLog("Level 1 Initialized. Player at (1,1).");
}

void Game::checkGameStatus() {
    if (current_state_ == GameState::kNormal) {
        if (isGameOver()) {
            notifyLog("GAME OVER! Player died.");
            notifyLog("Restarting game...");
            initializeGame();
        } else if (hasWon()) {
            notifyLog("Level Complete! All enemies defeated.");
            nextLevel();
        }
    }
}

void Game::nextLevel() {
    current_level_++;
    std::stringstream ss;
    ss << "Entering Level " << current_level_;
    notifyLog(ss.str());
    
    Player* p = entity_manager_.getPlayer();
    if(!p) return;
    
    p->healFull();
    notifyLog("Player health restored.");
    
    int count = p->getHand().getSpellCount();
    if (count > 0) {
        p->getHand().removeRandomSpells(count / 2);
        notifyLog("Half of your spells were lost in transit...");
    }
    
    processLevelUp();

    int new_w = 15 + (current_level_ * 2);
    int new_h = 10 + (current_level_ * 2);
    if (new_w > 25) new_w = 25;
    if (new_h > 25) new_h = 25;
    
    field_.resize(new_w, new_h);
    entity_manager_.clearNonPlayerEntities();
    
    p->setPosition(1, 1);
    field_.populateField(10 + current_level_, 5 + current_level_, p->getPosition());

    int hp_scale = 50 + (current_level_ * 10);
    int dmg_scale = 5 + current_level_;
    
    entity_manager_.addEnemy(std::make_unique<Enemy>(new_w - 2, new_h - 2, hp_scale, dmg_scale));
    entity_manager_.addEnemy(std::make_unique<Enemy>(new_w/2, new_h/2, hp_scale, dmg_scale));
    
    if (current_level_ % 2 == 0) {
         entity_manager_.addTower(std::make_unique<EnemyTower>(new_w/2, 2, hp_scale * 1.5, dmg_scale + 2, 5, 3));
    }
    if (current_level_ % 3 == 0) {
        entity_manager_.addBuilding(std::make_unique<EnemyBuilding>(new_w-3, 3, hp_scale * 2, 15));
    }
    
    notifyLog("New level entities spawned.");
}

void Game::printLevelUpOptions() {
    notifyLog("--- LEVEL UP BONUS ---");
    notifyLog("Choose upgrade by pressing key:");
    notifyLog("1. +10 Max HP & Full Heal");
    notifyLog("2. +5 Melee Damage");
    notifyLog("3. +1 Hand Size");
}

void Game::processLevelUp() {
    current_state_ = GameState::kLevelUp;
    
    printLevelUpOptions();
}

void Game::selectLevelUpBonus(int choice) {
    if (current_state_ != GameState::kLevelUp) return;

    Player* p = entity_manager_.getPlayer();
    if (!p) return;

    bool valid = true;
    switch(choice) {
        case 1: 
            p->increaseMaxHealth(10); 
            notifyLog("Selected: +10 Max HP. Health Restored.");
            break;
        case 2: 
            p->increaseMeleeDamage(5); 
            notifyLog("Selected: +5 Melee Damage.");
            break; 
        case 3: 
            p->getHand().setMaxSize(p->getHand().getMaxSize() + 1); 
            notifyLog("Selected: +1 Hand Size.");
            break;
        default: 
            notifyLog("Invalid choice. Please select 1, 2 or 3.");
            printLevelUpOptions();
            valid = false;
    }

    if (valid) {
        current_state_ = GameState::kNormal;
        notifyLog("Bonus applied. Ready for next level!");
    }
}

void Game::saveGame(const std::string& filename) {
    try {
        save_manager_.save(filename, current_level_, field_, entity_manager_);
        notifyLog("Game saved successfully to " + filename);
    } catch (const std::exception& e) {
        notifyLog("Save failed: " + std::string(e.what()));
    }
}

void Game::loadGame(const std::string& filename) {
    try {
        save_manager_.load(filename, current_level_, field_, entity_manager_);
        notifyLog("Game loaded successfully!");
        if (auto p = entity_manager_.getPlayer()) {
            casting_cursor_ = p->getPosition();
        }
    } catch (const GameException& e) {
        notifyLog("Load failed: " + std::string(e.what()));
        notifyLog("Restarting game due to error...");
        initializeGame(); 
    } catch (const std::exception& e) {
        notifyLog("Critical error during load: " + std::string(e.what()));
        initializeGame();
    }
}

void Game::endPlayerTurn() {
    updateGameState();
    notifyLog("Turn ended.");
}

void Game::updateGameState() {
    entity_manager_.updateAll(*this);
    entity_manager_.removeDeadEntities();
}

bool Game::isGameOver() const {
    const Player* player = entity_manager_.getPlayer();
    return player && !player->isAlive();
}

bool Game::hasWon() const {
    return entity_manager_.getEnemies().empty() && 
           entity_manager_.getBuildings().empty() && 
           entity_manager_.getTowers().empty();
}

void Game::movePlayer(int dx, int dy) {
    if (current_state_ == GameState::kLevelUp) {
        notifyLog("You must choose a level-up bonus first! (Press 1, 2, or 3)");
        return;
    }

    Player* player = entity_manager_.getPlayer();
    if (!player || !player->isAlive()) return;
    
    if (player->isSlowed()) {
        player->setSlowed(false);
        notifyLog("You are slowed and cannot move this turn.");
        return;
    }

    Point current_pos = player->getPosition();
    
    if (player->getStance() == CombatStance::kRanged) {
        int range = player->getRange();
        bool attacked = false;
        
        for (int i = 1; i <= range; ++i) {
            int check_x = current_pos.x + dx * i;
            int check_y = current_pos.y + dy * i;
            
            if (!field_.isValid(check_x, check_y) || field_.getCellType(check_x, check_y) == CellType::kImpassable) {
                break; 
            }

            Entity* target = entity_manager_.getEntityAt({check_x, check_y});
            if (target && (dynamic_cast<Enemy*>(target) || dynamic_cast<EnemyBuilding*>(target) || dynamic_cast<EnemyTower*>(target))) {
                std::stringstream ss;
                ss << "You shoot an enemy for " << player->getDamage() << " damage!";
                notifyLog(ss.str());
                
                if (target->takeDamage(player->getDamage())) {
                    notifyLog("You defeated an enemy!");
                    player->addScore(10);
                }
                attacked = true;
                break; 
            }
        }

        if (attacked) {
            return; 
        }
    }

    int new_x = current_pos.x + dx;
    int new_y = current_pos.y + dy;

    if (!field_.isValid(new_x, new_y) || field_.getCellType(new_x, new_y) == CellType::kImpassable) {
        notifyLog("You can't move there (Impassable or Out of Bounds).");
        return;
    }

    Entity* target = entity_manager_.getEntityAt({new_x, new_y});
    
    if (target && (dynamic_cast<Enemy*>(target) || dynamic_cast<EnemyBuilding*>(target) || dynamic_cast<EnemyTower*>(target))) {
        std::stringstream ss;
        ss << "You attack an enemy (melee) for " << player->getDamage() << " damage!";
        notifyLog(ss.str());
        
        if (target->takeDamage(player->getDamage())) {
            notifyLog("You defeated an enemy!");
            player->addScore(10);
        }
        return;
    }
    
    player->setPosition(new_x, new_y);
    entity_manager_.checkTrapsFor(*this, player);

    if (field_.getCellType(new_x, new_y) == CellType::kSlowing) {
        player->setSlowed(true);
        notifyLog("You stepped on a slowing trap! You will be unable to move next turn.");
    }
}

void Game::enterCastingMode() {
    current_state_ = GameState::kCasting;
    selected_spell_idx_ = -1;
    casting_cursor_ = entity_manager_.getPlayer()->getPosition();
    notifyLog("Entering CASTING mode. Select a spell (1-9) then move cursor.");
}

void Game::exitCastingMode() {
    current_state_ = GameState::kNormal;
    notifyLog("Casting cancelled.");
}

void Game::selectSpell(int index) {
    if (index < entity_manager_.getPlayer()->getHand().getSpellCount()) {
        selected_spell_idx_ = index;
        std::stringstream ss;
        ss << "Selected spell: " << entity_manager_.getPlayer()->getHand().getSpell(selected_spell_idx_)->getName();
        notifyLog(ss.str());
    } else {
        notifyLog("Invalid spell number selection.");
    }
}

void Game::castSelectedSpell() {
    if (selected_spell_idx_ != -1) {
        ISpell* spell = entity_manager_.getPlayer()->getHand().getSpell(selected_spell_idx_);
        if (spell->cast(*this, *entity_manager_.getPlayer(), casting_cursor_)) {
            current_state_ = GameState::kNormal;
            endPlayerTurn();
            notifyLog("Spell cast successfully.");
        } else {
            notifyLog("Spell cast failed (invalid target or conditions).");
        }
    } else {
        notifyLog("No spell selected. Press a number key first.");
    }
}

void Game::buySpell() {
    Player* player = entity_manager_.getPlayer();
    if (!player) return;

    notifyLog("--- SPELL SHOP ---");
    
    int cost = 25; 
    
    if (player->getScore() >= cost) {
        std::unique_ptr<ISpell> spell = std::make_unique<AreaDamageSpell>(15, 2);
        
        if (player->spendScore(cost)) {
            if (player->getHand().addSpell(std::move(spell))) {
                notifyLog("Purchased 'Area Damage' spell for 25 score.");
            } else {
                notifyLog("Your hand is full. Score refunded.");
                player->addScore(cost);
            }
        }
    } else {
        notifyLog("Not enough score to buy a spell (Cost: 25).");
    }
}

void Game::quit() {
    notifyLog("Quitting game.");
    game_running_ = false;
}

bool Game::isCellFree(const Point& pos) const {
    return field_.isValid(pos.x, pos.y) &&
           field_.getCellType(pos.x, pos.y) == CellType::kEmpty &&
           !entity_manager_.getEntityAt(pos) &&
           !entity_manager_.getTrapAt(pos);
}