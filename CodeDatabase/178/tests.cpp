#include <iostream>
#include <cassert>
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Ally.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "TrapSpell.h"
#include "AreaDamageSpell.h"
#include "DirectDamageSpell.h"
#include "SummonSpell.h"
#include "UpgradeSpell.h"
#include "Hand.h"
#include "GameCell.h"
#include "GameField.h"
#include "GameController.h"
#include "MovementSystem.h"
#include "CombatSystem.h"
#include "LevelManager.h"
#include "GameConfig.h"
#include "SpellSystem.h"
#include "Position.h"

using namespace std;

void testPosition() {
    cout << "Testing Position..." << endl;
    Position p1(2, 3), p2(2, 3), p3(1, 5);
    assert(p1 == p2);
    assert(p1 != p3);
    assert(p1.x == 2 && p1.y == 3);
    cout << "Position tests passed!" << endl;
}

void testCharacter() {
    cout << "Testing Character..." << endl;
    Player player(Position(1, 1), 100, 10, 8);
    Enemy enemy(Position(2, 2), 80, 12);
    assert(player.getHealth() == 100);
    assert(player.getDamage() == 10); 
    assert(player.isAlive());
    
    assert(enemy.getHealth() == 80);
    assert(enemy.getDamage() == 12);
    assert(enemy.isAlive());
    player.takeDamage(50);
    assert(player.getHealth() == 50);
    assert(player.isAlive());
    enemy.takeDamage(80);
    assert(enemy.getHealth() == 0);
    assert(!enemy.isAlive());
    assert(player.getPosition() == Position(1, 1));
    player.setPosition(Position(3, 3));
    assert(player.getPosition() == Position(3, 3));
    
    cout << "Character tests passed!" << endl;
}

void testPlayer() {
    cout << "Testing Player..." << endl;
    Player player(Position(0, 0), 100, 15, 10);
    assert(player.getHealth() == 100);
    assert(player.getMeleeDamage() == 15);
    assert(player.getRangedDamage() == 10);
    assert(player.getCombatType() == CombatType::MELEE);
    assert(player.getDamage() == 15);
    
    player.takeDamage(20);
    assert(player.getHealth() == 80);
    player.addCoins(50);
    assert(player.getCoins() == 50);
    assert(player.trySpendCoins(20) == true);
    assert(player.getCoins() == 30);
    assert(player.trySpendCoins(40) == false);
    assert(player.getCoins() == 30);
    
    player.switchCombatType();
    assert(player.getCombatType() == CombatType::RANGED);
    assert(player.getDamage() == 10); 
    
    player.move(Position(1, 0));
    assert(player.getPosition().x == 1);
    assert(player.getPosition().y == 0);
    
    player.upgradeHealth(20);
    assert(player.getMaxHealth() == 120);
    assert(player.getHealth() == 100);
    
    player.upgradeMeleeDamage(5);
    assert(player.getMeleeDamage() == 20);
    
    cout << "Player tests passed!" << endl;
}

void testEnemy() {
    cout << "Testing Enemy..." << endl;
    Enemy enemy(Position(2, 0), 40, 10);
    assert(enemy.isAlive());
    assert(enemy.getDamage() == 10);
    
    enemy.takeDamage(40);
    assert(!enemy.isAlive());
    
    Enemy enemy2(Position(3, 3), 50, 15);
    enemy2.move(Position(3, 4));
    assert(enemy2.getPosition().y == 4);
    cout << "Enemy tests passed!" << endl;
}

void testAlly() {
    cout << "Testing Ally..." << endl;
    Ally ally(Position(0, 1), 30, 5);
    ally.move(Position(1, 1));
    assert(ally.getPosition().x == 1);
    assert(ally.getPosition().y == 1);
    
    vector<Enemy> enemies;
    enemies.emplace_back(Position(2, 2), 20, 5);
    enemies.emplace_back(Position(1, 1), 15, 3);
    
    ally.act(enemies);
    cout << "Ally tests passed!" << endl;
}

void testEnemyBuilding() {
    cout << "Testing EnemyBuilding..." << endl;
    EnemyBuilding building(Position(3, 3), 2);
    assert(building.getPosition() == Position(3, 3));
    assert(!building.shouldSpawnEnemy());
    
    building.update();
    assert(!building.shouldSpawnEnemy());
    
    building.update();
    assert(building.shouldSpawnEnemy());
    
    Enemy newEnemy = building.spawnEnemy();
    assert(newEnemy.isAlive());
    assert(newEnemy.getPosition().x == 3);
    assert(newEnemy.getPosition().y == 4);
    
    building.update();
    assert(!building.shouldSpawnEnemy());
    cout << "EnemyBuilding tests passed!" << endl;
}

void testEnemyTower() {
    cout << "Testing EnemyTower..." << endl;
    EnemyTower tower(Position(5, 5), 50, 10, 3);
    assert(tower.getHealth() == 50);
    assert(tower.getDamage() == 10);
    assert(tower.getRange() == 3);
    assert(tower.isAlive());
    assert(tower.canFire());
    
    tower.takeDamage(10);
    assert(tower.getHealth() == 40);
    assert(tower.isAlive());
    
    tower.setCooldown(2);
    assert(!tower.canFire());
    
    tower.tickCooldown();
    tower.tickCooldown();
    assert(tower.canFire());
    
    tower.takeDamage(50);
    assert(!tower.isAlive());
    cout << "EnemyTower tests passed!" << endl;
}

void testTrap() {
    cout << "Testing Trap..." << endl;
    Trap trap(Position(2, 2), 7);
    assert(trap.getDamage() == 7);
    assert(trap.getPosition() == Position(2, 2));
    
    Trap defaultTrap;
    assert(defaultTrap.getDamage() == 5);
    cout << "Trap tests passed!" << endl;
}

void testGameCell() {
    cout << "Testing GameCell..." << endl;
    GameCell emptyCell(CellType::EMPTY);
    assert(emptyCell.isPassable());
    assert(!emptyCell.getIsOccupied());
    
    GameCell obstacleCell(CellType::OBSTACLE);
    assert(!obstacleCell.isPassable());
    
    GameCell slowingCell(CellType::SLOWING);
    assert(slowingCell.isPassable());
    
    emptyCell.setOccupied(true);
    assert(emptyCell.getIsOccupied());
    assert(!emptyCell.isPassable());
    cout << "GameCell tests passed!" << endl;
}

void testGameField() {
    cout << "Testing GameField..." << endl;
    GameField field(10, 10);
    assert(field.getWidth() == 10);
    assert(field.getHeight() == 10);
    
    Position validPos(3, 4);
    Position invalidPos(15, 15);
    
    assert(field.isValidPosition(validPos));
    assert(!field.isValidPosition(invalidPos));
    assert(field.isPositionPassable(validPos));
    
    field.setCellOccupied(validPos, true);
    assert(!field.isPositionPassable(validPos));
    
    const GameCell& cell = field.getCell(validPos);
    assert(cell.getIsOccupied());
    cout << "GameField tests passed!" << endl;
}

void testSpells() {
    cout << "Testing Spells..." << endl;
    DirectDamageSpell directSpell(20, 3);
    assert(string(directSpell.name()) == "Direct Damage");
    AreaDamageSpell areaSpell(15, 2, 2, 4);
    assert(string(areaSpell.name()) == "Area Damage");
    TrapSpell trapSpell(10);
    assert(string(trapSpell.name()) == "Trap");
    SummonSpell summonSpell(2);
    assert(string(summonSpell.name()) == "Summon");
    UpgradeSpell upgradeSpell(2);
    assert(string(upgradeSpell.name()) == "Upgrade");
    assert(upgradeSpell.getPower() == 2);
    
    cout << "Spells tests passed!" << endl;
}

void testHand() {
    cout << "Testing Hand..." << endl;
    Hand hand(3); // Max size 3
    assert(hand.size() == 0);
    hand.addSpell(std::make_unique<DirectDamageSpell>());
    hand.addSpell(std::make_unique<AreaDamageSpell>());
    assert(hand.size() == 2);
    hand.addSpell(std::make_unique<SummonSpell>());
    assert(hand.size() == 3);
    const ISpell* spell = hand.getAt(0);
    assert(spell != nullptr);
    assert(string(spell->name()) == "Direct Damage");
    
    auto extractedSpell = hand.extractAt(1);
    assert(extractedSpell != nullptr);
    assert(hand.size() == 2);
    
    hand.list();
    cout << "Hand tests passed!" << endl;
}

void testMovementSystem() {
    cout << "Testing MovementSystem..." << endl;
    GameField field(10, 10);
    MovementSystem movement(field);
    
    Position start(5, 5);
    Position direction(1, 0);
    
    Position newPos = movement.calculateNewPosition(start, direction);
    assert(newPos == Position(6, 5));
    
    assert(movement.isValidMove(Position(0, 0)));
    assert(movement.isValidMove(Position(9, 9)));
    assert(!movement.isValidMove(Position(-1, 0)));
    assert(!movement.isValidMove(Position(10, 10)));
    
    Player player(Position(0, 0), 100, 10, 8);
    movement.movePlayer(player, Position(1, 1));
    assert(player.getPosition() == Position(1, 1));
    
    cout << "MovementSystem tests passed!" << endl;
}

void testCombatSystem() {
    cout << "Testing CombatSystem..." << endl;
    
    Player player(Position(0, 0), 100, 20, 15);
    Enemy enemy(Position(1, 0), 50, 10);
    
    AttackResult result = CombatSystem::performPlayerAttack(player, enemy);
    assert(result == AttackResult::SUCCESS);
    assert(enemy.getHealth() == 30);
    
    Enemy farEnemy(Position(5, 5), 50, 10);
    AttackResult farResult = CombatSystem::performPlayerAttack(player, farEnemy);
    assert(farResult == AttackResult::OUT_OF_RANGE);
    
    assert(CombatSystem::isInRange(Position(0, 0), Position(1, 0), 2));
    assert(!CombatSystem::isInRange(Position(0, 0), Position(3, 0), 2));
    
    cout << "CombatSystem tests passed!" << endl;
}

void testGameConfig() {
    cout << "Testing GameConfig..." << endl;
    
    LevelConfig level1 = GameConfig::getLevelConfig(1);
    assert(level1.levelNumber == 1);
    assert(level1.enemyHealth == 20);
    assert(level1.towerHealth == 50);
    assert(level1.buildingSpawnInterval == 5);
    
    LevelConfig level2 = GameConfig::getLevelConfig(2);
    assert(level2.levelNumber == 2);
    assert(level2.enemyHealth == 30);
    assert(level2.towerHealth == 75);
    assert(level2.buildingSpawnInterval == 4);
    
    LevelConfig level3 = GameConfig::getLevelConfig(3);
    assert(level3.levelNumber == 3);
    assert(level3.enemyHealth == 40);
    assert(level3.towerHealth == 100);
    assert(level3.buildingSpawnInterval == 3);
    
    assert(GameConfig::getMaxLevels() == 3);
    assert(GameConfig::getUpgradeCost() == 50);
    
    cout << "GameConfig tests passed!" << endl;
}

void testGameController() {
    cout << "Testing GameController..." << endl;
    GameController game(15, 15);
    
    assert(game.isGameRunning());
    assert(game.getPlayer().isAlive());
    
    game.processPlayerMove(Position(1, 0));
    game.processPlayerAttack(Position(1, 0));
    
    game.addTower(EnemyTower(Position(5, 5), 50, 10, 3));
    game.addTrap(Trap(Position(3, 3), 8));
    game.addAlly(Ally(Position(2, 2), 30, 5));
    
    assert(game.getEnemies().size() >= 0);
    assert(game.getTowers().size() >= 1); 
    assert(game.getTraps().size() >= 1);   
    assert(game.getAllies().size() >= 1);  
    
    assert(game.getField().isValidPosition(Position(3, 3)));
    
    game.removeDeadEnemies();
    
    cout << "GameController tests passed!" << endl;
}

void testLevelManager() {
    cout << "Testing LevelManager..." << endl;
    LevelManager levelManager;
    
    assert(levelManager.getCurrentLevel() == 1);
    assert(!levelManager.isLevelCompleted());
    assert(!levelManager.isGameCompleted());
    
    levelManager.setCurrentLevel(2);
    assert(levelManager.getCurrentLevel() == 2);
    
    GameController game(15, 15);
    levelManager.initializeLevel(game);
    
    cout << "LevelManager tests passed!" << endl;
}

int main() {
    cout << "      TESTS STARTED     " << endl;
    
    try {
        testPosition();
        testCharacter();
        testPlayer();
        testEnemy();
        testAlly();
        testEnemyBuilding();
        testEnemyTower();
        testTrap();
        testGameCell();
        testGameField();
        testSpells();
        testHand();
        testMovementSystem();
        testCombatSystem();
        testGameConfig();
        testGameController();
        testLevelManager();
        
        cout << "       ALL TESTS PASSED SUCCESSFULLY      " << endl;
    } catch (const exception& e) {
        cerr << "Test failed with exception: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Test failed with unknown exception!" << endl;
        return 1;
    }
    
    return 0;
}