#include "GameController.h"
#include "GameConstants.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <limits>
#include <algorithm>
#include <sstream>
#include <iomanip>

GameController::GameController() 
    : field(GameField()),
      player(Constants::PLAYER_INITIAL_HEALTH, Constants::PLAYER_HAND_SIZE),
      gameRunning(false), 
      turnCount(0),
      randomGenerator(std::random_device{}()) {}

void GameController::startNewGame() {
    initializeGame();
}

void GameController::initializeGame() {
    std::cout << "\n=== GAME INITIALIZATION ===\n\n";
    
    int width = InputHandler::getValidatedIntegerInput(
        "Enter field width (" + 
        std::to_string(Constants::MIN_FIELD_SIZE) + " - " + 
        std::to_string(Constants::MAX_FIELD_SIZE) + "): ",
        Constants::MIN_FIELD_SIZE, Constants::MAX_FIELD_SIZE);

    int height = InputHandler::getValidatedIntegerInput(
        "Enter field height (" + 
        std::to_string(Constants::MIN_FIELD_SIZE) + " - " + 
        std::to_string(Constants::MAX_FIELD_SIZE) + "): ",
        Constants::MIN_FIELD_SIZE, Constants::MAX_FIELD_SIZE);

    field = GameField(width, height);
    
    field.spawnObstacles(Constants::OBSTACLE_PERCENTAGE);

    Position playerPos;
    if (!field.tryGetRandomEmptyPosition(playerPos, width * height)) {
        throw std::runtime_error("Failed to find empty position for player");
    }
    if (!field.placePlayer(playerPos)) {
        throw std::runtime_error("Failed to place player on field");
    }
    field.ensurePlayerHasFreeNeighbor();

    spawnEnemies();
    grantInitialSpell();
    gameRunning = true;
    turnCount = 0;
    
    std::cout << "\nGame initialized successfully!\n";
    std::cout << "Field size: " << width << "x" << height << "\n";
    std::cout << "Defeat all enemies and towers to win!\n\n";
}

void GameController::spawnEnemies() {
    int enemiesToSpawn = Constants::INITIAL_ENEMY_COUNT;
    int enemiesPlaced = 0;
    int towersToSpawn = 2;
    int towersPlaced = 0;
    int maxAttempts = (field.getWidth() * field.getHeight()) * 3;

    for (int attempt = 0; attempt < maxAttempts && enemiesPlaced < enemiesToSpawn; ++attempt) {
        Position enemyPos;
        if (field.tryGetRandomEmptyPosition(enemyPos, field.getWidth() * field.getHeight())) {
            Enemy enemy(Constants::ENEMY_HEALTH, Constants::ENEMY_DAMAGE);
            if (field.placeEnemy(enemyPos, enemy)) {
                enemiesPlaced++;
            }
        }
    }

    for (int attempt = 0; attempt < maxAttempts && towersPlaced < towersToSpawn; ++attempt) {
        Position towerPos;
        if (field.tryGetRandomEmptyPosition(towerPos, field.getWidth() * field.getHeight())) {
            int distToPlayer = std::abs(towerPos.x - field.getPlayerPosition().x) + 
                              std::abs(towerPos.y - field.getPlayerPosition().y);
            if (distToPlayer > 5) {
                EnemyTower tower(towerPos, Constants::TOWER_HEALTH, 
                               Constants::TOWER_DAMAGE, Constants::TOWER_RANGE);
                if (field.addTower(towerPos, std::move(tower))) {
                    towersPlaced++;
                    field.addToCurrentTurnLog("Tower spawned at (" + 
                                            std::to_string(towerPos.x) + ", " + 
                                            std::to_string(towerPos.y) + ")");
                }
            }
        }
    }

    if (enemiesPlaced == 0) {
        throw std::runtime_error("Failed to place any enemies on the field");
    }
    
    std::cout << "Enemies spawned: " << enemiesPlaced << "\n";
    std::cout << "Towers spawned: " << towersPlaced << "\n";
    
    if (enemiesPlaced < enemiesToSpawn) {
        std::cout << "Warning: Only placed " << enemiesPlaced << " out of " 
                  << enemiesToSpawn << " enemies due to limited space." << std::endl;
    }
}

void GameController::runGame() {
    Renderer::render(field, player, turnCount);
    
    while (gameRunning && player.isAlive()) {
        turnCount++;
        processPlayerTurn();

        if (player.isAlive() && gameRunning) {
            processEnemyTurns();
            checkGameState();
        }

        field.endTurn();
        if (player.isAlive() && gameRunning) {
            Renderer::render(field, player, turnCount);
        }
    }

    field.endTurn();
    Renderer::render(field, player, turnCount);
    
    if (field.getEnemyCount() == 0 && field.getTowerCount() == 0) {
        std::cout << "\n=== VICTORY! ===\n";
        std::cout << "You defeated all enemies and towers!\n";
        std::cout << "Turns taken: " << turnCount << "\n";
    } else if (!player.isAlive()) {
        std::cout << "\n=== DEFEAT! ===\n";
        std::cout << "You were defeated in " << turnCount << " turns.\n";
    } else {
        std::cout << "\n=== GAME EXITED ===\n";
        std::cout << "You exited the game after " << turnCount << " turns.\n";
    }
    
    InputHandler::waitForEnter();
}


void GameController::processPlayerTurn() {
    char input = InputHandler::getGameInput();

    if (input == 'Q') {
        gameRunning = false;
        return;
    }

    if (input == 'T') {
        player.switchAttackType();
        field.addToCurrentTurnLog("Switched to " + 
            std::string(player.getAttackType() == AttackType::MELEE ? "melee" : "ranged") + 
            " attack.");
        return;
    }
    else if (input == 'F') {
        std::cout << "Enter direction for ranged attack (W/A/S/D): ";
        char directionInput = InputHandler::getGameInput();

        Direction direction = DirectionHelper::convertToDirection(directionInput);
        if (direction != Direction::NONE) {
            field.performRangedAttack(direction, player);
            checkAndGrantSpells();
        } else {
            field.addToCurrentTurnLog("Invalid direction for ranged attack.");
        }
        return;
    }
    else if (input == 'R') {
        if (player.getHand().getHandSize() == 0) {
            field.addToCurrentTurnLog("You have no spells!");
            return;
        }
        displaySpellMenu();
        int spellChoice = getSpellChoice();
        if (spellChoice >= 0 && spellChoice < player.getHand().getHandSize()) {
            processSpellCast(spellChoice);
            checkAndGrantSpells();
        }
        return;
    }
    else if (input == 'X') {
        field.addToCurrentTurnLog("Player skipped turn.");
        return;
    }
    else if (input == 'K') {
        showInGameSaveMenu();
        return;
    }
    else if (input == 'L') {
        showInGameLoadMenu();
        return;
    }
    else {
        Direction direction = DirectionHelper::convertToDirection(input);
        if (direction != Direction::NONE) {
            bool actionPerformed = field.movePlayer(direction, player);
            if (!actionPerformed) {
                field.addToCurrentTurnLog("Cannot move in that direction!");
            }
            checkAndGrantSpells();
        } else {
            field.addToCurrentTurnLog("Invalid command!");
        }
    }
}

void GameController::processEnemyTurns() {
    field.moveEnemies(player);
    
    int towerDamage = field.checkTowerAttackOnPlayer(field.getPlayerPosition());
    if (towerDamage > 0) {
        player.takeDamage(towerDamage);
        field.addToCurrentTurnLog("Player health: " + std::to_string(player.getHealth()));
    }
}

void GameController::checkGameState() {
    if (!player.isAlive()) {
        gameRunning = false;
        field.addToCurrentTurnLog("*** PLAYER DEFEATED! ***");
    } else if (field.getEnemyCount() == 0 && field.getTowerCount() == 0) {
        gameRunning = false;
        field.addToCurrentTurnLog("*** ALL ENEMIES AND TOWERS DESTROYED! ***");
        field.addToCurrentTurnLog("*** VICTORY! ***");
    }
}

void GameController::displaySpellMenu() const {
    const PlayerHand& hand = player.getHand();
    std::cout << "\n=== SPELLS ===" << std::endl;
    std::cout << "Available: " << hand.getHandSize() << "/" << hand.getMaxSize() << std::endl;
    std::cout << "Enemies Killed: " << player.getEnemiesKilled() 
              << " (Progress to new spell: " << player.getEnemiesKilledSinceLastSpell() 
              << "/" << Constants::ENEMIES_KILLED_FOR_SPELL << ")" << std::endl;
    std::cout << std::endl;
    
    for (int i = 0; i < hand.getHandSize(); ++i) {
        const SpellCard& card = hand.getHand()[i];
        std::cout << i + 1 << ". " << card.getSpell()->getDescription() << std::endl;
    }
    std::cout << "0. Cancel" << std::endl;
}

int GameController::getSpellChoice() {
    int choice = InputHandler::getValidatedIntegerInput(
        "Select spell (0 to cancel): ",
        0,
        player.getHand().getHandSize());
    return choice - 1;
}

void GameController::processSpellCast(int spellIndex) {
    PlayerHand& hand = player.getHand();
    
    if (spellIndex < 0 || spellIndex >= hand.getHandSize()) {
        field.addToCurrentTurnLog("Invalid spell selection!");
        return;
    }
    
    SpellCard* card = hand.getSpell(spellIndex);
    if (!card) {
        field.addToCurrentTurnLog("ERROR: Spell card is null!");
        return;
    }
    
    Spell* spell = card->getSpell();
    if (!spell) {
        field.addToCurrentTurnLog("ERROR: Spell pointer is null!");
        return;
    }
    
    if (auto directSpell = dynamic_cast<DirectDamageSpell*>(spell)) {
        castDirectDamageSpell(directSpell, spellIndex);
    } else if (auto areaSpell = dynamic_cast<AreaDamageSpell*>(spell)) {
        castAreaDamageSpell(areaSpell, spellIndex);
    } else if (auto trapSpell = dynamic_cast<TrapSpell*>(spell)) {
        castTrapSpell(trapSpell, spellIndex);
    } else {
        field.addToCurrentTurnLog("Unknown spell type!");
    }
}

void GameController::castDirectDamageSpell(DirectDamageSpell* spell, int spellIndex) {
    if (!spell) {
        field.addToCurrentTurnLog("Error: Invalid spell pointer!");
        return;
    }
    
    std::cout << "Enter target direction (W/A/S/D): ";
    char directionInput = InputHandler::getGameInput();
    Direction direction = DirectionHelper::convertToDirection(directionInput);
    
    if (direction == Direction::NONE) {
        field.addToCurrentTurnLog("Invalid direction!");
        return;
    }
    
    Position playerPos = field.getPlayerPosition();
    bool hitTarget = field.damageTargetInLine(playerPos, direction, spell->getRange(), spell->getDamage());
    
    if (hitTarget) {
        field.addToCurrentTurnLog(spell->getName() + " hit!");
        
        if (spellIndex >= 0 && spellIndex < player.getHand().getHandSize()) {
            if (!player.getHand().removeSpell(spellIndex)) {
                field.addToCurrentTurnLog("ERROR: Failed to remove spell from hand!");
            }
        } else {
            field.addToCurrentTurnLog("ERROR: Spell index out of range!");
        }
        
        checkAndGrantSpells();
    } else {
        field.addToCurrentTurnLog(spell->getName() + " found no target! Spell remains in hand.");
    }
}

void GameController::castAreaDamageSpell(AreaDamageSpell* spell, int spellIndex) {
    if (!spell) {
        field.addToCurrentTurnLog("Error: Invalid spell pointer!");
        return;
    }
    
    std::cout << "Enter target direction (W/A/S/D): ";
    char directionInput = InputHandler::getGameInput();
    Direction direction = DirectionHelper::convertToDirection(directionInput);
    
    if (direction == Direction::NONE) {
        field.addToCurrentTurnLog("Invalid direction!");
        return;
    }
    
    Position playerPos = field.getPlayerPosition();
    Position offset = DirectionHelper::getPositionOffset(direction);
    Position targetPos = playerPos + offset;
    Position finalTargetPos = playerPos + offset;
    
    for (int i = 0; i < spell->getRange(); ++i) {
        if (!field.checkIsValidPosition(targetPos)) {
            break;
        }
        
        CellType cellType = field.getCell(targetPos).getType();
        if (cellType == CellType::OBSTACLE) {
            break;
        }
        
        std::vector<Position> enemyPositions = field.getEnemyPositions();
        bool foundEnemy = false;
        for (const auto& ep : enemyPositions) {
            if (ep == targetPos) {
                finalTargetPos = targetPos;
                foundEnemy = true;
                break;
            }
        }
        if (foundEnemy) break;
        
        std::vector<Position> towerPositions = field.getTowerPositions();
        bool foundTower = false;
        for (const auto& tp : towerPositions) {
            if (tp == targetPos) {
                finalTargetPos = targetPos;
                foundTower = true;
                break;
            }
        }
        if (foundTower) break;
        
        targetPos = targetPos + offset;
    }
    
    std::vector<Position> areaPositions = field.getAreaDamagePositions(finalTargetPos, direction, spell->getAreaSize());
    int targetsHit = 0;
    
    for (const auto& pos : areaPositions) {
        if (field.damageEnemyAt(pos, spell->getDamage())) {
            targetsHit++;
            player.incrementEnemyKill();
        }
        
        if (field.damageTowerAt(pos, spell->getDamage())) {
            targetsHit++;
            player.incrementEnemyKill();
        }
    }
    
    if (targetsHit > 0) {
        field.addToCurrentTurnLog(spell->getName() + " cast! Hit " + std::to_string(targetsHit) + " targets.");
    } else {
        field.addToCurrentTurnLog(spell->getName() + " cast! No targets hit.");
    }
    
    if (spellIndex >= 0 && spellIndex < player.getHand().getHandSize()) {
        if (!player.getHand().removeSpell(spellIndex)) {
            field.addToCurrentTurnLog("ERROR: Failed to remove spell from hand!");
        }
    } else {
        field.addToCurrentTurnLog("ERROR: Spell index out of range!");
    }
    
    checkAndGrantSpells();
}

void GameController::castTrapSpell(TrapSpell* spell, int spellIndex) {
    if (!spell) {
        field.addToCurrentTurnLog("Error: Invalid spell pointer!");
        return;
    }
    
    std::cout << "Enter target direction (W/A/S/D): ";
    char directionInput = InputHandler::getGameInput();
    Direction direction = DirectionHelper::convertToDirection(directionInput);
    
    if (direction == Direction::NONE) {
        field.addToCurrentTurnLog("Invalid direction!");
        return;
    }
    
    Position playerPos = field.getPlayerPosition();
    Position offset = DirectionHelper::getPositionOffset(direction);
    Position trapPos = playerPos + offset;
    
    if (!field.checkIsValidPosition(trapPos)) {
        field.addToCurrentTurnLog("Cannot place trap outside map bounds!");
        return;
    }
    
    if (!field.isPositionEmpty(trapPos)) {
        field.addToCurrentTurnLog("Cannot place trap on obstacle or enemy!");
        return;
    }
    
    if (field.placeTrap(trapPos, spell->getDamage())) {
        field.addToCurrentTurnLog("Trap placed at (" + std::to_string(trapPos.x) + ", " + 
                                std::to_string(trapPos.y) + ")!");
        
        if (spellIndex >= 0 && spellIndex < player.getHand().getHandSize()) {
            if (!player.getHand().removeSpell(spellIndex)) {
                field.addToCurrentTurnLog("ERROR: Failed to remove spell from hand!");
            }
        } else {
            field.addToCurrentTurnLog("ERROR: Spell index out of range!");
        }
        
        checkAndGrantSpells();
    } else {
        field.addToCurrentTurnLog("Failed to place trap!");
    }
}

void GameController::grantInitialSpell() {
    std::uniform_int_distribution<> dis(0, 2);
    
    int spellChoice = dis(randomGenerator);
    bool spellAdded = false;
    
    if (spellChoice == 0) {
        spellAdded = player.getHand().addSpell(SpellFactory::createMagicMissile());
        if (spellAdded) {
            field.addToCurrentTurnLog("You start with: Magic Missile");
        }
    } else if (spellChoice == 1) {
        spellAdded = player.getHand().addSpell(SpellFactory::createFireball());
        if (spellAdded) {
            field.addToCurrentTurnLog("You start with: Fireball");
        }
    } else {
        spellAdded = player.getHand().addSpell(SpellFactory::createTrap());
        if (spellAdded) {
            field.addToCurrentTurnLog("You start with: Trap");
        }
    }
    
    if (!spellAdded) {
        throw std::runtime_error("Failed to add initial spell to player hand");
    }
}

void GameController::checkAndGrantSpells() {
    if (player.shouldReceiveSpell()) {
        if (!player.getHand().isHandFull()) {
            std::uniform_int_distribution<> dis(0, 2);
            
            int spellChoice = dis(randomGenerator);
            bool spellAdded = false;
            
            if (spellChoice == 0) {
                spellAdded = player.getHand().addSpell(SpellFactory::createMagicMissile());
                if (spellAdded) {
                    field.addToCurrentTurnLog("*** NEW SPELL: Magic Missile ***");
                    std::cout << "\n*** YOU RECEIVED A NEW SPELL: MAGIC MISSILE ***\n" << std::endl;
                    player.resetSpellGrantCounter();
                }
            } else if (spellChoice == 1) {
                spellAdded = player.getHand().addSpell(SpellFactory::createFireball());
                if (spellAdded) {
                    field.addToCurrentTurnLog("*** NEW SPELL: Fireball ***");
                    std::cout << "\n*** YOU RECEIVED A NEW SPELL: FIREBALL ***\n" << std::endl;
                    player.resetSpellGrantCounter();
                }
            } else {
                spellAdded = player.getHand().addSpell(SpellFactory::createTrap());
                if (spellAdded) {
                    field.addToCurrentTurnLog("*** NEW SPELL: Trap ***");
                    std::cout << "\n*** YOU RECEIVED A NEW SPELL: TRAP ***\n" << std::endl;
                    player.resetSpellGrantCounter();
                }
            }
            
            if (!spellAdded) {
                field.addToCurrentTurnLog("WARNING: Spell grant failed!");
            }
        }
    }
}

SaveData GameController::getSaveData() const {
    SaveData data;
    
    data.fieldWidth = field.getWidth();
    data.fieldHeight = field.getHeight();
    
    data.playerPosition = field.getPlayerPosition();
    data.playerHealth = player.getHealth();
    data.playerMaxHealth = player.getMaxHealth();
    data.playerAttackType = player.getAttackType();
    data.enemiesKilled = player.getEnemiesKilled();
    data.enemiesKilledSinceLastSpell = player.getEnemiesKilledSinceLastSpell();
    
    const auto& hand = player.getHand().getHand();
    for (const auto& card : hand) {
        if (card.getSpell()) {
            std::string spellName = card.getSpell()->getName();
            if (spellName == "Magic Missile") {
                data.spells.push_back(SavedSpellType::MAGIC_MISSILE);
            } else if (spellName == "Fireball") {
                data.spells.push_back(SavedSpellType::FIREBALL);
            } else if (spellName == "Trap") {
                data.spells.push_back(SavedSpellType::TRAP);
            }
        }
    }
    
    auto enemyPositions = field.getEnemyPositions();
    for (const auto& pos : enemyPositions) {
        data.enemies.emplace_back(pos, Constants::ENEMY_HEALTH, Constants::ENEMY_DAMAGE);
    }
    
    const auto& towers = field.getTowers();
    for (const auto& tower : towers) {
        if (tower.isAlive()) {
            data.towers.emplace_back(tower.getPosition(),
                                   tower.getHealth(),
                                   tower.getMaxHealth(),
                                   tower.getDamage(),
                                   tower.getAttackRange(),
                                   tower.getCurrentCooldown());
        }
    }
    
    const auto& traps = field.getTraps();
    for (const auto& trap : traps) {
        if (trap.isActive()) {
            data.traps.emplace_back(trap.getPosition(),
                                  trap.getDamage(),
                                  trap.isActive());
        }
    }
    
    for (int x = 0; x < field.getWidth(); ++x) {
        for (int y = 0; y < field.getHeight(); ++y) {
            Position pos(x, y);
            CellType type = field.getCell(pos).getType();
            if (type == CellType::OBSTACLE) {
                data.obstacles.emplace_back(pos);
            }
        }
    }
    
    data.turnCount = turnCount;
    
    return data;
}

void GameController::resetForLoad() {
    gameRunning = false;
    turnCount = 0;
}

void GameController::loadFromSave(const SaveData& data) {
    try {
        resetForLoad();
        
        if (data.fieldWidth < Constants::MIN_FIELD_SIZE || 
            data.fieldWidth > Constants::MAX_FIELD_SIZE ||
            data.fieldHeight < Constants::MIN_FIELD_SIZE || 
            data.fieldHeight > Constants::MAX_FIELD_SIZE) {
            throw std::runtime_error("Invalid field size in save data");
        }
        
        if (data.playerHealth <= 0 || data.playerHealth > data.playerMaxHealth) {
            throw std::runtime_error("Invalid player health in save data");
        }
        
        field = GameField(data.fieldWidth, data.fieldHeight);
        turnCount = data.turnCount;
        
        for (const auto& obstacle : data.obstacles) {
            if (obstacle.position.x >= 0 && obstacle.position.x < data.fieldWidth &&
                obstacle.position.y >= 0 && obstacle.position.y < data.fieldHeight) {
                field.placeObstacle(obstacle.position);
            }
        }
        
        if (!field.placePlayer(data.playerPosition)) {
            Position newPos;
            if (field.tryGetRandomEmptyPosition(newPos, 100)) {
                field.placePlayer(newPos);
            } else {
                throw std::runtime_error("Cannot place player on field");
            }
        }
        
        player = Player(data.playerMaxHealth, Constants::PLAYER_HAND_SIZE);
        int damageTaken = data.playerMaxHealth - data.playerHealth;
        if (damageTaken > 0) {
            player.takeDamage(damageTaken);
        }
        
        player.setEnemiesKilled(data.enemiesKilled);
        player.setEnemiesKilledSinceLastSpell(data.enemiesKilledSinceLastSpell);
        
        if (data.playerAttackType == AttackType::RANGED) {
            player.switchAttackType();
        }
        
        player.getHand() = PlayerHand(Constants::PLAYER_HAND_SIZE);
        for (auto spellType : data.spells) {
            std::unique_ptr<Spell> spell;
            switch (spellType) {
                case SavedSpellType::MAGIC_MISSILE:
                    spell = SpellFactory::createMagicMissile();
                    break;
                case SavedSpellType::FIREBALL:
                    spell = SpellFactory::createFireball();
                    break;
                case SavedSpellType::TRAP:
                    spell = SpellFactory::createTrap();
                    break;
                default:
                    continue;
            }
            player.getHand().addSpell(std::move(spell));
        }
        
        for (const auto& savedEnemy : data.enemies) {
            if (savedEnemy.position.x >= 0 && savedEnemy.position.x < data.fieldWidth &&
                savedEnemy.position.y >= 0 && savedEnemy.position.y < data.fieldHeight &&
                savedEnemy.health > 0) {
                Enemy enemy(savedEnemy.health, savedEnemy.damage);
                field.placeEnemy(savedEnemy.position, enemy);
            }
        }
        
        for (const auto& savedTower : data.towers) {
            if (savedTower.position.x >= 0 && savedTower.position.x < data.fieldWidth &&
                savedTower.position.y >= 0 && savedTower.position.y < data.fieldHeight &&
                savedTower.health > 0) {
                EnemyTower tower(savedTower.position, savedTower.maxHealth, 
                               savedTower.damage, savedTower.range);
                
                int damageTaken = savedTower.maxHealth - savedTower.health;
                if (damageTaken > 0) {
                    tower.takeDamage(damageTaken);
                }
                
                saveManager.restoreTowerState(tower, savedTower);
                
                if (!field.addTowerWithState(savedTower.position, std::move(tower))) {
                    std::cerr << "Warning: Could not place tower at ("
                              << savedTower.position.x << ", " 
                              << savedTower.position.y << ")\n";
                }
            }
        }
        
        for (const auto& savedTrap : data.traps) {
            if (savedTrap.position.x >= 0 && savedTrap.position.x < data.fieldWidth &&
                savedTrap.position.y >= 0 && savedTrap.position.y < data.fieldHeight &&
                savedTrap.active) {
                field.placeTrap(savedTrap.position, savedTrap.damage);
            }
        }
        
        gameRunning = true;
        
        field.addToCurrentTurnLog("Game loaded successfully!");
        field.addToCurrentTurnLog("Turn: " + std::to_string(turnCount));
        
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to load game: " + std::string(e.what()));
    }
}

void GameController::showInGameSaveMenu() {
    Renderer::clearScreen();
    std::cout << "=== IN-GAME SAVE MENU ===\n\n";
    
    for (int i = 1; i <= 3; i++) {
        std::cout << "Slot " << i << ":\n";
        std::cout << "  " << saveManager.getSaveInfo(i) << "\n";
    }
    
    std::cout << "Select slot (1-3, 0 to cancel): ";
    
    int slot;
    std::cin >> slot;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (slot < 0 || slot > 3) {
        field.addToCurrentTurnLog("Invalid save slot. Save cancelled.");
        return;
    }
    
    if (slot == 0) {
        field.addToCurrentTurnLog("Save cancelled.");
        return;
    }
    
    try {
        SaveData data = getSaveData();
        if (saveManager.saveGame(data, slot)) {
            field.addToCurrentTurnLog("Game saved successfully in slot " + std::to_string(slot) + "!");
        } else {
            field.addToCurrentTurnLog("Failed to save game!");
        }
    } catch (const std::exception& e) {
        field.addToCurrentTurnLog("Save error: " + std::string(e.what()));
    }
}

void GameController::showInGameLoadMenu() {
    Renderer::clearScreen();
    std::cout << "=== IN-GAME LOAD MENU ===\n";
    std::cout << "WARNING: Current progress will be lost!\n\n";
    
    for (int i = 1; i <= 3; i++) {
        std::cout << "Slot " << i << ":\n";
        std::cout << "  " << saveManager.getSaveInfo(i) << "\n";
    }
    
    std::cout << "Select slot to load (1-3, 0 to cancel): ";
    
    int slot;
    std::cin >> slot;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (slot < 0 || slot > 3) {
        field.addToCurrentTurnLog("Invalid save slot. Load cancelled.");
        return;
    }
    
    if (slot == 0) {
        field.addToCurrentTurnLog("Load cancelled.");
        return;
    }
    
    try {
        SaveData data;
        if (saveManager.loadGame(data, slot)) {
            loadFromSave(data);
            field.addToCurrentTurnLog("Game loaded successfully from slot " + std::to_string(slot) + "!");
        } else {
            field.addToCurrentTurnLog("Failed to load game!");
        }
    } catch (const std::exception& e) {
        field.addToCurrentTurnLog("Load error: " + std::string(e.what()));
    }
}