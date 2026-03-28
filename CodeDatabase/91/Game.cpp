#include "Game.h"
#include "SaveLoad.h"
#include <iostream>
#include <string>
#include <chrono>
#include <algorithm>
#include <cctype>
#include <random>
#include <iomanip>

Game::Game(int fieldWidth, int fieldHeight, Logger* logger)
  : field(fieldWidth, fieldHeight, {Position(0, 0)}),
    gameRunning(true),
    game_won_(false),
    turnCount(0),
    logger_(logger)
{
    Position playerStartPosition(0, 0);
    player = std::make_unique<Player>("NeoN", playerStartPosition);

    int centerX = fieldWidth / 2;
    int centerY = fieldHeight / 2;
    Position towerPos(centerX, centerY);
    towers.push_back(std::make_unique<Tower>(Position(centerX, centerY), 3));

    field.getCell(towerPos).setType(CellType::TOWER);

    spawnEnemies(3);

    std::cout << "=== GAME STARTED ===" << std::endl;
}

GameResult Game::run(IInputHandler& input_handler, IGameRenderer* renderer) {
    std::cout << "\n=== LEVEL STARTED ===" << std::endl;
    std::cout << "Field size: " << field.getWidth() << "x" << field.getHeight() << std::endl;

    while (player->isAlive() && gameRunning) {
        turnCount++;

        if (turnCount % 3 == 0) {  // каждые 3 хода восстанавливается 10 маны
            player->restoreMana(10);
        }

        std::cout << "\n=== TURN " << turnCount << " ===" << std::endl;
        if (renderer) {
            renderer->render(*this);
        }
        std::cout << "Controls: WASD - move, C - change combat, Q - quit, F - ranged attack, M - cast spell, B - buy spell, X - save" << std::endl;

        std::unique_ptr<ICommand> command = nullptr;
        bool commandProcessed = false;

        while (!commandProcessed) {
            command = input_handler.getCommand();

            if (!command) {
                continue;
            }

            command->execute(*this);
            commandProcessed = true;
        }

        if (!gameRunning) {
            break;
        }

        checkCombat();
        removeDeadEnemies();

        if (!player->isAlive()) {
             break;
        }

        processEnemyTurns();
        checkCombat();
        removeDeadEnemies();
        processTowerTurns();

        if (!player->isAlive()) {
            break;
        }

        checkGameState();
    }

    if (renderer) {
        renderer->render(*this);
    }

    if (!gameRunning) {
        if (player->isAlive()) {
            if (game_won_) {
                std::cout << "\n=== VICTORY ===" << std::endl;
                std::cout << "You defeated all enemies!" << std::endl;
                std::cout << "Final score: " << player->getScore() << std::endl;
                return GameResult::WIN;
            } else {
                std::cout << "Game ended by player." << std::endl;
                return GameResult::QUIT;
            }
        }
    }

    if (!player->isAlive()) {
        if (renderer) {
            renderer->render(*this);
        }

        std::cout << "\n=== GAME OVER ===" << std::endl;
        std::cout << "Your hero has fallen in battle!" << std::endl;
        std::cout << "Final score: " << player->getScore() << std::endl;

        return GameResult::LOSE;
    }

    return GameResult::QUIT;
}

bool Game::processPlayerTurn(char input) {
    Position oldPos = player->getPosition();
    Position newPosition = oldPos;

    switch (input) {
    case 'W':
        newPosition.y -= 1;
        break;
    case 'S':
        newPosition.y += 1;
        break;
    case 'A':
        newPosition.x -= 1;
        break;
    case 'D':
        newPosition.x += 1;
        break;
    case 'C':
        player->switchCombatType();
        std::cout << "Switched to " << player->getCombatTypeString() << " combat!" << std::endl;
        if (logger_) {
            logger_->logCombatTypeSwitch(player->getName(), player->getCombatTypeString());
        }
        return true;
    case 'F':
        return processRangedAttack();
    case 'M':
        return processSpellSelection();
    case 'B':
        return processBuySpell();
    case 'Q':
        gameRunning = false;
        std::cout << "Game ended by player." << std::endl;
        return false;
    default:
        std::cout << "Invalid command! Use WASD to move, C to change combat, Q to quit." << std::endl;
        return false;
    }

    if (field.canMoveTo(newPosition)) {
        player->setPosition(newPosition);
        if (logger_) {
            logger_->logPlayerMove(oldPos.x, oldPos.y, newPosition.x, newPosition.y);
        }
        if (player->getCombatType() == CombatType::MELEE) {
            checkCombat();
            removeDeadEnemies();
        }
        return true;
    } else {
        std::cout << "Cannot move there! Cell is blocked or out of bounds." << std::endl;
        return false;
    }
}

bool Game::processRangedAttack() {
    if (player->getCombatType() != CombatType::RANGED) {
        std::cout << "You need to switch to Ranged combat first! Press C." << std::endl;
        return false;
    }

    const int attackRange = 2;
    Position playerPos = player->getPosition();

    Position target = player->findClosestEnemyInRange(enemies, attackRange);

    if (target.x == -1 && target.y == -1) {
        for (auto& tower : towers) {
            if (tower->isActive() && playerPos.distanceTo(tower->getPosition()) <= attackRange) {
                target = tower->getPosition();
                break;
            }
        }

        if (target.x == -1 && target.y == -1) {
            std::cout << "No enemies or towers in ranged attack range (2 cells)!" << std::endl;
            return false;
        }
    }

    if (!hasLineOfSight(playerPos, target)) {
        std::cout << "Target is behind an obstacle! No line of sight." << std::endl;
        return false;
    }

    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == target) {
            int damage = player->getDamage();
            int oldHealth = enemy.getHealth();
            enemy.takeDamage(damage);
            std::cout << "Ranged attack hit enemy at (" << target.x << ", " << target.y << ") for " << damage << " damage!" << std::endl;

            if (!enemy.isAlive() && oldHealth > 0) {
                if (logger_) {
                    logger_->logEnemyDefeated("Enemy", player->getName(), target.x, target.y);
                }
                player->addScore(100);
                player->incrementEnemiesDefeated();
                std::cout << "Enemy defeated! +100 points!" << std::endl;
            }
            removeDeadEnemies();
            return true;
        }
    }

    for (auto& tower : towers) {
        if (tower->isActive() && tower->getPosition() == target) {
            int damage = player->getDamage();
            tower->takeDamage(damage);
            std::cout << "Ranged attack hit tower at (" << target.x << ", " << target.y << ") for " << damage << " damage!" << std::endl;
            return true;
        }
    }

    return false;
}

bool Game::processSpellSelection() {
    const SpellHand& hand = player->getSpellHand();
    if (hand.isEmpty()) {
        std::cout << "You have no spells in your hand!\n";
        return false;
    }

    hand.displayHand();

    std::cout << "Choose spell (1-" << hand.getSpellCount() << ") or 0 to cancel: ";
    int choice;
    if (!(std::cin >> choice)) {
        std::cout << "Invalid input: expected a number.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    std::cin.ignore();

    if (choice == 0) {
        std::cout << "Spell casting cancelled.\n";
        return false;
    }
    if (choice < 1 || choice > hand.getSpellCount()) {
        std::cout << "Invalid spell number: " << choice << ". Must be between 1 and "
                  << hand.getSpellCount() << ".\n";
        return false;
    }

    int spellIndex = choice - 1;
    const ISpell* spell = hand.getSpell(spellIndex);
    if (!spell) {
        std::cout << "Error: selected spell is null!\n";
        return false;
    }

    std::cout << "Target for '" << spell->getName() << "' (enter x y): ";
    int x, y;
    if (!(std::cin >> x >> y)) {
        std::cout << "Invalid coordinates: expected two integers.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    std::cin.ignore();

    Position target(x, y);
    bool success = castSpell(spellIndex, target);

    if (success) {
        std::cout << "Spell cast successfully!\n";
    } else {
        std::cout << "Spell failed to cast.\n";
    }

    return success;
}

bool Game::processBuySpell() {
    if (!player->canBuySpell()) {
        if (player->getScore() > player->getSpellPurchaseCost()) {
            std::cout << "Hand is full.\n";
            return false;
        }
        std::cout << "Not enough score to buy a spell (need "
                  << player->getSpellPurchaseCost() << ").\n";
        return false;
    }

    std::cout << "Buy new spell for " << player->getSpellPurchaseCost()
              << " points? (Y/N): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();

    if (std::toupper(choice) == 'Y') {
        int spell_count_before = player->getSpellHand().getSpellCount();
        if (player->buySpell()) {
            int spell_count_after = player->getSpellHand().getSpellCount();

            if (spell_count_after > spell_count_before) {
                const ISpell* new_spell = player->getSpellHand().getSpell(spell_count_after - 1);
                if (new_spell && logger_) {
                    logger_->logSpellLearned(player->getName(), new_spell->getName());
                }
            }
            std::cout << "Spell purchased!\n";
            return true;
        }
    }
    std::cout << "Purchase cancelled.\n";
    return false;
}

void Game::processTowerTurns() {
    for (auto& tower : towers) {
        if (tower->isActive()) {
            tower->update(*this);
            if (tower->canAttack(*this)) {
                tower->attack(*this);
            }
        }
    }
}

bool Game::hasLineOfSight(const Position& from, const Position& to) const {
    if (from.x == to.x) {
        int startY = std::min(from.y, to.y);
        int endY = std::max(from.y, to.y);
        
        for (int y = startY + 1; y < endY; ++y) {
            Position checkPos(from.x, y);
            if (!field.isCellPassable(checkPos)) {
                return false;
            }
        }
    } else if (from.y == to.y) {
        int startX = std::min(from.x, to.x);
        int endX = std::max(from.x, to.x);
        
        for (int x = startX + 1; x < endX; ++x) {
            Position checkPos(x, from.y);
            if (!field.isCellPassable(checkPos)) {
                return false;
            }
        }
    } else {
        int dx = (to.x > from.x) ? 1 : -1;
        int dy = (to.y > from.y) ? 1 : -1;
        
        int x = from.x + dx;
        int y = from.y + dy;
        
        while (x != to.x && y != to.y) {
            Position checkPos(x, y);
            if (!field.isCellPassable(checkPos)) {
                return false;
            }
            x += dx;
            y += dy;
        }
    }
    
    return true;
}

const Enemy* Game::findEnemyAtPosition(const Position& position) const {
    for (const auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == position) {
            return &enemy;
        }
    }
    return nullptr;
}

Enemy* Game::findEnemyAtPosition(const Position& position) {
    return const_cast<Enemy*>(
        static_cast<const Game*>(this)->findEnemyAtPosition(position)
    );
}

void Game::processEnemyTurns() {
    for (auto& enemy : enemies) {
        Position oldPosition = enemy.getPosition();

        if (enemy.isAlive() && enemy.moveTowards(player->getPosition())) {
            Position desiredPosition = enemy.getPosition();

            if (field.canMoveTo(desiredPosition) && desiredPosition != player->getPosition()) {
                if (hasTrapAtPosition(desiredPosition)) {
                    auto it = std::find_if(
                        activeTraps.begin(),
                        activeTraps.end(),
                        [&](const auto& trap) {
                            return trap.first == desiredPosition;
                        }
                    );

                    if (it != activeTraps.end()) {
                        int trapDamage = it->second;
                        int oldHealth = enemy.getHealth();
                        enemy.takeDamage(trapDamage);
                        activeTraps.erase(it);
                        std::cout << "Enemy stepped on a trap! Took "
                                  << trapDamage << " damage.\n";
                        
                        if (!enemy.isAlive() && oldHealth > 0) {
                            if (logger_) {
                                logger_->logEnemyDefeated("Enemy", "Trap", desiredPosition.x, desiredPosition.y);
                            }
                        }

                        if (!enemy.isAlive()) {
                            player->addScore(100);
                            player->incrementEnemiesDefeated();
                        }
                    }
                }

                if (logger_) {
                    logger_->logEnemyMove(oldPosition.x, oldPosition.y, desiredPosition.x, desiredPosition.y);
                }
                enemy.setPosition(desiredPosition);
            } else {
                enemy.setPosition(oldPosition);
            }
        }
    }
}

void Game::checkCombat() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && enemy.getPosition() == player->getPosition()) {
            int enemyDamage = enemy.getDamage();
            int playerOldHealth = player->getHealth();
            player->takeDamage(enemyDamage);
            if (logger_) {
                logger_->logDamage("Enemy", "Player", enemyDamage);
            }
            std::cout << "Enemy attacks you for " << enemyDamage << " damage!" << std::endl;

            if (player->isAlive()) {
                int playerDamage = player->getDamage();
                int enemyOldHealth = enemy.getHealth();
                enemy.takeDamage(playerDamage);
                if (logger_) {
                    logger_->logDamage("Player", "Enemy", playerDamage);
                }
                std::cout << "You attack for " << playerDamage << " damage!" << std::endl;

                if (!enemy.isAlive() && enemyOldHealth > 0) {
                    if (logger_) {
                        logger_->logGenericMessage("Enemy at (" + std::to_string(enemy.getPosition().x) + ", " + std::to_string(enemy.getPosition().y) + ") was defeated by Player.");
                    }
                }

                if (!enemy.isAlive()) {
                    player->addScore(100);
                    player->incrementEnemiesDefeated();
                    std::cout << "Enemy defeated! +100 points!" << std::endl;
                }
            } else {
                if (logger_ && playerOldHealth > 0) {
                    logger_->logGenericMessage("Player was defeated by Enemy at (" + std::to_string(enemy.getPosition().x) + ", " + std::to_string(enemy.getPosition().y) + ").");
                }
            }
        }
    }
}

void Game::spawnEnemies(int enemyCount) {
    std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());

    for (int i = 0; i < enemyCount; ++i) {
        Position enemyPos;
        bool positionFound = false;
        int attempts = 0;

        while (!positionFound && attempts < 100) {
            std::uniform_int_distribution<> disX(0, field.getWidth()-1);
            std::uniform_int_distribution<> disY(0, field.getHeight()-1);

            enemyPos = Position(disX(gen), disY(gen));

            if (field.isCellPassable(enemyPos) && enemyPos != player->getPosition() && !findEnemyAtPosition(enemyPos)) {
                positionFound = true;
            }
            attempts++;
        }

        if (positionFound) {
            enemies.emplace_back(enemyPos);
            if (logger_) {
                logger_->logEnemySpawned("GenericEnemy", enemyPos.x, enemyPos.y);
            }
        }
    }
}

void Game::removeDeadEnemies() {
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](const Enemy& enemy) { return !enemy.isAlive(); }),
        enemies.end()
    );
}

void Game::checkGameState() {
    if (!gameRunning) {
        return;
    }

    bool allEnemiesDead = std::all_of(enemies.begin(), enemies.end(),
        [](const Enemy& enemy) { return !enemy.isAlive(); });

    if (allEnemiesDead) {
        std::cout << "All enemies defeated! You win!" << std::endl;
        game_won_ = true;
        gameRunning = false;
    }
}

void Game::damagePlayer(int damage) {
    player->takeDamage(damage);
}

bool Game::canTargetForAreaSpell(const Position& center, int /*range*/, int /*areaSize*/) const {
    return getField().isPositionValid(center);
}

bool Game::castSpell(int spellIndex, const Position& target) {
    const auto& hand = player->getSpellHand();
    if (spellIndex < 0 || spellIndex >= hand.getSpellCount()) {
        std::cout << "Invalid spell insex!\n";
        return false;
    }

    const ISpell* spell = hand.getSpell(spellIndex);
    if (!spell) {
        std::cout << "Spell not found!\n";
        return false;
    }

    if (player->getMana() < spell->getManaCost()) {
        std::cout << "Not enough mana! Need " << spell->getManaCost()
                  << ", have " << player->getMana() << ".\n";
        return false;
    }

    if (!spell->canTarget(*this, player->getPosition(), target)) {
        std::cout << "Cannot cast " << spell->getName() << " at ("
                  << target.x << ", " << target.y << "): invalid target.\n";
        return false;
    }

    player->spendMana(spell->getManaCost());
    bool success = spell->cast(*this, target);
    if (success) {
        if (logger_) {
            logger_->logSpellCast(player->getName(), spell->getName(), target.x, target.y);
        }

        std::cout << "Casted " << spell->getName() << " at ("
                  << target.x << ", " << target.y << ").\n";
    }
    return success;
}

void Game::addTrap(const Position& position, int damage) {
    if (!field.isCellPassable(position)) {
        return;
    }
    activeTraps.emplace_back(position, damage);
}

void Game::removeTrap(const Position& position) {
    activeTraps.erase(
        std::remove_if(activeTraps.begin(), activeTraps.end(),
            [&](const std::pair<Position, int>& trap) {
                return trap.first == position;
            }),
        activeTraps.end()
    );
}

bool Game::hasTrapAtPosition(const Position& position) const {
    return std::any_of(activeTraps.begin(), activeTraps.end(),
        [&](const auto& t) {
            return t.first == position;
        });
}

bool Game::hasTowerAtPosition(const Position& position) const {
    for (const auto& tower : towers) {
        if (tower->isActive() && tower->getPosition() == position) {
            return true;
        }
    }
    return false;
}

bool Game::isPlayerInRange(const Position& position, int range) const {
    return player->getPosition().distanceTo(position) <= range;
}

const std::vector<std::unique_ptr<ITower>>& Game::getTowers() const {
    return towers;
}

const std::vector<std::pair<Position, int>>& Game::getTraps() const {
    return activeTraps;
}

// === Target Validation For Spells ===
bool Game::canTargetForDamage(const Position& position) const {
    return (findEnemyAtPosition(position) != nullptr) || hasTowerAtPosition(position);
}

bool Game::canTargetForTrap(const Position& position) const {
    return field.isCellPassable(position);
}

bool Game::damageEntityAt(const Position& pos, int damage) {
    bool hit = false;

    // Damage to player
    if (player->getPosition() == pos) {
        int old_health = player->getHealth();
        player->takeDamage(damage);
        if (logger_) {
            logger_->logDamage("UnknownSource", "Player", damage, "PLAYER_DAMAGE_TAKEN");
        }

        if (!player->isAlive() && old_health > 0) {
             if (logger_) {
                 logger_->logPlayerDefeated(player->getName(), "UnknownSource", pos.x, pos.y);
             }
        }

        hit = true;
    }

    // Damage to enemy
    Enemy* enemy = findEnemyAtPosition(pos);
    if (enemy) {
        int old_health = enemy->getHealth();
        enemy->takeDamage(damage);
        if (logger_) {
            logger_->logDamage("UnknownSource", "Enemy", damage, "ENEMY_DAMAGE_TAKEN");
        }

        if (!enemy->isAlive() && old_health > 0) {
             if (logger_) {
                 logger_->logEnemyDefeated("Enemy", "UnknownSource", pos.x, pos.y);
             }
        }

        if (old_health > 0 && !enemy->isAlive()) {
            player->addScore(100);
            player->incrementEnemiesDefeated();
        }
        hit = true;
    }

    // Damage to tower
    for (auto& tower_ptr : towers) {
        if (tower_ptr && tower_ptr->isActive() && tower_ptr->getPosition() == pos) {
            tower_ptr->takeDamage(damage);
            if (logger_) {
                logger_->logTowerDamageTaken("EnemyTower", "UnknownSource", damage);
            }
            hit = true;
        }
    }

    return hit;
}

void Game::damageEnemiesInArea(const std::vector<Position>& area, int damage) {
    int killedCount = 0;
    std::vector<Position> damaged;
    int totalDamageDealt = 0;

    for (const auto& pos : area) {
        if (!field.isPositionValid(pos)) {
            continue;
        }

        bool enemyHit = false;
        for (auto& enemy : enemies) {
            if (enemy.isAlive() && enemy.getPosition() == pos) {
                int oldHP = enemy.getHealth();
                enemy.takeDamage(damage);
                if (oldHP > 0 && !enemy.isAlive()) {
                    if (logger_) {
                        logger_->logEnemyDefeated("Enemy", "AreaSpell", pos.x, pos.y);
                    }
                    ++killedCount;
                }
                damaged.push_back(pos);
                totalDamageDealt += damage;
                enemyHit = true;
                break;
            }
        }

        if (!enemyHit) {
            if (damageEntityAt(pos, damage)) {
                damaged.push_back(pos);
                totalDamageDealt += damage;
            }
        }
    }

    if (logger_ && !damaged.empty()) {
        std::vector<std::pair<int, int>> damaged_pairs;
        damaged_pairs.reserve(damaged.size());
        for (const auto& pos : damaged) {
            damaged_pairs.emplace_back(pos.x, pos.y);
        }

        int sumX = 0, sumY = 0;
        for (const auto& p : area) { sumX += p.x; sumY += p.y; }
        int centerX = sumX / area.size();
        int centerY = sumY / area.size();

        logger_->logAreaSpellDamage("UnknownAreaSpell", centerX, centerY, area.size(), 1, damaged_pairs, totalDamageDealt);
    }

    if (killedCount > 0) {
        player->addScore(killedCount * 100);
        for (int i = 0; i < killedCount; ++i) {
            player->incrementEnemiesDefeated();
        }
    }
    removeDeadEnemies();
}

void Game::placeTrapAt(const Position& position, int damage) {
    addTrap(position, damage);
}

void Game::executeCommand(std::unique_ptr<ICommand> command) {
    if (command) {
        command->execute(*this);
    }
}

void Game::executeSaveLogic() {
    std::string filepath;
    std::cout << "Enter save file path (<file_name>.json): ";
    std::getline(std::cin, filepath);

    if (filepath.empty()) {
        std::cout << "Invalid file path. Save cancelled.\n";
        return;
    }

    try {
        saveGame(*this, filepath);
        std::cout << "Game saved successfully to " << filepath << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Failed to save game: " << e.what() << std::endl;
    }
}

// === Getters ===
const Player& Game::getPlayer() const {
    return *player;
}

const std::vector<Enemy>& Game::getEnemies() const {
    return enemies;
}

const GameField& Game::getField() const {
    return field;
}

int Game::getTurnCount() const {
    return turnCount;
}

Player& Game::getPlayerRef() {
    if (!player) {
        throw std::runtime_error("Game::getPlayerRef: Player is not initialized.");
    }
    return *player;
}

void Game::setPlayerState(Player new_player) {
    *player = std::move(new_player);
}

std::vector<Enemy>& Game::getEnemiesRef() {
    return enemies;
}

std::vector<std::unique_ptr<ITower>>& Game::getTowersRef() {
    return towers;
}

// === JSON ===
void Game::loadState(const nlohmann::json& j) {
    player->loadState(j.at("player"));

    field.loadState(j.at("field"));

    const auto& enemies_json = j.at("enemies");
    std::vector<Enemy> new_enemies;
    new_enemies.reserve(enemies_json.size());
    for (const auto& enemy_json : enemies_json) {
        Enemy enemy(Position(0,0));
        enemy.loadState(enemy_json);
        new_enemies.push_back(std::move(enemy));
    }
    enemies = std::move(new_enemies);

    const auto& towers_json = j.at("towers");
    std::vector<std::unique_ptr<ITower>> new_towers;
    new_towers.reserve(towers_json.size());
    for (const auto& tower_json : towers_json) {
        std::unique_ptr<ITower> tower;
        tower_json.get_to(tower);
        new_towers.push_back(std::move(tower));
    }
    towers = std::move(new_towers);

    const auto& traps_json = j.at("activeTraps");
    std::vector<std::pair<Position, int>> new_traps;
    new_traps.reserve(traps_json.size());
    for (const auto& trap_json : traps_json) {
        Position pos = trap_json.at("first").get<Position>();
        int damage = trap_json.at("second").get<int>();
        new_traps.emplace_back(pos, damage);
    }

    for (const auto& trap_json : traps_json) {
        Position pos = trap_json.at("position").get<Position>();
        int damage = trap_json.at("damage").get<int>();
        new_traps.emplace_back(pos, damage);
    }
    activeTraps = std::move(new_traps);

    turnCount = j.at("turnCount").get<int>();

    gameRunning = true;
}