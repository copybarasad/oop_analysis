#include "../include/Game.h"
#include "../include/Direction.h"
#include "../include/SaveGameManager.h"
#include "../include/GameExceptions.h"
#include "../include/DirectDamageSpell.h"
#include "../include/AreaDamageSpell.h"
#include "../include/TrapSpell.h"
#include "../include/SummonSpell.h"
#include "../include/EnhancementSpell.h"
#include "../include/Enemy.h"
#include "../include/EnemyBuilding.h"
#include "../include/EnemyTower.h"
#include "../include/Ally.h"
#include "../include/Trap.h"
#include "../include/GameEvent.h"
#include "../include/GameEventLogger.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <random>
#include <ctime>
#include <algorithm>

Game::Game(int fieldSize, GameMode mode) 
    : fieldSize(fieldSize),
      world(std::make_unique<GameWorld>(fieldSize, fieldSize)),
      player(std::make_unique<Player>(100, 40, 20)),
      spellHand(std::make_unique<SpellHand>(5)),
      levelManager(mode),
      turnNumber(0),
      isRunning(true),
      shouldRestart(false),
      levelCompleted(false),
      lastTurnDamageDealt(0),
      lastTurnEnemiesHit(0),
      lastTurnEnemiesKilled(0) {}

void Game::initialize() {
    const LevelConfig& config = levelManager.getCurrentConfig();
    
    GameEventLogger::getInstance().enable();
    GameEventDispatcher::getInstance().addListener(&GameEventLogger::getInstance());
    
    world->initializeWorld();
    world->spawnInitialEnemies(config.enemyCount, config.enemyHealth, config.enemyDamage);
    double minDistance = (fieldSize / 3.0) * std::sqrt(2.0);
    world->spawnBuildingsAtDistance(minDistance);
    world->spawnTowersAtDistance(minDistance);
    
    if (spellHand->getSpellCount() == 0) {
        spellHand->addRandomSpell();
    }
    
    levelCompleted = false;
    
    GameEvent startEvent(GameEventType::LEVEL_STARTED, 
                         "Level " + std::to_string(levelManager.getCurrentLevel()) + " started");
    GameEventDispatcher::getInstance().dispatch(startEvent);
}

char Game::getCellSymbol(int x, int y) const {
    Position pos(x + 1, y + 1);
    
    if (pos == world->getPlayerPosition()) {
        return 'P';
    }
    
    if (world->hasBuildingAt(pos)) {
        return 'B';
    }
    
    if (world->hasTowerAt(pos)) {
        return 'T';
    }
    
    if (world->hasAliveEnemyAt(pos)) {
        return 'E';
    }
    
    if (world->isPositionOccupiedByAlly(pos)) {
        return 'A';
    }
    
    if (world->hasTrapAt(pos)) {
        return '^';
    }
    
    CellType cellType = world->getCellType(pos);
    switch (cellType) {
        case CellType::WALL:
            return '#';
        case CellType::SLOWING:
            return '~';
        case CellType::EMPTY:
            return '.';
        default:
            return '?';
    }
}

static char getCellSymbolWrapper(int x, int y, void* context) {
    Game* game = static_cast<Game*>(context);
    return game->getCellSymbol(x, y);
}

bool Game::processPlayerTurn() {
    renderer.renderTurnInfo(*player, turnNumber, player->getIsSlowed(), 
                           lastTurnDamageDealt, lastTurnEnemiesKilled, *spellHand, enhancementBuffer);
    renderer.renderField(world->getWidth(), world->getHeight(), getCellSymbolWrapper, this);
    
    std::unique_ptr<Command> command = controller.readCommand(spellHand.get());
    
    std::string cmdDesc = command->getDescription();
    
    GameEvent inputEvent(GameEventType::COMMAND_INPUT, "Command: " + cmdDesc);
    GameEventDispatcher::getInstance().dispatch(inputEvent);
    
    bool isAttack = (cmdDesc.find("Attack") != std::string::npos);
    
    bool validAction = processCommand(std::move(command));
    
    if (validAction && !isAttack) {
        lastTurnDamageDealt = 0;
        lastTurnEnemiesHit = 0;
        lastTurnEnemiesKilled = 0;
    }
    
    return validAction;
}

bool Game::processCommand(std::unique_ptr<Command> cmd) {
    if (!cmd) {
        return false;
    }
    
    GameEvent cmdEvent(GameEventType::PLAYER_MOVED, 
                       "Command: " + cmd->getDescription());
    GameEventDispatcher::getInstance().dispatch(cmdEvent);
    
    return cmd->execute(*this);
}

bool Game::handlePlayerAction(const PlayerAction& action) {
    if (action.type == ActionType::QUIT) {
        isRunning = false;
        return false;
    }
    
    if (action.type == ActionType::RESTART) {
        shouldRestart = true;
        isRunning = false;
        renderer.renderMessage("\n=== RESTARTING GAME ===");
        return false;
    }
    
    if (action.type == ActionType::INVALID) {
        return false;
    }
    
    if (player->getIsSlowed()) {
        player->setSlow(false);
        renderer.renderMessage("You are slowed and cannot move this turn.");
        return true;
    }
    
    if (action.type == ActionType::ATTACK) {
        bool isRanged = (player->getCombatMode() == CombatMode::RANGED);
        int totalDamage = 0;
        int hitCount = world->attackInDirection(action.direction, player->getCurrentDamage(), isRanged, totalDamage);
        
        int killedEnemies = world->removeDeadEnemies();
        if (killedEnemies > 0) {
            player->addScore(killedEnemies * 25);
        }
        
        lastTurnEnemiesHit = hitCount;
        lastTurnDamageDealt = totalDamage;
        lastTurnEnemiesKilled = killedEnemies;
        
        GameEvent attackEvent(GameEventType::PLAYER_ATTACKED, world->getPlayerPosition(), totalDamage,
                               "Player attacked, hit " + std::to_string(hitCount) + " enemies for " + std::to_string(totalDamage) + " damage");
        GameEventDispatcher::getInstance().dispatch(attackEvent);
        
        if (killedEnemies > 0) {
            GameEvent killEvent(GameEventType::ENEMY_KILLED, 
                               "Killed " + std::to_string(killedEnemies) + " enemies");
            GameEventDispatcher::getInstance().dispatch(killEvent);
        }
        
        if (hitCount > 0) {
            std::string message = "Hit " + std::to_string(hitCount) + " enemy(s) for total " + 
                                 std::to_string(totalDamage) + " damage!";
            if (killedEnemies > 0) {
                message += " Killed " + std::to_string(killedEnemies) + " enemies! +" + 
                          std::to_string(killedEnemies * 25) + " points";
            }
            renderer.renderMessage(message);
        } else {
            renderer.renderMessage("No enemies hit.");
        }
        return true;
    }
    
    if (action.type == ActionType::SWITCH_MODE) {
        player->switchMode();
        std::string newMode = (player->getCombatMode() == CombatMode::MELEE ? "MELEE" : "RANGED");
        GameEvent modeEvent(GameEventType::PLAYER_MODE_SWITCHED, 
                           "Switched to " + newMode + " mode");
        GameEventDispatcher::getInstance().dispatch(modeEvent);
        renderer.renderMessage("Combat mode switched to " + newMode + "! (turn used)");
        return true;
    }
    
    if (action.type == ActionType::SKIP_TURN) {
        GameEvent skipEvent(GameEventType::TURN_STARTED, "Player skipped turn");
        GameEventDispatcher::getInstance().dispatch(skipEvent);
        renderer.renderMessage("Turn skipped.");
        return true;
    }
    
    if (action.type == ActionType::CAST_SPELL) {
        if (!spellHand->hasSpell(action.spellIndex)) {
            renderer.renderMessage("Invalid spell index.");
            return false;
        }
        
        Spell* spell = spellHand->getSpell(action.spellIndex);
        Position playerPos = world->getPlayerPosition();
        Position targetPos = action.targetPosition;
        
        SpellTarget target(targetPos);
        
        int dx = target.targetPosition.getX() - playerPos.getX();
        int dy = target.targetPosition.getY() - playerPos.getY();
        int distance = std::abs(dx) + std::abs(dy);
        
        std::string spellName = spell->getName();
        
        if (spellName == "Fireball") {
            int range = 5 + enhancementBuffer.getEnhancementCount(SpellType::DIRECT_DAMAGE);
            if (distance > range) {
                renderer.renderMessage("Cell is unreachable! Distance: " + std::to_string(distance) + 
                                      ", Max range: " + std::to_string(range) + " (turn not used)");
                return false;
            }
        }
        
        if (!spell->canCast(playerPos, target, *world)) {
            if (spellName == "Fireball") {
                renderer.renderMessage("No valid target (enemy/building/tower) at that position! (turn not used)");
                return false;
            }
            
            if (spellName == "Meteor") {
                int range = 4;
                if (distance > range) {
                    renderer.renderMessage("Cell is unreachable! Distance: " + std::to_string(distance) + 
                                          ", Max range: " + std::to_string(range) + " (turn not used)");
                } else {
                    renderer.renderMessage("Cannot cast Meteor there (turn not used)");
                }
                return false;
            }
            
            if (spellName == "Trap") {
                int range = 2;
                if (distance > range) {
                    renderer.renderMessage("Cell is unreachable! Distance: " + std::to_string(distance) + 
                                          ", Max range: " + std::to_string(range) + " (turn not used)");
                } else if (playerPos == target.targetPosition) {
                    renderer.renderMessage("Cannot place trap on your position! (turn not used)");
                } else {
                    renderer.renderMessage("Cannot place trap there (occupied or wall)! (turn not used)");
                }
                return false;
            }
            
            if (spellName == "Summon") {
                renderer.renderMessage("Cannot summon ally there (occupied or wall)! (turn not used)");
                return false;
            }
            
            renderer.renderMessage("Cannot cast spell there (turn not used)");
            return false;
        }
        
        spell->cast(playerPos, target, *world, enhancementBuffer);
        spellHand->removeSpell(action.spellIndex);
        renderer.renderMessage("Cast " + spellName + "!");
        
        int killedEnemies = world->removeDeadEnemies();
        if (killedEnemies > 0) {
            player->addScore(killedEnemies * 25);
            renderer.renderMessage("Killed " + std::to_string(killedEnemies) + " enemies! +" + 
                                  std::to_string(killedEnemies * 25) + " points");
        }
        
        return true;
    }
    
    if (action.type == ActionType::BUY_SPELL) {
        if (player->getScore() >= 50) {
            if (spellHand->getSpellCount() < spellHand->getMaxSpells()) {
                player->addScore(-50);
                spellHand->addRandomSpell();
                renderer.renderMessage("Bought a new spell! -50 points");
                return true;
            } else {
                renderer.renderMessage("Spell hand is full!");
                return false;
            }
        } else {
            renderer.renderMessage("Not enough points! Need 50 points.");
            return false;
        }
    }
    
    if (action.type == ActionType::SAVE_GAME) {
        try {
            SaveGameManager saveManager;
            saveManager.saveGame(*this, levelManager.getCurrentLevel());
            GameEvent saveEvent(GameEventType::GAME_SAVED, "Game saved successfully");
            GameEventDispatcher::getInstance().dispatch(saveEvent);
            renderer.renderMessage("Game saved successfully!");
        } catch (const GameException& e) {
            renderer.renderMessage("Save failed: " + std::string(e.getFullMessage()));
        }
        return false;
    }
    
    if (action.type == ActionType::LOAD_GAME) {
        try {
            SaveGameManager saveManager;
            int loadedLevel = 1;
            saveManager.loadGame(*this, loadedLevel);
            GameEvent loadEvent(GameEventType::GAME_LOADED, "Game loaded, level " + std::to_string(loadedLevel));
            GameEventDispatcher::getInstance().dispatch(loadEvent);
            renderer.renderMessage("Game loaded successfully! Level: " + std::to_string(loadedLevel));
        } catch (const GameException& e) {
            renderer.renderMessage("Load failed: " + std::string(e.getFullMessage()));
        }
        return false;
    }
    
    if (action.type == ActionType::MOVE) {
        Position oldPos = world->getPlayerPosition();
        bool moved = world->movePlayer(action.direction, *player);
        if (moved) {
            Position newPos = world->getPlayerPosition();
            GameEvent moveEvent(GameEventType::PLAYER_MOVED, newPos,
                               "Moved from (" + std::to_string(oldPos.getX()) + "," + std::to_string(oldPos.getY()) + 
                               ") to (" + std::to_string(newPos.getX()) + "," + std::to_string(newPos.getY()) + ")");
            GameEventDispatcher::getInstance().dispatch(moveEvent);
            renderer.renderMessage("Moved successfully.");
        } else {
            renderer.renderMessage("Cannot move there.");
        }
        return true;
    }
    
    return false;
}

void Game::processEnemyTurn() {
    int killedEnemies = 0;
    int alliesKilled = 0;
    world->processEnemyTurn(*player, killedEnemies, alliesKilled);
    
    if (alliesKilled > 0) {
        player->addScore(alliesKilled * 25);
        renderer.renderMessage("Allies killed " + std::to_string(alliesKilled) + " enemies! +" + 
                              std::to_string(alliesKilled * 25) + " points");
    }
    
    if (killedEnemies > 0) {
        player->addScore(killedEnemies * 25);
        renderer.renderMessage("Destroyed " + std::to_string(killedEnemies) + " buildings/towers! +" + 
                              std::to_string(killedEnemies * 25) + " points");
    }
}

void Game::checkGameStatus() {
    if (!player->isAlive()) {
        renderer.renderGameOver(player->getScore(), turnNumber);
        handleGameOver();
        isRunning = false;
    } else {
        checkLevelCompletion();
    }
}

void Game::handleGameOver() {
    std::cout << "\n=== GAME OVER ===\n";
    
    bool validChoice = false;
    while (!validChoice) {
        std::cout << "1. Restart game\n";
        std::cout << "2. Exit to menu\n";
        std::cout << "Enter choice (1-2): ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            shouldRestart = true;
            std::cout << "\nRestarting game...\n";
            validChoice = true;
        } else if (choice == "2") {
            shouldRestart = false;
            std::cout << "\nReturning to menu...\n";
            validChoice = true;
        } else {
            std::cout << "Error: Invalid choice! Please enter 1 or 2.\n\n";
        }
    }
}

void Game::handleVictory() {
    std::cout << "\n=== CONGRATULATIONS! You completed all levels! ===\n";
    
    bool validChoice = false;
    while (!validChoice) {
        std::cout << "1. Play again\n";
        std::cout << "2. Exit to menu\n";
        std::cout << "Enter choice (1-2): ";
        
        std::string choice;
        std::getline(std::cin, choice);
        
        if (choice == "1") {
            shouldRestart = true;
            std::cout << "\nStarting new game...\n";
            validChoice = true;
        } else if (choice == "2") {
            shouldRestart = false;
            std::cout << "\nReturning to menu...\n";
            validChoice = true;
        } else {
            std::cout << "Error: Invalid choice! Please enter 1 or 2.\n\n";
        }
    }
}

bool Game::getShouldRestart() const {
    return shouldRestart;
}

void Game::run() {
    renderer.renderWelcome();
    
    while (isRunning) {
        bool validAction = processPlayerTurn();
        
        if (!isRunning) {
            break;
        }
        
        if (!validAction) {
            continue;
        }
        
        turnNumber++;
        processEnemyTurn();
        checkGameStatus();
    }
    
    std::cout << "\nThank you for playing!\n";
}

const Player& Game::getPlayer() const {
    return *player;
}

const GameWorld& Game::getWorld() const {
    return *world;
}

const SpellHand& Game::getSpellHand() const {
    return *spellHand;
}

const EnhancementBuffer& Game::getEnhancementBuffer() const {
    return enhancementBuffer;
}

const PlayerProgress& Game::getPlayerProgress() const {
    return playerProgress;
}

int Game::getTurnNumber() const {
    return turnNumber;
}

int Game::getCurrentLevel() const {
    return levelManager.getCurrentLevel();
}


GameSnapshot Game::exportState() const {
    GameSnapshot snapshot;
    
    snapshot.version = 1;
    snapshot.turnNumber = turnNumber;
    snapshot.currentLevel = levelManager.getCurrentLevel();
    snapshot.gameMode = levelManager.getGameMode();
    
    snapshot.player.health = player->getHealth();
    snapshot.player.maxHealth = 100;
    snapshot.player.meleeDamage = player->getMeleeDamage();
    snapshot.player.rangedDamage = player->getRangedDamage();
    snapshot.player.score = player->getScore();
    snapshot.player.combatMode = player->getCombatMode();
    snapshot.player.isSlowed = player->getIsSlowed();
    snapshot.player.position = world->getPlayerPosition();
    
    snapshot.progress = playerProgress;
    
    snapshot.enhancements.fireballEnhancements = enhancementBuffer.getEnhancementCount(SpellType::DIRECT_DAMAGE);
    snapshot.enhancements.meteorEnhancements = enhancementBuffer.getEnhancementCount(SpellType::AREA_DAMAGE);
    snapshot.enhancements.trapEnhancements = enhancementBuffer.getEnhancementCount(SpellType::TRAP);
    snapshot.enhancements.summonEnhancements = enhancementBuffer.getEnhancementCount(SpellType::SUMMON);
    
    for (int i = 0; i < spellHand->getSpellCount(); i++) {
        const Spell* spell = spellHand->getSpell(i);
        SpellSnapshot spellSnap;
        spellSnap.type = spell->getType();
        
        if (spell->getType() == SpellType::DIRECT_DAMAGE) {
            const DirectDamageSpell* dd = static_cast<const DirectDamageSpell*>(spell);
            spellSnap.baseDamage = dd->getBaseDamage();
            spellSnap.baseRange = dd->getBaseRange();
        } else if (spell->getType() == SpellType::AREA_DAMAGE) {
            const AreaDamageSpell* ad = static_cast<const AreaDamageSpell*>(spell);
            spellSnap.baseDamage = ad->getBaseDamage();
            spellSnap.baseRange = ad->getBaseRange();
            spellSnap.baseAreaSize = ad->getBaseAreaSize();
        } else if (spell->getType() == SpellType::TRAP) {
            const TrapSpell* ts = static_cast<const TrapSpell*>(spell);
            spellSnap.baseDamage = ts->getBaseDamage();
            spellSnap.baseRange = ts->getBaseRange();
        } else if (spell->getType() == SpellType::SUMMON) {
            const SummonSpell* ss = static_cast<const SummonSpell*>(spell);
            spellSnap.baseSummonCount = ss->getBaseSummonCount();
        }
        
        snapshot.spells.push_back(spellSnap);
    }
    
    snapshot.world.width = world->getWidth();
    snapshot.world.height = world->getHeight();
    
    const MapGrid& grid = world->getMapGrid();
    snapshot.world.grid.resize(snapshot.world.height, std::vector<CellType>(snapshot.world.width));
    for (int y = 0; y < snapshot.world.height; y++) {
        for (int x = 0; x < snapshot.world.width; x++) {
            snapshot.world.grid[y][x] = grid.getCellType(Position(x + 1, y + 1));
        }
    }
    
    const EnemyManager& enemyMgr = world->getEnemyManager();
    for (const auto& enemyPair : enemyMgr.getEnemies()) {
        EnemySnapshot enemySnap;
        enemySnap.position = enemyPair.first;
        enemySnap.health = enemyPair.second->getHealth();
        enemySnap.damage = enemyPair.second->getDamage();
        enemySnap.patrolPattern = enemyPair.second->getPatrolPattern();
        snapshot.world.enemies.push_back(enemySnap);
    }
    
    const BuildingManager& buildingMgr = world->getBuildingManager();
    for (const auto& buildingPair : buildingMgr.getBuildings()) {
        BuildingSnapshot buildingSnap;
        buildingSnap.position = buildingPair.first;
        buildingSnap.health = buildingPair.second->getHealth();
        buildingSnap.turnsUntilSpawn = buildingPair.second->getTurnsUntilSpawn();
        snapshot.world.buildings.push_back(buildingSnap);
    }
    
    const TowerManager& towerMgr = world->getTowerManager();
    for (const auto& towerPair : towerMgr.getTowers()) {
        TowerSnapshot towerSnap;
        towerSnap.position = towerPair.first;
        towerSnap.health = towerPair.second->getHealth();
        towerSnap.cooldownRemaining = towerPair.second->getCooldownRemaining();
        snapshot.world.towers.push_back(towerSnap);
    }
    
    const AllyManager& allyMgr = world->getAllyManager();
    for (const auto& allyPair : allyMgr.getAllies()) {
        AllySnapshot allySnap;
        allySnap.position = allyPair.first;
        allySnap.health = allyPair.second->getHealth();
        allySnap.damage = allyPair.second->getDamage();
        snapshot.world.allies.push_back(allySnap);
    }
    
    const TrapManager& trapMgr = world->getTrapManager();
    for (const auto& trapPair : trapMgr.getTraps()) {
        TrapSnapshot trapSnap;
        trapSnap.position = trapPair.first;
        trapSnap.damage = trapPair.second->getDamage();
        snapshot.world.traps.push_back(trapSnap);
    }
    
    return snapshot;
}

void Game::importState(const GameSnapshot& snapshot) {
    turnNumber = snapshot.turnNumber;
    levelManager.setLevel(snapshot.currentLevel);
    fieldSize = snapshot.world.width;
    playerProgress = snapshot.progress;
    
    world = std::make_unique<GameWorld>(snapshot.world.width, snapshot.world.height);
    player = std::make_unique<Player>(snapshot.player.health, snapshot.player.meleeDamage, snapshot.player.rangedDamage);
    
    if (snapshot.player.combatMode == CombatMode::RANGED && player->getCombatMode() == CombatMode::MELEE) {
        player->switchMode();
    }
    player->setSlow(snapshot.player.isSlowed);
    player->addScore(snapshot.player.score);
    
    world->setPlayerPosition(snapshot.player.position);
    
    MapGrid& grid = world->getGrid();
    for (int y = 0; y < snapshot.world.height; y++) {
        for (int x = 0; x < snapshot.world.width; x++) {
            Position pos(x + 1, y + 1);
            CellType savedType = snapshot.world.grid[y][x];
            grid.setCellType(pos, savedType);
        }
    }
    
    spellHand = std::make_unique<SpellHand>(5);
    for (const auto& spellSnap : snapshot.spells) {
        std::unique_ptr<Spell> spell;
        switch (spellSnap.type) {
            case SpellType::DIRECT_DAMAGE:
                spell = std::make_unique<DirectDamageSpell>(spellSnap.baseDamage, spellSnap.baseRange);
                break;
            case SpellType::AREA_DAMAGE:
                spell = std::make_unique<AreaDamageSpell>(spellSnap.baseDamage, spellSnap.baseRange, spellSnap.baseAreaSize);
                break;
            case SpellType::TRAP:
                spell = std::make_unique<TrapSpell>(spellSnap.baseDamage, spellSnap.baseRange);
                break;
            case SpellType::SUMMON:
                spell = std::make_unique<SummonSpell>(spellSnap.baseSummonCount);
                break;
            case SpellType::ENHANCEMENT:
                spell = std::make_unique<EnhancementSpell>();
                break;
        }
        if (spell) {
            spellHand->addSpell(std::move(spell));
        }
    }
    
    enhancementBuffer = EnhancementBuffer();
    int maxEnhancements = std::max({
        snapshot.enhancements.fireballEnhancements,
        snapshot.enhancements.meteorEnhancements,
        snapshot.enhancements.trapEnhancements,
        snapshot.enhancements.summonEnhancements
    });
    for (int i = 0; i < maxEnhancements; i++) {
        enhancementBuffer.addEnhancement();
    }
    
    EnemyManager& enemyMgr = world->getEnemyManagerNonConst();
    for (const auto& enemySnap : snapshot.world.enemies) {
        auto enemy = std::make_unique<Enemy>(enemySnap.health, enemySnap.damage, enemySnap.patrolPattern);
        enemyMgr.addEnemy(enemySnap.position, std::move(enemy));
    }
    
    BuildingManager& buildingMgr = world->getBuildingManagerNonConst();
    for (const auto& buildingSnap : snapshot.world.buildings) {
        auto building = std::make_unique<EnemyBuilding>(5, buildingSnap.health);
        building->setTurnsUntilSpawn(buildingSnap.turnsUntilSpawn);
        buildingMgr.addBuilding(buildingSnap.position, std::move(building));
    }
    
    TowerManager& towerMgr = world->getTowerManagerNonConst();
    for (const auto& towerSnap : snapshot.world.towers) {
        auto tower = std::make_unique<EnemyTower>(15, 2, 2, towerSnap.health);
        tower->setCurrentCooldown(towerSnap.cooldownRemaining);
        towerMgr.addTower(towerSnap.position, std::move(tower));
    }
    
    AllyManager& allyMgr = world->getAllyManagerNonConst();
    for (const auto& allySnap : snapshot.world.allies) {
        auto ally = std::make_unique<Ally>(allySnap.health, allySnap.damage);
        allyMgr.addAlly(allySnap.position, std::move(ally));
    }
    
    TrapManager& trapMgr = world->getTrapManagerNonConst();
    for (const auto& trapSnap : snapshot.world.traps) {
        auto trap = std::make_unique<Trap>(trapSnap.damage);
        trapMgr.addTrap(trapSnap.position, std::move(trap));
    }
}

void Game::checkLevelCompletion() {
    const LevelConfig& config = levelManager.getCurrentConfig();
    if (player->getScore() >= config.victoryScore && !levelCompleted) {
        levelCompleted = true;
        std::cout << "\n=== LEVEL " << levelManager.getCurrentLevel() << " COMPLETED! ===\n";
        
        if (levelManager.hasNextLevel()) {
            std::cout << "Advancing to next level...\n";
            handleLevelTransition();
        } else {
            handleVictory();
            isRunning = false;
        }
    }
}

void Game::handleLevelTransition() {
    int currentMeleeDamage = player->getMeleeDamage();
    int currentRangedDamage = player->getRangedDamage();
    
    trimSpellHand();
    
    int meleeDamageUpgrade = 0;
    int rangedDamageUpgrade = 0;
    int healthBonus = 0;
    handlePlayerUpgrade(meleeDamageUpgrade, rangedDamageUpgrade, healthBonus);
    
    player = std::make_unique<Player>(100 + healthBonus, currentMeleeDamage + meleeDamageUpgrade, currentRangedDamage + rangedDamageUpgrade);
    
    levelManager.advanceToNextLevel();
    initializeLevel(levelManager.getCurrentLevel());
    
    playerProgress.level = levelManager.getCurrentLevel();
}

void Game::handlePlayerUpgrade(int& meleeDamageUpgrade, int& rangedDamageUpgrade, int& healthBonus) {
    std::cout << "\n=== LEVEL UP! Choose upgrade: ===\n";
    std::cout << "1. Increase melee damage (+20)\n";
    std::cout << "2. Increase ranged damage (+10)\n";
    std::cout << "3. Increase max health (+20 HP)\n";
    std::cout << "Enter choice (1-3): ";
    
    std::string choice;
    std::getline(std::cin, choice);
    
    if (choice == "1") {
        meleeDamageUpgrade = 20;
        std::cout << "Melee damage increased by 20!\n";
    } else if (choice == "2") {
        rangedDamageUpgrade = 10;
        std::cout << "Ranged damage increased by 10!\n";
    } else if (choice == "3") {
        healthBonus = 20;
        std::cout << "Max health increased by 20!\n";
    } else {
        std::cout << "Invalid choice, no upgrade applied.\n";
    }
}

void Game::trimSpellHand() {
    int currentCount = spellHand->getSpellCount();
    int toRemove = currentCount / 2;
    
    static std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)) + 999);
    
    for (int i = 0; i < toRemove; i++) {
        if (spellHand->getSpellCount() > 0) {
            std::uniform_int_distribution<int> dist(0, spellHand->getSpellCount() - 1);
            int randomIndex = dist(rng);
            spellHand->removeSpell(randomIndex);
        }
    }
    
    std::cout << "Randomly removed " << toRemove << " spells from hand.\n";
}

void Game::initializeLevel(int level) {
    levelManager.setLevel(level);
    const LevelConfig& config = levelManager.getCurrentConfig();
    
    fieldSize = config.fieldSize;
    world = std::make_unique<GameWorld>(config.fieldSize, config.fieldSize);
    world->initializeWorld();
    world->spawnInitialEnemies(config.enemyCount, config.enemyHealth, config.enemyDamage);
    
    double minDistance = (fieldSize / 3.0) * std::sqrt(2.0);
    world->spawnBuildingsAtDistance(minDistance);
    world->spawnTowersAtDistance(minDistance);
    
    levelCompleted = false;
}

bool Game::executeMove(Direction dir) {
    PlayerAction action(ActionType::MOVE, dir);
    return handlePlayerAction(action);
}

bool Game::executeAttack(Direction dir) {
    PlayerAction action(ActionType::ATTACK, dir);
    return handlePlayerAction(action);
}

bool Game::executeSwitchMode() {
    PlayerAction action(ActionType::SWITCH_MODE);
    return handlePlayerAction(action);
}

bool Game::executeSkipTurn() {
    PlayerAction action(ActionType::SKIP_TURN);
    return handlePlayerAction(action);
}

bool Game::executeCastSpell(int spellIndex, Position target) {
    PlayerAction action(ActionType::CAST_SPELL, Direction::NONE, spellIndex, target);
    return handlePlayerAction(action);
}

bool Game::executeBuySpell() {
    PlayerAction action(ActionType::BUY_SPELL);
    return handlePlayerAction(action);
}

bool Game::executeSaveGame() {
    PlayerAction action(ActionType::SAVE_GAME);
    return handlePlayerAction(action);
}

bool Game::executeRestart() {
    PlayerAction action(ActionType::RESTART);
    return handlePlayerAction(action);
}

bool Game::executeQuit() {
    PlayerAction action(ActionType::QUIT);
    return handlePlayerAction(action);
}
