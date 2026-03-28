#include "Game.h"
#include "GameExceptions.h"
#include <random>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "DirectDamageSpell.h"
#include "AoESpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BoostSpell.h"

Game::Game(int field_width, int field_height)
        : field_(field_width, field_height),
          player_(),
          game_state_(GameState::PlayerTurn),
          last_turn_message_("Game started. Level 1"),
          current_level_(1) {
    initializeLevel();
}

int Game::getCurrentLevel() const { return current_level_; }

void Game::clearEntities() {
    enemies_.clear();
    buildings_.clear();
    towers_.clear();
    allies_.clear();
    traps_.clear();
}

void Game::initializeLevel() {
    clearEntities();

    if (current_level_ > 1) {
        int new_w = std::min(25, 15 + (current_level_ - 1) * 2);
        int new_h = std::min(25, 15 + (current_level_ - 1) * 2);
        field_ = Field(new_w, new_h);
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distX(0, field_.getWidth() - 1);
    std::uniform_int_distribution<int> distY(0, field_.getHeight() - 1);

    int px, py;
    do {
        px = distX(rng);
        py = distY(rng);
    } while (field_.getCellType(px, py) != CellType::kEmpty);
    player_.setPosition(px, py);


    auto findEmptyCell = [&]() {
        int x, y;
        int attempts = 0;
        bool occupied = true;

        while (occupied && attempts < 5000) {
            occupied = false;
            x = distX(rng);
            y = distY(rng);
            attempts++;

            if (field_.getCellType(x, y) != CellType::kEmpty) { occupied = true; continue; }
            if (x == player_.getX() && y == player_.getY()) { occupied = true; continue; }

            for (const auto& e : enemies_) { if (e.getX() == x && e.getY() == y) { occupied = true; break; } }
            if (occupied) continue;

            for (const auto& t : towers_) { if (t.getX() == x && t.getY() == y) { occupied = true; break; } }
            if (occupied) continue;

            for (const auto& b : buildings_) { if (b.getX() == x && b.getY() == y) { occupied = true; break; } }
        }

        if (occupied) return std::make_pair(0, 0);
        return std::make_pair(x, y);
    };

    if (current_level_ == 1 && player_.getHand().getSpells().empty()) {
        player_.getHand().addSpell(std::make_unique<DirectDamageSpell>(25, 5));
        player_.getHand().addSpell(std::make_unique<AoESpell>(15 , 4, 1));
    }
    else if (current_level_ > 1) {
        if (!player_.getHand().isFull()) {
            addRandomSpell();
            setLastMessage("New level reached! You received a bonus spell.");
        }
    }

    int enemy_count = 0;
    int tower_count = 0;
    int building_count = 0;


    int enemy_hp = 20;
    int enemy_dmg = 5;
    int tower_hp = 50;
    int tower_dmg = 0;

    if (current_level_ == 1) {
        enemy_count = 2;
        tower_count = 0;
        building_count = 0;
    }
    else if (current_level_ == 2) {
        enemy_count = 3;
        enemy_hp = 30;
        enemy_dmg = 10;
        tower_count = 1;
        tower_dmg = 5;
        building_count = 0;
    }
    else if (current_level_ >= 3) {
        enemy_count = 3;
        enemy_hp = 30;
        enemy_dmg = 10;
        tower_count = 2;
        tower_dmg = 5;
        building_count = 1;
    }

    auto pos = std::make_pair(0,0);

    for (int i = 0; i < enemy_count; ++i) {
        pos = findEmptyCell();
        enemies_.emplace_back(enemy_hp, enemy_dmg, pos.first, pos.second);
    }

    for (int i = 0; i < tower_count; ++i) {
        pos = findEmptyCell();
        towers_.emplace_back(tower_hp, tower_dmg, 4, pos.first, pos.second, 5);
    }

    for (int i = 0; i < building_count; ++i) {
        pos = findEmptyCell();
        int spawn_rate = 25;
        buildings_.emplace_back(pos.first, pos.second, spawn_rate);
    }

    enemies_killed_since_last_spell_ = 0;

    std::string msg = "Level " + std::to_string(current_level_) + " started! ";
    msg += "E:" + std::to_string(enemy_count) + " T:" + std::to_string(tower_count) + " B:" + std::to_string(building_count);
    setLastMessage(msg);
}

void Game::tryNextLevel() {
    if (enemies_.empty() && towers_.empty() && buildings_.empty()) {
        if (current_level_ >= 3) {
            game_state_ = GameState::Victory;
            setLastMessage("Victory! All levels completed.");
        } else {
            game_state_ = GameState::UpgradeMenu;
            setLastMessage("Level complete! Choose an upgrade.");
        }
    } else {
        setLastMessage("Destroy all enemies and structures first!");
    }
}

void Game::upgradePlayer(int choice) {
    if (game_state_ != GameState::UpgradeMenu) return;

    int spells_count = player_.getHand().getSpells().size();
    int to_remove = spells_count / 2;
    for(int i = 0; i < to_remove; ++i) {
        player_.getHand().removeRandomSpell();
    }

    if (current_level_ == 1) {
        if (choice == 1) {
            player_.setMaxHealth(130);
            setLastMessage("Max HP set to 130! Health restored.");
        } else {
            player_.increaseDamage(10);
            setLastMessage("Damage +10! Health restored.");
        }
    }
    else if (current_level_ == 2) {
        if (choice == 1) {
            player_.setMaxHealth(190);
            setLastMessage("Max HP set to 190! Health restored.");
        } else {
            player_.increaseDamage(5);
            setLastMessage("Damage +5! Health restored.");
        }
    }

    player_.heal();

    if (to_remove > 0) {
        setLastMessage(getLastMessage() + " | " + std::to_string(to_remove) + " spells decayed.");
    }

    current_level_++;
    initializeLevel();
    game_state_ = GameState::PlayerTurn;
}

void Game::giveSpellOnKill() {
    enemies_killed_since_last_spell_++;
    int threshold = (current_level_ == 1) ? 1 : 2;

    if (enemies_killed_since_last_spell_ >= threshold) {
        if (!player_.getHand().isFull()) {
            addRandomSpell();
            setLastMessage(getLastMessage() + " You earned a spell!");
            enemies_killed_since_last_spell_ = 0;
        }
    }
}

void Game::restartGame() {
    current_level_ = 1;
    player_ = Player();
    initializeLevel();
    game_state_ = GameState::PlayerTurn;
}

void Game::saveGame(const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs.is_open()) throw SaveFileException("Cannot open file: " + filename);

    try {
        ofs << "GAME_SAVE_V2\n";
        ofs << current_level_ << "\n";

        ofs << player_.getHealth() << " " << player_.getScore() << " "
            << player_.getX() << " " << player_.getY() << " "
            << player_.getBoostCharges() << "\n";
        player_.getHand().save(ofs);

        ofs << enemies_.size() << "\n";
        for(const auto& e : enemies_) ofs << e.getHealth() << " " << e.getX() << " " << e.getY() << "\n";

        ofs << towers_.size() << "\n";
        for(const auto& t : towers_) ofs << t.getHealth() << " " << t.getX() << " " << t.getY() << "\n";

        ofs << buildings_.size() << "\n";
        for(const auto& b : buildings_) ofs << b.getX() << " " << b.getY() << "\n";

        field_.save(ofs);

        ofs << traps_.size() << "\n";
        for(const auto& t : traps_) ofs << t.getDamage() << " " << t.getX() << " " << t.getY() << "\n";

        ofs << allies_.size() << "\n";
        for(const auto& a : allies_) ofs << a.getHealth() << " " << a.getX() << " " << a.getY() << "\n";

        if (ofs.fail()) throw SaveFileException("Write failed");
    } catch (const std::exception& e) {
        throw SaveFileException(e.what());
    }
    setLastMessage("Game Saved.");
}

void Game::loadGame(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) throw LoadFileException("Cannot open file: " + filename);

    try {
        std::string header;
        ifs >> header;
        if (header != "GAME_SAVE_V2") throw CorruptedDataException("Incompatible save version");

        ifs >> current_level_;

        int hp, score, px, py, boosts;
        if(!(ifs >> hp >> score >> px >> py >> boosts)) throw CorruptedDataException("Read stats error");

        player_ = Player(hp, 20, 10, score, px, py);
        player_.setBoostCharges(boosts);

        if (current_level_ == 2) { player_.setMaxHealth(130); player_.increaseDamage(10); }
        if (current_level_ >= 3) { player_.setMaxHealth(190); player_.increaseDamage(15); }

        player_.getHand().load(ifs);

        clearEntities();

        size_t count;
        ifs >> count;
        for(size_t i=0; i<count; ++i) {
            int ehp, ex, ey;
            ifs >> ehp >> ex >> ey;
            int dmg = (current_level_ == 1) ? 15 : (current_level_ == 2 ? 20 : 25);
            enemies_.emplace_back(ehp, dmg, ex, ey);
        }

        ifs >> count;
        for(size_t i=0; i<count; ++i) {
            int thp, tx, ty;
            ifs >> thp >> tx >> ty;
            int tdmg = (current_level_ <= 1) ? 0 : (current_level_ == 2 ? 10 : 11);
            towers_.emplace_back(thp, tdmg, 4, tx, ty, 5);
        }

        ifs >> count;
        for(size_t i=0; i<count; ++i) {
            int bx, by;
            ifs >> bx >> by;
            buildings_.emplace_back(bx, by, 30);
        }

        field_.load(ifs);

        ifs >> count;
        for(size_t i=0; i<count; ++i) {
            int tdmg, tx, ty;
            ifs >> tdmg >> tx >> ty;
            traps_.emplace_back(tdmg, tx, ty);
        }

        ifs >> count;
        for(size_t i=0; i<count; ++i) {
            int ahp, ax, ay;
            ifs >> ahp >> ax >> ay;
            allies_.emplace_back(ahp, 10, ax, ay);
        }

        if(ifs.fail()) throw CorruptedDataException("Bad format");

    } catch (const std::exception& e) {
        throw LoadFileException(e.what());
    }
    setLastMessage("Game Loaded.");
    game_state_ = GameState::PlayerTurn;
}

const std::vector<std::pair<int, int>>& Game::getLastShotPath() const {
    return last_shot_path_;
}

bool Game::hasNewShotAnimation() const {
    return has_new_shot_;
}

void Game::clearShotAnimation() {
    has_new_shot_ = false;
    last_shot_path_.clear();
}

void Game::processPlayerMove(char direction) {
    setLastMessage("");
    if (player_.isSlowed()) {
        setLastMessage("You are slowed!");
        player_.setSlowed(false);
        endPlayerTurn();
        return;
    }

    int next_x = player_.getX();
    int next_y = player_.getY();
    switch (direction) {
        case 'w': --next_y; break;
        case 's': ++next_y; break;
        case 'a': --next_x; break;
        case 'd': ++next_x; break;
    }

    if (!field_.isValidPosition(next_x, next_y) || field_.getCellType(next_x, next_y) == CellType::kObstacle) {
        setLastMessage("Blocked.");
        return;
    }

    auto enemy_it = std::find_if(enemies_.begin(), enemies_.end(),
                                 [next_x, next_y](const Enemy& e) { return e.getX() == next_x && e.getY() == next_y; });
    if (enemy_it != enemies_.end()) {
        damageEntityAt(next_x, next_y, player_.getDamage());
        setLastMessage("Hit enemy!");
        endPlayerTurn();
        return;
    }

    auto tower_it = std::find_if(towers_.begin(), towers_.end(),
                                 [next_x, next_y](const EnemyTower& t) { return t.getX() == next_x && t.getY() == next_y; });
    if (tower_it != towers_.end()) {
        damageEntityAt(next_x, next_y, player_.getDamage());
        setLastMessage("Hit tower!");
        endPlayerTurn();
        return;
    }

    auto build_it = std::find_if(buildings_.begin(), buildings_.end(),
                                 [next_x, next_y](const EnemyBuilding& b) { return b.getX() == next_x && b.getY() == next_y; });
    if (build_it != buildings_.end()) {
        setLastMessage("Blocked by building.");
        return;
    }

    if (!canPlaceEntityAt(next_x, next_y, true)) {
        setLastMessage("Blocked by entity.");
        return;
    }

    player_.setPosition(next_x, next_y);
    if (field_.getCellType(next_x, next_y) == CellType::kSlowingTrap) {
        player_.setSlowed(true);
        setLastMessage("Stepped on trap! Slowed.");
    }

    endPlayerTurn();
}

void Game::processPlayerAction(char action) {
    if (action == 'c') {
        player_.switchCombatMode();
        if (player_.getModeName() == "Ranged") {
            setLastMessage("Switched mode: Ranged. Press 'F' to Aim!");
        } else {
            setLastMessage("Switched mode: Melee.");
        }
        endPlayerTurn();
    } else if (action == 'f') {
        if (player_.getModeName() == "Ranged") {
            game_state_ = GameState::Aiming;
            setLastMessage("Aiming (WASD to shoot)...");
        } else {
            setLastMessage("Ranged mode only!");
        }
    }
}

void Game::processPlayerAim(char direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case 'w': dy = -1; break; case 's': dy = 1; break;
        case 'a': dx = -1; break; case 'd': dx = 1; break;
    }

    last_shot_path_.clear();
    has_new_shot_ = true;

    int currentX = player_.getX();
    int currentY = player_.getY();

    currentX += dx;
    currentY += dy;

    bool hit = false;
    while (field_.isValidPosition(currentX, currentY)) {
        last_shot_path_.push_back({currentX, currentY});

        if (field_.getCellType(currentX, currentY) == CellType::kObstacle) {
            setLastMessage("Hit obstacle.");
            hit = true;
            break;
        }
        if (damageEntityAt(currentX, currentY, player_.getDamage())) {
            setLastMessage("Arrow hit target!");
            hit = true;
            break;
        }
        currentX += dx;
        currentY += dy;
    }
    if (!hit) setLastMessage("Missed.");

    game_state_ = GameState::PlayerTurn;
}

void Game::selectSpell(int index) {
    if (index >= 0 && index < player_.getHand().getSpells().size()) {
        selected_spell_index_ = index;
        game_state_ = GameState::SelectingSpellTarget;
        setLastMessage("Target for: " + player_.getHand().getSpells()[index]->getName());
    }
}

void Game::applySpell(int target_x, int target_y) {
    if (game_state_ == GameState::SelectingSpellTarget && selected_spell_index_ != -1) {
        player_.getHand().useSpell(selected_spell_index_, *this, target_x, target_y);
        selected_spell_index_ = -1;
        if (game_state_ == GameState::SelectingSpellTarget) game_state_ = GameState::PlayerTurn;
    }
}

void Game::cancelSpellSelection() {
    game_state_ = GameState::PlayerTurn;
    selected_spell_index_ = -1;
    setLastMessage("Cancelled.");
}

void Game::endPlayerTurn() {
    game_state_ = GameState::PlayerTurn;
    updateAfterPlayerTurn();
}

void Game::updateAfterPlayerTurn() {
    updateAllies();
    updateEnemies();
    updateBuildings();
    updateTowers();
    checkTraps();
    cleanupDestroyedEntities();

    if (!player_.isAlive()) {
        game_state_ = GameState::GameOver;
    } else if (enemies_.empty() && towers_.empty() && buildings_.empty()) {
        if (current_level_ >= 3) {
            setLastMessage("Area cleared! Press 'N' to finish game!");
        } else {
            setLastMessage("Area cleared! Press 'N' for next level.");
        }
    }
}

void Game::updateEnemies() {
    for (auto& enemy : enemies_) {
        int dx = player_.getX() - enemy.getX();
        int dy = player_.getY() - enemy.getY();

        if (std::abs(dx) <= 1 && std::abs(dy) <= 1) {
            damagePlayer(enemy.getDamage());
            continue;
        }
        int move_dx = (dx > 0) ? 1 : (dx < 0) ? -1 : 0;
        int move_dy = (dy > 0) ? 1 : (dy < 0) ? -1 : 0;
        int next_x = enemy.getX() + move_dx;
        int next_y = enemy.getY() + move_dy;
        if (canPlaceEntityAt(next_x, next_y)) enemy.setPosition(next_x, next_y);
    }
}

void Game::updateBuildings() {
    for (auto& building : buildings_) {
        if (building.update()) spawnEnemyNear(building.getX(), building.getY());
    }
}

void Game::updateTowers() {
    for (auto& tower : towers_) {
        tower.update(*this);
    }
}

void Game::updateAllies() {
    for (auto& ally : allies_) {
        ally.update(*this);
    }
}

void Game::checkTraps() {
    for (auto& enemy : enemies_) {
        auto it = std::find_if(traps_.begin(), traps_.end(), [&](const Trap& t){ return t.getX()==enemy.getX() && t.getY()==enemy.getY(); });
        if (it != traps_.end()) {
            enemy.takeDamage(it->getDamage());
            traps_.erase(it);
        }
    }
}

bool Game::damageEntityAt(int x, int y, int damage) {
    auto e_it = std::find_if(enemies_.begin(), enemies_.end(), [x,y](const Enemy& e){ return e.getX()==x && e.getY()==y; });
    if (e_it != enemies_.end()) {
        e_it->takeDamage(damage);
        if (!e_it->isAlive()) {
            player_.addScore(10);
            giveSpellOnKill();
        }
        return true;
    }
    auto t_it = std::find_if(towers_.begin(), towers_.end(), [x,y](const EnemyTower& t){ return t.getX()==x && t.getY()==y; });
    if (t_it != towers_.end()) {
        t_it->takeDamage(damage);
        if (t_it->isDestroyed()) {
            player_.addScore(50);
            giveSpellOnKill();
        }
        return true;
    }
    auto b_it = std::find_if(buildings_.begin(), buildings_.end(), [x,y](const EnemyBuilding& b){ return b.getX()==x && b.getY()==y; });
    if (b_it != buildings_.end()) {
        buildings_.erase(b_it);
        player_.addScore(30);
        setLastMessage("Building destroyed!");
        giveSpellOnKill();
        return true;
    }
    return false;
}

void Game::damagePlayer(int damage) { player_.takeDamage(damage); }
void Game::spawnAlly(int px, int py, int c, int h, int d) {
    for(int i=0; i<c; ++i) {
        int directions[8][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
        for(auto& dir : directions) {
            if(canPlaceEntityAt(px+dir[0], py+dir[1])) {
                allies_.emplace_back(h, d, px+dir[0], py+dir[1]);
                break;
            }
        }
    }
}
void Game::placeTrap(int x, int y, int d) { traps_.emplace_back(d, x, y); }
bool Game::canPlaceEntityAt(int x, int y, bool ip) const {
    if(!field_.isValidPosition(x,y) || field_.getCellType(x,y)==CellType::kObstacle) return false;
    if(!ip && player_.getX()==x && player_.getY()==y) return false;
    for(const auto& e: enemies_) if(e.getX()==x && e.getY()==y) return false;
    for(const auto& t: towers_) if(t.getX()==x && t.getY()==y) return false;
    for(const auto& b: buildings_) if(b.getX()==x && b.getY()==y) return false;
    for(const auto& a: allies_) if(a.getX()==x && a.getY()==y) return false;
    return true;
}
void Game::cleanupDestroyedEntities() {
    enemies_.erase(std::remove_if(enemies_.begin(), enemies_.end(), [](const Enemy& e){return !e.isAlive();}), enemies_.end());
    towers_.erase(std::remove_if(towers_.begin(), towers_.end(), [](const EnemyTower& t){return t.isDestroyed();}), towers_.end());
    allies_.erase(std::remove_if(allies_.begin(), allies_.end(), [](const Ally& a){return !a.isAlive();}), allies_.end());
}
void Game::addRandomSpell() {
    std::mt19937 rng(std::random_device{}());
    int r = std::uniform_int_distribution<int>(0,4)(rng);
    if(r==0) player_.getHand().addSpell(std::make_unique<DirectDamageSpell>(25,5));
    else if(r==1) player_.getHand().addSpell(std::make_unique<AoESpell>(15,4,1));
    else if(r==2) player_.getHand().addSpell(std::make_unique<TrapSpell>(40));
    else if(r==3) player_.getHand().addSpell(std::make_unique<SummonSpell>(1,50,15));
    else player_.getHand().addSpell(std::make_unique<BoostSpell>());
}
void Game::spawnEnemyNear(int x, int y) {
    int ehp = (current_level_==1)?40:(current_level_==2?50:60);
    int edmg = (current_level_==1)?15:(current_level_==2?20:25);

    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for (auto& dir : directions) {
        int spawnX = x + dir[0];
        int spawnY = y + dir[1];
        if (canPlaceEntityAt(spawnX, spawnY)) {
            enemies_.emplace_back(ehp, edmg, spawnX, spawnY);
            setLastMessage(last_turn_message_ + " A new enemy has spawned!");
            return;
        }
    }
}

const Field& Game::getField() const { return field_; }
Player& Game::getPlayer() { return player_; }
const std::vector<Enemy>& Game::getEnemies() const { return enemies_; }
const std::vector<EnemyBuilding>& Game::getBuildings() const { return buildings_; }
const std::vector<Ally>& Game::getAllies() const { return allies_; }
const std::vector<EnemyTower>& Game::getTowers() const { return towers_; }
const std::vector<Trap>& Game::getTraps() const { return traps_; }
GameState Game::getGameState() const { return game_state_; }
const std::string& Game::getLastMessage() const { return last_turn_message_; }
void Game::setLastMessage(const std::string& message) {
    last_turn_message_ = message;
    notifyObservers(message);
}