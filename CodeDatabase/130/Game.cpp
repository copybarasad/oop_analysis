#include "Game.h"
#include "GameWorldFacade.h"
#include "GameExceptions.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhanceSpell.h"
#include "GameEvent.h"
#include "InputReader.h"
#include "Renderer.h"
#include <filesystem>
#include <sstream>
#include <iostream>
#include <chrono>
#include <random>
#include <cmath>

Game::Game()
    : field_(nullptr),
      player_(std::make_unique<Player>(100, 20, 15)),
      enemyManager_(std::make_unique<EnemyManager>()),
      buildingManager_(std::make_unique<BuildingManager>()),
      trapManager_(std::make_unique<TrapManager>()),
      allyManager_(std::make_unique<AllyManager>()),
      towerManager_(std::make_unique<TowerManager>()),
      hand_(std::make_unique<Hand>(5)),
      saveManager_(std::make_unique<GameSaveManager>()),
      inputConfig_(),
      eventBus_(std::make_shared<EventBus>()),
      gameRunning_(false),
      currentTurn_(0),
      enemiesKilled_(0),
      currentLevel_(1),
      hasUsedRevive_(false)
{
}

void Game::initializeGame() {
    currentLevel_ = 1;
    hasUsedRevive_ = false;
    appliedUpgrades_.clear();
    bool loaded = inputConfig_.loadFromFile("controls.cfg");
    if (!loaded) {
        std::filesystem::path alt = std::filesystem::current_path().parent_path() / "controls.cfg";
        if (std::filesystem::exists(alt)) {
            loaded = inputConfig_.loadFromFile(alt.string());
        }
    }
    if (!loaded) {
        std::cout << "Using default controls\n";
    }
    loadLevel(currentLevel_);
    initializeHand();
    gameRunning_ = true;
    currentTurn_ = 0;
    enemiesKilled_ = 0;
}

InputConfig& Game::getInputConfig() { return inputConfig_; }
const InputConfig& Game::getInputConfig() const { return inputConfig_; }
bool Game::canProcessInput() const { return player_ && player_->isAlive() && player_->canPlayerMove(); }
bool Game::isGameRunning() const { return gameRunning_; }
int Game::getCurrentTurn() const { return currentTurn_; }

void Game::skipSlowedTurn() {
    if (player_ && !player_->canPlayerMove()) {
        std::cout << "You are slowed and need a moment to recover. Turn skipped!\n";
        player_->setMoveAllowed(true);
    }
}

void Game::setEventBus(const std::shared_ptr<EventBus>& bus) {
    eventBus_ = bus ? bus : std::make_shared<EventBus>();
}

GameRenderData Game::getRenderData() const {
    GameRenderData data;
    data.currentTurn = currentTurn_;
    data.currentLevel = currentLevel_;
    
    if (field_) {
        data.fieldWidth = field_->getWidth();
        data.fieldHeight = field_->getHeight();
        data.cells.resize(data.fieldHeight);
        for (int y = 0; y < data.fieldHeight; ++y) {
            data.cells[y].resize(data.fieldWidth);
            for (int x = 0; x < data.fieldWidth; ++x) {
                data.cells[y][x] = field_->getCellType(x, y);
            }
        }
    }
    
    if (player_) {
        data.playerPos = player_->getPosition();
        data.playerHealth = player_->getHealth();
        data.playerMaxHealth = player_->getMaxHealth();
        data.playerDamage = player_->getDamage();
        data.playerScore = player_->getScore();
        data.playerCanMove = player_->canPlayerMove();
        data.playerSlowed = player_->isPlayerSlowed();
        data.playerMode = player_->getMode();
    }
    
    data.enemiesKilled = enemiesKilled_;
    data.enemyCount = enemyManager_ ? enemyManager_->getEnemyCount() : 0;
    data.buildingCount = buildingManager_ ? buildingManager_->getBuildingCount() : 0;
    data.allyCount = allyManager_ ? allyManager_->getAllyCount() : 0;
    data.towerCount = towerManager_ ? towerManager_->getTowerCount() : 0;
    data.trapCount = trapManager_ ? trapManager_->getTrapCount() : 0;

    // Позиции сущностей
    if (enemyManager_) {
        for (auto* e : enemyManager_->getEnemies()) {
            if (e && e->isAlive()) data.enemyPositions.push_back(e->getPosition());
        }
    }
    if (allyManager_) {
        for (const auto& a : allyManager_->getAllies()) {
            if (a && a->isAlive()) data.allyPositions.push_back(a->getPosition());
        }
    }
    if (buildingManager_) {
        for (const auto& b : buildingManager_->getBuildings()) {
            if (b.building && b.building->isAlive()) data.buildingPositions.push_back(b.pos);
        }
    }
    if (towerManager_) {
        for (const auto& t : towerManager_->getTowers()) {
            if (t && t->isAlive()) data.towerPositions.push_back(t->getPosition());
        }
    }
    if (trapManager_) {
        for (const auto& t : trapManager_->getTraps()) {
            if (t) data.trapPositions.push_back(t->getPosition());
        }
    }
    
    // Заклинания
    if (hand_) {
        data.handMaxSize = hand_->getMaxSize();
        for (size_t i = 0; i < hand_->getSpellCount(); ++i) {
            const Spell* spell = hand_->getSpell(i);
            if (spell) {
                GameRenderData::SpellInfo info;
                info.name = spell->getName();
                info.description = spell->getDescription();
                info.type = spell->getType();
                data.handSpells.push_back(info);
            }
        }
        const auto& enh = hand_->getEnhancementState();
        data.damageBonus = enh.damageBonus;
        data.rangeBonus = enh.rangeBonus;
        data.areaBonus = enh.areaBonus;
        data.summonBonus = enh.summonBonus;
    }
    
    data.keyBindings = inputConfig_.getBindings();
    return data;
}

void Game::displayGameState() {
    ConsoleRenderer renderer;
    renderer.render(*const_cast<Game*>(this));
}

void Game::startGame() {
    std::cout << "=== Turn-based OOP Game ===\n";
    initializeGame();
    runGameLoop();
}

void Game::runGameLoop() {
    ConsoleInputReader reader(inputConfig_);
    ConsoleRenderer renderer;
    
    while (gameRunning_) {
        renderer.render(*this);
        
        if (!canProcessInput()) {
            if (player_ && !player_->canPlayerMove()) {
                skipSlowedTurn();
                processTurnEnd();
            } else {
                gameRunning_ = false;
            }
            continue;
        }
        
        Command command = reader.readCommand();
        if (command.type == CommandType::INVALID || command.type == CommandType::NONE) {
            std::cout << "Unknown command\n";
            continue;
        }
        
        bool turnExecuted = handleCommand(command);
        if (!gameRunning_) break;
        
        if (turnExecuted) {
            processEnemyTurns();
            processTurnEnd();
        }
    }
}

void Game::processTurnEnd() {
    if (!player_->isAlive()) { handlePlayerDeath(); return; }
    allyManager_->processAllyMovement(*player_, *enemyManager_, *buildingManager_, *towerManager_, 
                                       field_->getGrid(), field_->getWidth(), field_->getHeight());
    towerManager_->processTowerAttacks(*player_);
    if (!player_->isAlive()) { handlePlayerDeath(); return; }
    
    int before = enemyManager_->getEnemyCount();
    enemyManager_->removeDeadEnemies();
    enemiesKilled_ += (before - enemyManager_->getEnemyCount());
    
    allyManager_->removeDeadAllies();
    towerManager_->removeDestroyedTowers();
    buildingManager_->removeDeadBuildings();
    buildingManager_->updateBuildings(*enemyManager_, *player_, allyManager_.get(), towerManager_.get(),
                                       field_->getGrid(), field_->getWidth(), field_->getHeight());
    tryGivePlayerSpell();
    player_->resetMovement();
    checkGameEnd();
    ++currentTurn_;
}

void Game::processEnemyTurns() {
    enemyManager_->processEnemyMovement(*player_, *buildingManager_, allyManager_.get(), towerManager_.get(),
                                         field_->getGrid(), field_->getWidth(), field_->getHeight());
    for (Enemy* enemy : enemyManager_->getEnemies()) {
        if (!enemy || !enemy->isAlive()) continue;
        Pos pos = enemy->getPosition();
        if (trapManager_->hasTrapOnCell(pos.x, pos.y)) {
            int dmg = trapManager_->activateTrap(pos.x, pos.y);
            if (dmg > 0) {
                enemy->takeDamage(dmg);
                std::cout << "Enemy triggered trap at (" << pos.x << "," << pos.y << ") for " << dmg << " damage!\n";
            }
        }
    }
}

void Game::checkGameEnd() {
    if (enemyManager_->getEnemyCount() == 0 && buildingManager_->getBuildingCount() == 0 && 
        towerManager_->getTowerCount() == 0) {
        transitionToNextLevel();
    }
}

bool Game::handleCommand(const Command& command) {
    switch (command.type) {
        case CommandType::QUIT: showQuitMenu(); return false;
        case CommandType::SWITCH_MODE: player_->switchMode(); return false;
        case CommandType::CAST_SPELL: return castSpellCommand();
        case CommandType::SAVE: return saveGameCommand();
        case CommandType::MOVE_UP: return movePlayerTo(0, -1);
        case CommandType::MOVE_DOWN: return movePlayerTo(0, 1);
        case CommandType::MOVE_LEFT: return movePlayerTo(-1, 0);
        case CommandType::MOVE_RIGHT: return movePlayerTo(1, 0);
        case CommandType::ATTACK: return attackInDirection(command.dx, command.dy);
        default: std::cout << "Unknown command\n"; return false;
    }
}

bool Game::movePlayerTo(int dx, int dy) {
    if (!player_ || !player_->canPlayerMove()) { nearbyEnemiesAttackPlayer(); return false; }
    Pos cur = player_->getPosition();
    int nx = cur.x + dx, ny = cur.y + dy;
    
    if (!field_->isValidPosition(nx, ny) || !field_->isCellPassable(nx, ny)) {
        std::cout << "Cannot move there\n"; nearbyEnemiesAttackPlayer(); return false;
    }
    if (buildingManager_->hasBuildingOnCell(nx, ny) || towerManager_->hasTowerOnCell(nx, ny) ||
        allyManager_->hasAllyOnCell(nx, ny)) {
        std::cout << "Path blocked!\n"; nearbyEnemiesAttackPlayer(); return false;
    }
    if (enemyManager_->hasEnemyOnCell(nx, ny)) {
        enemyManager_->getEnemyAt(nx, ny).takeDamage(player_->getDamage());
        player_->setMoveAllowed(false);
        if (eventBus_) eventBus_->publish({GameEventType::DAMAGE, "Player collided with enemy"});
        nearbyEnemiesAttackPlayer();
        return true;
    }
    player_->setPosition(nx, ny);
    if (field_->getCellType(nx, ny) == CellType::SLOW) player_->setSlowed(true);
    if (eventBus_) eventBus_->publish({GameEventType::MOVE, "Player moved to (" + std::to_string(nx) + "," + std::to_string(ny) + ")"});
    return true;
}

bool Game::attackInDirection(int dx, int dy) {
    if (!player_ || (dx == 0 && dy == 0)) return false;
    Pos cur = player_->getPosition();
    int nx = cur.x + dx, ny = cur.y + dy;
    if (!field_->isValidPosition(nx, ny)) { std::cout << "Out of bounds\n"; nearbyEnemiesAttackPlayer(); return false; }
    
    bool attacked = false;
    if (enemyManager_->hasEnemyOnCell(nx, ny)) {
        enemyManager_->getEnemyAt(nx, ny).takeDamage(player_->getDamage());
        std::cout << "Attacked enemy for " << player_->getDamage() << " damage!\n";
        attacked = true;
    } else if (towerManager_->hasTowerOnCell(nx, ny)) {
        towerManager_->getTowerAt(nx, ny).takeDamage(player_->getDamage());
        std::cout << "Attacked tower!\n";
        attacked = true;
    } else if (buildingManager_->hasBuildingOnCell(nx, ny)) {
        buildingManager_->getBuildingAt(nx, ny).building->takeDamage(player_->getDamage());
        std::cout << "Attacked building!\n";
        attacked = true;
    } else {
        std::cout << "Nothing to attack there.\n";
    }
    nearbyEnemiesAttackPlayer();
    return attacked;
}

void Game::nearbyEnemiesAttackPlayer() {
    if (!player_ || !player_->isAlive()) return;
    Pos pos = player_->getPosition();
    for (auto& dir : std::vector<Pos>{{1,0},{-1,0},{0,1},{0,-1}}) {
        int nx = pos.x + dir.x, ny = pos.y + dir.y;
        if (field_->isValidPosition(nx, ny) && enemyManager_->hasEnemyOnCell(nx, ny)) {
            Enemy& e = enemyManager_->getEnemyAt(nx, ny);
            if (e.isAlive()) { player_->takeDamage(e.getDamage()); e.skipNextMove(); }
        }
    }
}

bool Game::castSpellCommand() {
    if (!hand_ || hand_->getSpellCount() == 0) { std::cout << "No spells!\n"; return false; }
    
    std::cout << "\nAvailable spells:\n";
    for (size_t i = 0; i < hand_->getSpellCount(); ++i) {
        if (auto* s = hand_->getSpell(i)) std::cout << i << ": " << s->getDescription() << "\n";
    }
    
    std::cout << "\nPlayer: (" << player_->getPosition().x << "," << player_->getPosition().y << ")\n";
    std::cout << "Enemies: ";
    for (auto* e : enemyManager_->getEnemies()) {
        if (e) std::cout << "(" << e->getPosition().x << "," << e->getPosition().y << ") ";
    }
    std::cout << "\n";
    
    std::cout << "Choose spell (-1 to cancel): ";
    int idx; if (!(std::cin >> idx)) { std::cin.clear(); std::string d; std::getline(std::cin, d); return false; }
    if (idx < 0 || idx >= (int)hand_->getSpellCount()) { std::cout << "Cancelled.\n"; return false; }
    
    const Spell* spell = hand_->getSpell(idx);
    if (!spell) return false;
    std::string name = spell->getName();
    Pos target = player_->getPosition();
    
    if (spell->getType() != SpellType::ENHANCE) {
        std::cout << "Target X: "; if (!(std::cin >> target.x)) { std::cin.clear(); std::string d; std::getline(std::cin, d); return false; }
        std::cout << "Target Y: "; if (!(std::cin >> target.y)) { std::cin.clear(); std::string d; std::getline(std::cin, d); return false; }
    }
    
    GameWorldFacade facade(player_.get(), enemyManager_.get(), buildingManager_.get(),
                           towerManager_.get(), trapManager_.get(), allyManager_.get(), field_.get());
    SpellContext context(&facade, target);
    bool ok = hand_->castSpell(idx, context);
    std::cout << (ok ? "Spell cast!\n" : "Failed!\n");
    if (ok && eventBus_) eventBus_->publish({GameEventType::SPELL_CAST, "Spell: " + name});
    return ok;
}

void Game::tryGivePlayerSpell() {
    static int lastReward = 0;
    if (enemiesKilled_ >= lastReward + 3) {
        std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
        auto spell = Hand::getRandomSpell(rng);
        if (auto* t = dynamic_cast<TrapSpell*>(spell.get())) t->setTrapManager(trapManager_.get());
        else if (auto* s = dynamic_cast<SummonSpell*>(spell.get())) s->setAllyManager(allyManager_.get());
        if (hand_->addSpell(std::move(spell))) {
            std::cout << "\n*** New spell! ***\n";
            if (eventBus_) eventBus_->publish({GameEventType::SPELL_DRAW, "New spell"});
            lastReward = enemiesKilled_;
        }
    }
}

void Game::setupEnemies(int count, int hp, int damage) {
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distX(field_->getWidth()/2, field_->getWidth()-2);
    std::uniform_int_distribution<int> distY(field_->getHeight()/2, field_->getHeight()-2);
    for (int i = 0; i < count; ++i) {
        for (int a = 0; a < 100; ++a) {
            int x = distX(rng), y = distY(rng);
            if (field_->isValidPosition(x,y) && field_->isCellPassable(x,y) &&
                !enemyManager_->hasEnemyOnCell(x,y) && !buildingManager_->hasBuildingOnCell(x,y)) {
                enemyManager_->addEnemy(std::make_unique<Enemy>(hp, damage), x, y);
                break;
            }
        }
    }
}

void Game::setupBuildings(int count, int cooldown) {
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distX(field_->getWidth()/3, 2*field_->getWidth()/3);
    std::uniform_int_distribution<int> distY(field_->getHeight()/3, 2*field_->getHeight()/3);
    for (int i = 0; i < count; ++i) {
        for (int a = 0; a < 100; ++a) {
            int x = distX(rng), y = distY(rng);
            if (field_->isValidPosition(x,y) && field_->isCellPassable(x,y) &&
                !enemyManager_->hasEnemyOnCell(x,y) && !buildingManager_->hasBuildingOnCell(x,y)) {
                buildingManager_->addBuilding(std::make_unique<EnemyBuilding>(cooldown), x, y);
                break;
            }
        }
    }
}

void Game::setupTowers(int count) {
    Pos ppos = player_->getPosition();
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    for (int i = 0; i < count; ++i) {
        for (int a = 0; a < 100; ++a) {
            std::uniform_int_distribution<int> distX(0, field_->getWidth()-1);
            std::uniform_int_distribution<int> distY(0, field_->getHeight()-1);
            int x = distX(rng), y = distY(rng);
            if (std::abs(x-ppos.x)+std::abs(y-ppos.y) >= 5 && field_->isValidPosition(x,y) && 
                field_->isCellPassable(x,y) && !enemyManager_->hasEnemyOnCell(x,y) &&
                !buildingManager_->hasBuildingOnCell(x,y) && !towerManager_->hasTowerOnCell(x,y)) {
                towerManager_->addTower(std::make_unique<EnemyTower>(Pos{x,y}, 3, 10, 3, 80));
                break;
            }
        }
    }
}

void Game::initializeHand() {
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    auto spell = Hand::getRandomSpell(rng);
    if (auto* t = dynamic_cast<TrapSpell*>(spell.get())) t->setTrapManager(trapManager_.get());
    else if (auto* s = dynamic_cast<SummonSpell*>(spell.get())) s->setAllyManager(allyManager_.get());
    hand_->addSpell(std::move(spell));
    std::cout << "You start with one spell!\n";
}

void Game::loadLevel(int levelNum) {
    Level level = Level::generateLevel(levelNum);
    currentLevel_ = levelNum;
    field_ = std::make_unique<GameField>(level.getFieldWidth(), level.getFieldHeight());
    player_->setPosition(field_->findFirstPassableCell());
    enemyManager_ = std::make_unique<EnemyManager>();
    buildingManager_ = std::make_unique<BuildingManager>();
    towerManager_ = std::make_unique<TowerManager>();
    trapManager_ = std::make_unique<TrapManager>();
    if (allyManager_) allyManager_->removeDeadAllies();
    setupEnemies(level.getEnemyCount(), level.getEnemyHP(), level.getEnemyDamage());
    setupBuildings(level.getBuildingCount(), level.getBuildingCooldown());
    setupTowers(level.getTowerCount());
    std::cout << "\n=== LEVEL " << currentLevel_ << " ===\n";
}

void Game::transitionToNextLevel() {
    if (currentLevel_ >= 10) {
        std::cout << "\n*** YOU WON! ***\n";
        saveManager_->deleteAllSaves();
        gameRunning_ = false;
        return;
    }
    std::cout << "\n=== LEVEL " << currentLevel_ << " COMPLETE! ===\n";
    player_->restoreFullHealth();
    removeHalfSpells();
    showUpgradeMenu();
    currentLevel_++;
    loadLevel(currentLevel_);
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    if (auto s = Hand::getRandomSpell(rng)) hand_->addSpell(std::move(s));
    autoSaveGame();
}

void Game::removeHalfSpells() {
    size_t cnt = hand_->getSpellCount();
    if (cnt <= 1) { hand_->clearAllSpells(); return; }
    std::mt19937 rng(std::chrono::system_clock::now().time_since_epoch().count());
    for (size_t i = 0; i < cnt/2 && hand_->getSpellCount() > 0; ++i) {
        std::uniform_int_distribution<size_t> dist(0, hand_->getSpellCount()-1);
        hand_->removeSpellAt(dist(rng));
    }
}

void Game::showUpgradeMenu() {
    std::cout << "\n=== UPGRADE ===\n1. Player (+20 HP, +5 Dmg)\n2. Spells (+1 Range/Area, +5 Dmg)\nChoose: ";
    int c; if (!(std::cin >> c)) { std::cin.clear(); std::string d; std::getline(std::cin, d); c = 1; }
    applyUpgrade(c == 2 ? UpgradeType::UPGRADE_SPELLS : UpgradeType::UPGRADE_PLAYER);
}

void Game::applyUpgrade(UpgradeType upgrade) {
    appliedUpgrades_.push_back(upgrade);
    if (upgrade == UpgradeType::UPGRADE_PLAYER) {
        player_->increaseMaxHealth(20); player_->increaseDamage(5);
        std::cout << "Player upgraded!\n";
    } else {
        EnhancementState b; b.rangeBonus = 1; b.areaBonus = 1; b.damageBonus = 5; b.summonBonus = 1;
        hand_->addPermanentSpellBonus(b);
        std::cout << "Spells upgraded!\n";
    }
}

GameState Game::createGameState() const {
    GameState state;
    state.version = "1.1";
    state.currentLevel = currentLevel_;
    state.currentTurn = currentTurn_;
    state.enemiesKilled = enemiesKilled_;
    state.hasUsedRevive = hasUsedRevive_;
    if (field_) {
        state.field.width = field_->getWidth();
        state.field.height = field_->getHeight();
        state.field.cellTypes = field_->getCellTypesAsInt();
    }
    if (player_) {
        Pos pos = player_->getPosition();
        state.player.x = pos.x;
        state.player.y = pos.y;
        state.player.health = player_->getHealth();
        state.player.maxHealth = player_->getMaxHealth();
        state.player.damage = player_->getDamage();
        state.player.baseMovementSpeed = 1;
        state.player.canMove = player_->canPlayerMove();
        state.player.isSlowed = player_->isPlayerSlowed();
    }
    if (enemyManager_) {
        for (Enemy* enemy : enemyManager_->getEnemies()) {
            if (enemy && enemy->isAlive()) {
                EnemyState es;
                Pos pos = enemy->getPosition();
                es.x = pos.x; es.y = pos.y;
                es.health = enemy->getHealth();
                es.damage = enemy->getDamage();
                state.enemies.push_back(es);
            }
        }
    }
    if (buildingManager_) {
        for (const auto& bi : buildingManager_->getBuildings()) {
            if (bi.building && bi.building->isAlive()) {
                BuildingState bs;
                bs.x = bi.pos.x; bs.y = bi.pos.y;
                bs.health = bi.building->getHealth();
                bs.maxHealth = bi.building->getMaxHealth();
                bs.turnCounter = 0; bs.spawnCooldown = 9;
                state.buildings.push_back(bs);
            }
        }
    }
    if (towerManager_) {
        for (const auto& tower : towerManager_->getTowers()) {
            if (tower && tower->isAlive()) {
                TowerState ts;
                Pos pos = tower->getPosition();
                ts.x = pos.x; ts.y = pos.y;
                ts.health = tower->getHealth();
                ts.maxHealth = tower->getMaxHealth();
                ts.attackRange = tower->getAttackRange();
                ts.attackDamage = tower->getAttackDamage();
                ts.attackCooldown = 3; ts.cooldownCounter = 0;
                state.towers.push_back(ts);
            }
        }
    }
    if (allyManager_) {
        for (const auto& ally : allyManager_->getAllies()) {
            if (ally && ally->isAlive()) {
                AllyState as;
                Pos pos = ally->getPosition();
                as.x = pos.x; as.y = pos.y;
                as.health = ally->getHealth();
                as.damage = ally->getDamage();
                as.shouldSkip = ally->shouldSkipTurn();
                state.allies.push_back(as);
            }
        }
    }
    if (trapManager_) {
        for (const auto& trap : trapManager_->getTraps()) {
            if (trap) {
                TrapState ts;
                Pos pos = trap->getPosition();
                ts.x = pos.x; ts.y = pos.y;
                ts.damage = trap->getDamage();
                state.traps.push_back(ts);
            }
        }
    }
    if (hand_) {
        for (size_t i = 0; i < hand_->getSpellCount(); ++i) {
            const Spell* spell = hand_->getSpell(i);
            if (spell) {
                SpellState ss;
                ss.name = spell->getName();
                ss.type = spell->getType();
                state.handSpells.push_back(ss);
            }
        }
    }
    state.upgrades = appliedUpgrades_;
    return state;
}

void Game::restoreFromGameState(const GameState& state) {
    currentLevel_ = state.currentLevel;
    currentTurn_ = state.currentTurn;
    enemiesKilled_ = state.enemiesKilled;
    hasUsedRevive_ = state.hasUsedRevive;
    appliedUpgrades_ = state.upgrades;
    if (state.field.width > 0 && state.field.height > 0) {
        field_ = std::make_unique<GameField>(state.field.width, state.field.height);
        field_->restoreFromCellTypes(state.field.cellTypes);
    } else {
        Level level = Level::generateLevel(currentLevel_);
        field_ = std::make_unique<GameField>(level.getFieldWidth(), level.getFieldHeight());
    }
    if (player_) {
        player_->setPosition(state.player.x, state.player.y);
        player_->setMaxHealth(state.player.maxHealth);
        player_->setHealth(state.player.health);
        player_->setDamage(state.player.damage);
    }
    enemyManager_ = std::make_unique<EnemyManager>();
    buildingManager_ = std::make_unique<BuildingManager>();
    towerManager_ = std::make_unique<TowerManager>();
    allyManager_ = std::make_unique<AllyManager>();
    trapManager_ = std::make_unique<TrapManager>();
    for (const auto& es : state.enemies) {
        enemyManager_->addEnemy(std::make_unique<Enemy>(es.health, es.damage), es.x, es.y);
    }
    for (const auto& bs : state.buildings) {
        buildingManager_->addBuilding(std::make_unique<EnemyBuilding>(bs.spawnCooldown, bs.health), bs.x, bs.y);
    }
    for (const auto& ts : state.towers) {
        towerManager_->addTower(std::make_unique<EnemyTower>(Pos{ts.x, ts.y}, ts.attackRange, ts.attackDamage, ts.attackCooldown, ts.health));
    }
    for (const auto& as : state.allies) {
        auto ally = std::make_unique<Ally>(as.health, as.damage);
        if (as.shouldSkip) ally->skipNextMove();
        allyManager_->addAlly(std::move(ally), as.x, as.y);
    }
    for (const auto& ts : state.traps) {
        trapManager_->addTrap(std::make_unique<Trap>(Pos{ts.x, ts.y}, ts.damage));
    }
    if (hand_) {
        hand_->clearAllSpells();
        for (const auto& spellState : state.handSpells) {
            auto spell = Hand::createSpell(spellState.type, spellState.name);
            if (auto* trapSpell = dynamic_cast<TrapSpell*>(spell.get())) trapSpell->setTrapManager(trapManager_.get());
            else if (auto* summonSpell = dynamic_cast<SummonSpell*>(spell.get())) summonSpell->setAllyManager(allyManager_.get());
            hand_->addSpell(std::move(spell));
        }
    }
}

void Game::showQuitMenu() {
    std::cout << "\n=== QUIT ===\n1. Save and Exit\n2. Exit without saving\n3. Cancel\nChoose: ";
    int c; if (!(std::cin >> c)) { std::cin.clear(); std::string d; std::getline(std::cin, d); return; }
    if (c == 1) {
        try { saveGame(); saveManager_->deleteAutoSave(); gameRunning_ = false; }
        catch (const SaveLoadException& e) { std::cout << "Save failed: " << e.what() << "\n"; }
    } else if (c == 2) {
        std::cout << "Sure? (Y/N): "; char ch; std::cin >> ch;
        if (ch == 'Y' || ch == 'y') { saveManager_->deleteAutoSave(); gameRunning_ = false; }
    }
}

bool Game::saveGameCommand() {
    try { saveGame(); std::cout << "Saved!\n"; }
    catch (const SaveLoadException& e) { std::cout << "Save failed: " << e.what() << "\n"; }
    return false;
}

void Game::saveGame() {
    GameState state = createGameState();
    saveManager_->saveGame(state);
    if (eventBus_) eventBus_->publish({GameEventType::SAVE, "Game saved"});
}

void Game::autoSaveGame() {
    try {
        GameState state = createGameState();
        saveManager_->autoSave(state);
        if (eventBus_) eventBus_->publish({GameEventType::SAVE, "Auto-saved"});
    } catch (const SaveLoadException&) {}
}

bool Game::loadGame() {
    try {
        if (!saveManager_->hasSaveFile()) { std::cout << "No save!\n"; return false; }
        GameState state = saveManager_->loadGame();
        restoreFromGameState(state);
        gameRunning_ = true;
        saveManager_->deleteSaveFile();
        std::cout << "Loaded!\n";
        return true;
    } catch (const SaveLoadException& e) { std::cout << "Load failed: " << e.what() << "\n"; return false; }
}

bool Game::loadAutoSave() {
    try {
        if (!saveManager_->hasAutoSave()) return false;
        GameState state = saveManager_->loadAutoSave();
        restoreFromGameState(state);
        return true;
    } catch (const SaveLoadException&) { return false; }
}

Game::DeathChoice Game::showDeathMenu() {
    std::cout << "\n=== YOU DIED! ===\n1. New game\n";
    std::cout << ((!hasUsedRevive_ && saveManager_->hasAutoSave()) ? "2. Restore level\n" : "2. (unavailable)\n");
    std::cout << (saveManager_->hasSaveFile() ? "3. Load save\n" : "3. (no save)\n");
    std::cout << "4. Exit\nChoose: ";
    int c; if (!(std::cin >> c)) { std::cin.clear(); std::string d; std::getline(std::cin, d); c = 4; }
    if (c == 1) return DeathChoice::NEW_GAME;
    if (c == 2 && !hasUsedRevive_ && saveManager_->hasAutoSave()) return DeathChoice::RESTORE_LEVEL;
    if (c == 3 && saveManager_->hasSaveFile()) return DeathChoice::LOAD_SAVE;
    return DeathChoice::EXIT;
}

void Game::handlePlayerDeath() {
    DeathChoice choice = showDeathMenu();
    if (choice == DeathChoice::NEW_GAME) {
        saveManager_->deleteAllSaves();
        player_ = std::make_unique<Player>(100, 20);
        initializeGame();
        gameRunning_ = true;
    } else if (choice == DeathChoice::RESTORE_LEVEL) {
        if (loadAutoSave()) { hasUsedRevive_ = true; saveManager_->deleteAutoSave(); gameRunning_ = true; }
        else gameRunning_ = false;
    } else if (choice == DeathChoice::LOAD_SAVE) {
        if (loadGame()) gameRunning_ = true;
        else gameRunning_ = false;
    } else {
        saveManager_->deleteAllSaves();
        gameRunning_ = false;
    }
}
