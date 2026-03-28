#include "Game.h"
#include "Constants.h"
#include "GameException.h"
#include "LevelUpSystem.h"
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>
#include <climits>

using namespace std;

Game::Game(int fieldSize)
    : field(fieldSize),
      player(fieldSize / 2, fieldSize / 2),
      gameRunning(true),
      playerWon(false),
      killCount(0),
      currentLevel(1),
      totalLevels(TOTAL_LEVELS) {
    levelUpSystem = make_unique<LevelUpSystem>(player);
    createInitialUnits();
    updateGameState();
}



void Game::updateGameState() {
    field.updateField(player, enemies, buildings, allies);
}

void Game::createInitialUnits() {    
    createEnemies(INITIAL_ENEMIES);
    createBarracks(INITIAL_BARRACKS);
    createTowers(INITIAL_TOWERS);
}

void Game::createEnemies(int count) {
    mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0, field.getSize() - 1);

    // Увеличиваем количество и силу врагов с уровнем
    int adjustedCount = count * currentLevel;
    int enemyHealth = DEFAULT_ENEMY_HEALTH + (currentLevel - 1) * 5;
    int enemyDamage = DEFAULT_ENEMY_DAMAGE + (currentLevel - 1) * 3;

    for (int i = 0; i < adjustedCount; i++) {
        int x, y;
        do {
            x = distribution(generator);
            y = distribution(generator);
        } while (!isValidSpawnPosition(x, y));

        auto enemy = make_shared<Enemy>(x, y);
        enemy->setHealth(enemyHealth);
        enemy->setDamage(enemyDamage);
        enemies.push_back(enemy);
    }
}

void Game::createBarracks(int count) {
    mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0, field.getSize() - 1);

    // Увеличиваем количество и прочность бараков с уровнем
    int adjustedCount = count + currentLevel - 1;
    int barrackHealth = DEFAULT_BARRACK_HEALTH + (currentLevel - 1) * 10;

    for (int i = 0; i < adjustedCount; i++) {
        int x, y;
        do {
            x = distribution(generator);
            y = distribution(generator);
        } while (!isValidSpawnPosition(x, y));

        auto barrack = make_shared<EnemyBarrack>(x, y, DEFAULT_SPAWN_INTERVAL);
        barrack->setHealth(barrackHealth);
        barracks.push_back(barrack);
        buildings.push_back(barrack);
    }
}

void Game::createTowers(int count) {
    mt19937 generator(chrono::system_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> distribution(0, field.getSize() - 1);

    // Увеличиваем количество и силу башен с уровнем
    int adjustedCount = count + currentLevel / 3;
    int towerHealth = DEFAULT_TOWER_HEALTH + (currentLevel - 1) * 10;
    int towerDamage = DEFAULT_TOWER_DAMAGE + (currentLevel - 1) * 5;

    for (int i = 0; i < adjustedCount; i++) {
        int x, y;
        do {
            x = distribution(generator);
            y = distribution(generator);
        } while (!isValidSpawnPosition(x, y));

        auto tower = make_shared<EnemyTower>(x, y, DEFAULT_TOWER_ATTACK_RANGE, 
                                           towerDamage, DEFAULT_TOWER_ATTACK_COOLDOWN);
        tower->setHealth(towerHealth);
        towers.push_back(tower);
        buildings.push_back(tower);
    }
}

bool Game::isValidSpawnPosition(int x, int y) const {
    if (!field.isValidPosition(x, y) || field.isObstacleAt(x, y)) {
        return false;
    }

    if (x == player.getPosition().getX() && y == player.getPosition().getY()) {
        return false;
    }

    for (const auto& building : buildings) { 
        if (building && building->getPosition() == Position(x, y)) {
            return false;
        }
    }

    for (const auto& enemy : enemies) {
        if (enemy && enemy->getPosition() == Position(x, y)) {
            return false;
        }
    }

    for (const auto& ally : allies) {
        if (ally && ally->getPosition() == Position(x, y)) {
            return false;
        }
    }

    Position playerPos = player.getPosition();
    int distance = abs(x - playerPos.getX()) + abs(y - playerPos.getY());
    return distance >= MIN_SPAWN_DISTANCE;
}

// Методы для GameController
void Game::movePlayer(Direction direction) {
    if (!gameRunning || !player.isAlive()) return;
    
    bool pauseNeeded = player.move(direction, field.getSize(), field.getSize(), 
                                   enemies, buildings, field);
    
}

void Game::performAttack() {
    if (!gameRunning || !player.isAlive()) return;

    combatManager.performPlayerAttack(player, enemies, buildings);    
    for (const auto& enemy : enemies) {
        if (enemy && !enemy->isAlive()) {
            killCount++;
        }
    }
    
    static int lastProcessedKillCount = 0;
    if (killCount > lastProcessedKillCount && killCount % KILL_COUNT_FOR_SPELL == 0) {
        onEnemyKilled();
        lastProcessedKillCount = killCount;
    }
}

void Game::changeFightingStyle() {
    if (!gameRunning || !player.isAlive()) return;
    
    if (player.getFightingStyle() == 'm') {
        player.setFightingStyle('r');
    } else {
        player.setFightingStyle('m');
    }
}

bool Game::processPlayerSpell() {
    auto& hand = player.getHand();
    const auto& spells = hand.getSpells();

    if (spells.empty()) {
        std::cout << "No spells in hand.\n";
        return true;
    }

    std::cout << "Available spells:\n";
    for (size_t i = 0; i < spells.size(); ++i) {
        std::cout << i << ": " << spells[i]->getName() << "\n";
    }

    int index;
    std::cout << "Choose spell index: ";
    std::cin >> index;

    if (index < 0 || index >= static_cast<int>(spells.size())) {
        std::cout << "Invalid spell index.\n";
        return true;
    }

    const auto& chosenSpell = spells[index];
    bool needsTarget = (chosenSpell->getName() == "Lightning Bolt" ||  
                        chosenSpell->getName() == "Fireball" ||        
                        chosenSpell->getName() == "Set Trap");         

    Position target(0, 0);
    if (needsTarget) {
        int x, y;
        std::cout << "Enter target position (x y): ";
        std::cin >> x >> y;
        target = Position(x, y);
    }

    bool result = hand.useSpell(index, field, target, player, *this);
        
    return result;
}


void Game::updateEnemies() {
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            // Проверяем атаку на игрока
            if (enemy->canAttackTarget(player.getPosition())) {
                player.takeDamage(enemy->getDamage());
                continue;
            }
            
            // Проверяем атаку на союзников
            std::shared_ptr<AllyUnit> allyToAttack = nullptr;
            for (auto& ally : allies) {
                if (ally && ally->isAlive() && enemy->canAttackTarget(ally->getPosition())) {
                    allyToAttack = ally;
                    break;
                }
            }
            
            if (allyToAttack) {
                allyToAttack->takeDamage(enemy->getDamage());
                continue;
            }
            
            enemy->moveAI(player.getPosition(), allies, enemies, buildings, field);
        }
    }
    
    // Подсчет убитых врагов
    for (auto& enemy : enemies) {
        if (enemy && !enemy->isAlive()) {
            killCount++;
        }
    }
}

void Game::updateBarracks() {
    for (auto& barrack : barracks) {
        if (barrack && barrack->isAlive() && barrack->canSpawnEnemy()) {
            Position spawnPos = barrack->getSpawnPosition(field);
            if (spawnPos.getX() != -1) {
                enemies.push_back(make_shared<Enemy>(spawnPos.getX(), spawnPos.getY()));
            }
        }
    }
}

void Game::updateTowers() {
    for (auto& tower : towers) {
        if (tower && tower->isAlive()) {
            tower->update();
            tower->tryAttack(player, field);
        }
    }
}

void Game::updateAllies() {
    for (auto& ally : allies) {
        if (ally && ally->isAlive()) {
            ally->moveAI(enemies, buildings, field);
        }
    }
}

void Game::removeDeadUnits() {
    vector<shared_ptr<Enemy>> aliveEnemies;
    for (auto& enemy : enemies) {
        if (enemy && enemy->isAlive()) {
            aliveEnemies.push_back(enemy);
        }
    }
    enemies = aliveEnemies;

    vector<shared_ptr<EnemyBarrack>> aliveBarracks;
    for (auto& barrack : barracks) {
        if (barrack && barrack->isAlive()) {
            aliveBarracks.push_back(barrack);
        } else {
            // Удаляем из общего списка зданий
            auto it = remove_if(buildings.begin(), buildings.end(),
                [&barrack](const shared_ptr<EnemyBuilding>& b) {
                    return b.get() == barrack.get();
                });
            buildings.erase(it, buildings.end());
        }
    }
    barracks = aliveBarracks;

    vector<shared_ptr<EnemyTower>> aliveTowers;
    for (auto& tower : towers) {
        if (tower && tower->isAlive()) {
            aliveTowers.push_back(tower);
        } else {
            auto it = remove_if(buildings.begin(), buildings.end(),
                [&tower](const shared_ptr<EnemyBuilding>& b) {
                    return b.get() == tower.get();
                });
            buildings.erase(it, buildings.end());
        }
    }
    towers = aliveTowers;

    vector<shared_ptr<AllyUnit>> aliveAllies;
    for (auto& ally : allies) {
        if (ally && ally->isAlive()) {
            aliveAllies.push_back(ally);
        }
    }
    allies = aliveAllies;

    vector<shared_ptr<EnemyBuilding>> aliveBuildings;
    for (auto& building : buildings) {
        if (building && building->isAlive()) {
            aliveBuildings.push_back(building);
        }
    }
    buildings = aliveBuildings;
}

void Game::addAlly(unique_ptr<AllyUnit> ally) {
    allies.push_back(move(ally));
}

void Game::onEnemyKilled() {
    auto newSpell = PlayerSpellHand::generateRandomSpell();
    string spellName = newSpell->getName();
    if (player.getHand().addSpell(move(newSpell))) {
    }
}

bool Game::checkGameEndCondition() const {
    if (!player.isAlive()) {
        return true;
    }
    
    if (playerWon) {
        return true;
    }
    
    if (!gameRunning) {
        return true;
    }
    
    return false;
}

void Game::advanceToNextLevel() {
    if (currentLevel >= totalLevels) {
        playerWon = true;
        gameRunning = false;
        return;
    }

    levelUpSystem->performLevelUp(currentLevel);
    
    currentLevel++;
    
    int newFieldSize = field.getSize() + (currentLevel - 1) * 2;
    field = GameField(newFieldSize);
    player.setPosition(newFieldSize / 2, newFieldSize / 2);
    player.setHealth(player.getMaxHealthPoints());
    player.getHand().removeHalfOfSpells();
    
    resetGame();    
    createInitialUnits();
    
    updateGameState();
}

void Game::resetGame() {
    enemies.clear();
    barracks.clear();
    towers.clear();
    buildings.clear();
    allies.clear();
    killCount = 0;
}

GameState Game::captureGameState() const {
    GameState state;
    
    // Данные игрока
    GameState::PlayerData playerData;
    playerData.health = player.getHealthPoints();
    playerData.damage = player.getDamage() - player.getDamageBonus();
    playerData.position = player.getPosition();
    playerData.score = player.getScorePoints();
    playerData.fightingStyle = player.getFightingStyle();
    playerData.attackRange = player.getAttackRange();
    playerData.skipNextMove = player.shouldSkipMove();
    playerData.damageBonus = player.getDamageBonus();
    
    // Заклинания
    const auto& spells = player.getHand().getSpells();
    playerData.spellNames.clear();
    for (const auto& spell : spells) {
        playerData.spellNames.push_back(spell->getName());
    }
    playerData.buffCount = player.getHand().getBuffCount();
    
    state.setPlayerData(playerData);
    
    vector<GameState::UnitData> enemiesData;
    for (const auto& enemy : getEnemies()) {
        if (enemy && enemy->isAlive()) {
            GameState::UnitData data;
            data.type = 0; // Enemy
            data.health = enemy->getHealthPoints();
            data.damage = enemy->getDamage();
            data.position = enemy->getPosition();
            data.alive = true;
            enemiesData.push_back(data);
        }
    }
    state.setEnemiesData(enemiesData);
    
    vector<GameState::UnitData> buildingsData;
    for (const auto& building : getBuildings()) {
        if (building && building->isAlive()) {
            GameState::UnitData data;
            
            CellType type = building->getCellType();
            if (type == CellType::ENEMY_BARRACK) {
                data.type = 2; // Barrack
            } else if (type == CellType::ENEMY_TOWER) {
                data.type = 3; // Tower
            } else {
                data.type = 1;
            }
            
            data.health = building->getHealthPoints();
            data.damage = building->getDamage();
            data.position = building->getPosition();
            data.alive = true;
            buildingsData.push_back(data);
        }
    }
    state.setBuildingsData(buildingsData);
    
    // Союзники
    vector<GameState::UnitData> alliesData;
    for (const auto& ally : getAllies()) {
        if (ally && ally->isAlive()) {
            GameState::UnitData data;
            data.type = 1; // Ally
            data.health = ally->getHealthPoints();
            data.damage = ally->getDamage();
            data.position = ally->getPosition();
            data.alive = true;
            alliesData.push_back(data);
        }
    }
    state.setAlliesData(alliesData);
    
    // Поле
    state.setFieldSize(field.getSize());
    vector<GameState::FieldCellData> fieldData;
    for (int y = 0; y < field.getSize(); y++) {
        for (int x = 0; x < field.getSize(); x++) {
            GameState::FieldCellData cell;
            cell.x = x;
            cell.y = y;
            cell.cellType = field.getCellType(x, y);
            fieldData.push_back(cell);
        }
    }
    state.setFieldData(fieldData);
    
    state.setKillCount(killCount);
    state.setCurrentLevel(currentLevel);
    
    return state;
}

void Game::applyGameState(const GameState& state) {
    resetGame();
    
    currentLevel = state.getCurrentLevel();
    
    // Восстанавливаем поле
    field = GameField(state.getFieldSize());
    for (const auto& cell : state.getFieldData()) {
        field.setCellType(cell.x, cell.y, cell.cellType);
    }
    
    const auto& playerData = state.getPlayerData();
    
    // Восстанавливаем игрока
    player = Player(playerData.position.getX(), playerData.position.getY());
    player.setHealth(playerData.health);
    player.setDamage(playerData.damage); 
    player.setDamageBonus(playerData.damageBonus);
    player.setFightingStyle(playerData.fightingStyle);
    player.addScorePoints(playerData.score);
    player.setSkipMove(playerData.skipNextMove);
    
    // Восстанавливаем заклинания
    auto& hand = player.getHand();
    hand.clear();
    
    for (const auto& spellName : playerData.spellNames) {
        auto spell = PlayerSpellHand::createSpellByName(spellName);
        if (spell) {
            hand.addSpell(move(spell));
        }
    }
    
    for (int i = 0; i < playerData.buffCount; i++) {
        hand.incrementBuffCount();
    }
    
    // Восстанавливаем врагов
    for (const auto& enemyData : state.getEnemiesData()) {
        if (enemyData.alive) {
            createEnemyFromData(enemyData);
        }
    }
    
    // Восстанавливаем здания
    for (const auto& buildingData : state.getBuildingsData()) {
        if (buildingData.alive) {
            createBuildingFromData(buildingData);
        }
    }
    
    // Восстанавливаем союзников
    for (const auto& allyData : state.getAlliesData()) {
        if (allyData.alive) {
            createAllyFromData(allyData);
        }
    }
    
    killCount = state.getKillCount();
}

void Game::createEnemyFromData(const GameState::UnitData& data) {
    auto enemy = make_shared<Enemy>(data.position.getX(), data.position.getY());
    enemy->setHealth(data.health);
    enemy->setDamage(data.damage);
    enemies.push_back(enemy);
}

void Game::createBuildingFromData(const GameState::UnitData& data) {
    if (data.type == 2) { // Barrack
        auto barrack = make_shared<EnemyBarrack>(
            data.position.getX(), 
            data.position.getY(), 
            DEFAULT_SPAWN_INTERVAL
        );
        barrack->setHealth(data.health);
        barrack->setDamage(data.damage);
        barracks.push_back(barrack);
        buildings.push_back(barrack);
    } else if (data.type == 3) { // Tower
        auto tower = make_shared<EnemyTower>(
            data.position.getX(),
            data.position.getY(),
            DEFAULT_TOWER_ATTACK_RANGE,
            data.damage,
            DEFAULT_TOWER_ATTACK_COOLDOWN
        );
        tower->setHealth(data.health);
        towers.push_back(tower);
        buildings.push_back(tower);
    }
}

void Game::createAllyFromData(const GameState::UnitData& data) {
    auto ally = make_shared<AllyUnit>(data.position.getX(), data.position.getY());
    ally->setHealth(data.health);
    ally->setDamage(data.damage);
    allies.push_back(ally);
}

void Game::saveGame(const std::string& filename) {
    try {
        std::string finalFilename = filename;
        if (finalFilename.length() < 6 || 
            finalFilename.substr(finalFilename.length() - 5) != ".save") {
            finalFilename += ".save";
        }
        
        captureGameState().saveToFile(finalFilename);
    }
    catch (const SaveGameException& e) {
        throw;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Unexpected error during save: " + std::string(e.what()));
    }
}

void Game::loadGame(const std::string& filename) {
    try {
        GameState state;
        state.loadFromFile(filename);
        applyGameState(state);
    }
    catch (const LoadGameException& e) {
        throw;
    }
    catch (const std::exception& e) {
        throw std::runtime_error("Unexpected error during load: " + std::string(e.what()));
    }
}