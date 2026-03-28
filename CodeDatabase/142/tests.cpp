#include <iostream>
#include <cassert>
#include <memory>
#include <cstdlib>
#include <windows.h>
#include <fstream>
#include <string>
#include <unordered_map>
#include <optional>

#include "Cell.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Field.h"
#include "Hand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "Game.h"
#include "GameState.h"

namespace {
    void clearField(Field& field) {
        for (int y = 0; y < field.getHeight(); ++y) {
            for (int x = 0; x < field.getWidth(); ++x) {
                field.clearCell(x, y);
            }
        }
    }

    void writeControlsFile(const std::string& content) {
        std::ofstream out("controls.cfg", std::ios::trunc);
        out << content;
    }

    void writeFile(const std::string& path, const std::string& content) {
        std::ofstream out(path, std::ios::trunc | std::ios::binary);
        out << content;
    }

    std::unordered_map<std::string, char> defaultControls() {
        return {
            {"MOVE_UP", 'w'},
            {"MOVE_DOWN", 's'},
            {"MOVE_LEFT", 'a'},
            {"MOVE_RIGHT", 'd'},
            {"SWITCH_MODE", 'm'},
            {"CAST_SPELL", 'c'},
            {"SAVE_GAME", 'v'},
            {"QUIT", 'q'}
        };
    }
}

// ===== Testing Cell =====
void testCell() {
    std::cout << "Testing Cell..." << std::endl;
    
    Cell cell;
    
    // Check initial state
    assert(cell.isEmpty() == true);
    assert(cell.isWall() == false);
    
    // Check type setting
    cell.setType(CellType::Wall);
    assert(cell.isWall() == true);
    assert(cell.isEmpty() == false);
    
    cell.setType(CellType::Player);
    assert(cell.getType() == CellType::Player);
    assert(cell.isOccupied() == true);
    
    std::cout << "Cell tests passed!" << std::endl;
}

// ===== Testing Entity =====
void testEntity() {
    std::cout << "Testing Entity..." << std::endl;
    
    // Create test class for abstract Entity
    class TestEntity : public Entity {
    public:
        TestEntity(int x, int y, int hp) : Entity(x, y, hp) {}
        int getDamage() const override { return 10; }
        char getDisplayChar() const override { return 'T'; }
    };
    
    TestEntity entity(5, 5, 100);
    
    // Check position and health
    assert(entity.getX() == 5);
    assert(entity.getY() == 5);
    assert(entity.getHP() == 100);
    assert(entity.isAlive() == true);
    
    // Check damage taking
    entity.takeDamage(30);
    assert(entity.getHP() == 70);
    
    // Check death
    entity.takeDamage(100);
    assert(entity.getHP() == 0);
    assert(entity.isAlive() == false);
    
    std::cout << "Entity tests passed!" << std::endl;
}

// ===== Testing Player =====
void testPlayer() {
    std::cout << "Testing Player..." << std::endl;
    
    Player player(2, 2, 100, 15, 5);
    
    // Check initial state
    assert(player.getHP() == 100);
    assert(player.getCombatMode() == CombatMode::Melee);
    assert(player.getDamage() == 15); // Melee mode
    
    // Check combat mode switching
    player.switchCombatMode();
    assert(player.getCombatMode() == CombatMode::Ranged);
    assert(player.getDamage() == 5); // Ranged mode
    
    // Check score system
    assert(player.getScore() == 0);
    player.addScore(10);
    assert(player.getScore() == 10);
    
    // Check display character
    assert(player.getDisplayChar() == 'P');
    
    std::cout << "Player tests passed!" << std::endl;
}

// ===== Testing Hand =====
void testHand() {
    std::cout << "Testing Hand..." << std::endl;

    std::srand(0); // deterministic first spell

    Hand hand(3);
    assert(hand.getMaxSize() == 3);
    assert(hand.getSpellCount() == 1);
    assert(!hand.isEmpty());
    assert(!hand.isFull());
    assert(hand.getSpell(0) != nullptr);

    bool added = hand.addSpell(std::make_unique<DirectDamageSpell>(3, 25));
    assert(added);
    assert(hand.getSpellCount() == 2);
    assert(hand.getSpell(1) != nullptr);

    added = hand.addSpell(std::make_unique<AreaDamageSpell>(3, 15));
    assert(added);
    assert(hand.getSpellCount() == 3);
    assert(hand.isFull());

    added = hand.addSpell(std::make_unique<DirectDamageSpell>(3, 10));
    assert(added == false);

    Hand copy = hand;
    assert(copy.getSpellCount() == hand.getSpellCount());
    assert(copy.isFull() == hand.isFull());
    assert(copy.getSpell(0) != nullptr);

    Hand assigned(2);
    assigned = hand;
    assert(assigned.getSpellCount() == hand.getSpellCount());
    assert(assigned.isFull() == hand.isFull());

    std::cout << "Hand tests passed!" << std::endl;
}

void testEnemy() {
    std::cout << "Testing Enemy..." << std::endl;
    
    Enemy enemy(3, 3, 50, 8);
    
    assert(enemy.getHP() == 50);
    assert(enemy.getDamage() == 8);
    assert(enemy.isAlive() == true);
    
    enemy.takeDamage(20);
    assert(enemy.getHP() == 30);
    assert(enemy.isAlive() == true);
    
    assert(enemy.getDisplayChar() == 'E');
    
    std::cout << "Enemy tests passed!" << std::endl;
}

void testField() {
    std::cout << "Testing Field..." << std::endl;
    
    Field field(10, 10);
    clearField(field);
    
    assert(field.getWidth() == 10);
    assert(field.getHeight() == 10);
    
    assert(field.isValidPosition(0, 0) == true);
    assert(field.isValidPosition(9, 9) == true);
    assert(field.isValidPosition(-1, 0) == false);
    assert(field.isValidPosition(10, 10) == false);
    
    auto player = std::make_unique<Player>(1, 1);
    field.setPlayer(std::move(player));
    assert(field.getPlayer() != nullptr);
    assert(field.hasPlayerAt(1, 1) == true);
    
    auto enemy = std::make_unique<Enemy>(2, 2);
    field.addEnemy(std::move(enemy));
    assert(field.getEnemies().size() == 1);
    assert(field.hasEnemyAt(2, 2) == true);

    Enemy* enemyPtr = field.getEnemyAt(2, 2);
    assert(enemyPtr != nullptr);
    field.removeEnemy(enemyPtr);
    assert(field.hasEnemyAt(2, 2) == false);
    
    std::cout << "Field tests passed!" << std::endl;
}

void testMovement() {
    std::cout << "Testing Movement..." << std::endl;
    
    Field field(10, 10);
    clearField(field);
    auto player = std::make_unique<Player>(5, 5);
    field.setPlayer(std::move(player));
    
    assert(field.getPlayer() != nullptr);
    assert(field.getPlayer()->getX() == 5);
    assert(field.getPlayer()->getY() == 5);
    
    assert(field.getPlayer()->canMove(1, 0, field) == true);
    assert(field.getPlayer()->canMove(-5, 0, field) == false);
    
    std::cout << "Movement tests passed!" << std::endl;
}

// ===== Testing Direct Damage Spell =====
void testDirectDamageSpell() {
    std::cout << "Testing DirectDamageSpell..." << std::endl;

    Field field(10, 10);
    clearField(field);

    auto player = std::make_unique<Player>(1, 1);
    Player* playerPtr = player.get();
    field.setPlayer(std::move(player));

    DirectDamageSpell spell(3, 20);

    field.clearCell(2, 1);
    auto enemy1 = std::make_unique<Enemy>(2, 1, 30, 5);
    field.addEnemy(std::move(enemy1));

    bool castSuccess = spell.cast(playerPtr, 2, 1, field);
    assert(castSuccess == true);
    Enemy* enemyAfter = field.getEnemyAt(2, 1);
    assert(enemyAfter != nullptr);
    assert(enemyAfter->getHP() == 10);
    assert(playerPtr->getScore() == 0);

    bool castNoTarget = spell.cast(playerPtr, 3, 1, field);
    assert(castNoTarget == false);

    bool castOutOfRange = spell.cast(playerPtr, 9, 9, field);
    assert(castOutOfRange == false);

    field.clearCell(3, 2);
    auto enemy2 = std::make_unique<Enemy>(3, 2, 10, 5);
    field.addEnemy(std::move(enemy2));
    Enemy* enemyToDie = field.getEnemyAt(3, 2);
    assert(enemyToDie != nullptr);
    int previousScore = playerPtr->getScore();

    bool castKill = spell.cast(playerPtr, 3, 2, field);
    assert(castKill == true);
    assert(field.getEnemyAt(3, 2) == nullptr);
    assert(playerPtr->getScore() == previousScore + 10);

    std::cout << "DirectDamageSpell tests passed!" << std::endl;
}

// ===== Testing Area Damage Spell =====
void testAreaDamageSpell() {
    std::cout << "Testing AreaDamageSpell..." << std::endl;

    Field field(10, 10);
    clearField(field);

    auto player = std::make_unique<Player>(2, 2);
    Player* playerPtr = player.get();
    field.setPlayer(std::move(player));

    AreaDamageSpell spell(4, 15);

    field.clearCell(3, 3);
    field.clearCell(4, 3);
    field.clearCell(3, 4);
    field.clearCell(4, 4);

    auto enemy1 = std::make_unique<Enemy>(3, 3, 25, 5);
    auto enemy2 = std::make_unique<Enemy>(4, 4, 25, 5);
    field.addEnemy(std::move(enemy1));
    field.addEnemy(std::move(enemy2));

    bool castSuccess = spell.cast(playerPtr, 3, 3, field);
    assert(castSuccess == true);
    Enemy* enemyA = field.getEnemyAt(3, 3);
    Enemy* enemyB = field.getEnemyAt(4, 4);
    assert(enemyA != nullptr);
    assert(enemyB != nullptr);
    assert(enemyA->getHP() == 10);
    assert(enemyB->getHP() == 10);

    bool castEmptyArea = spell.cast(playerPtr, 7, 7, field);

    std::cout << "AreaDamageSpell tests passed!" << std::endl;
}

// ===== Testing Player Casting via Hand =====
void testPlayerCasting() {
    std::cout << "Testing Player casting spells..." << std::endl;

    Field field(10, 10);
    clearField(field);

    auto player = std::make_unique<Player>(1, 1);
    Player* playerPtr = player.get();
    field.setPlayer(std::move(player));

    Hand& hand = playerPtr->getHand();
    int initialCount = hand.getSpellCount();

    bool added = hand.addSpell(std::make_unique<DirectDamageSpell>(3, 35));
    assert(added == true);
    int spellIndex = hand.getSpellCount() - 1;

    field.clearCell(2, 1);
    auto enemy = std::make_unique<Enemy>(2, 1, 25, 5);
    field.addEnemy(std::move(enemy));

    bool castSuccess = playerPtr->castSpell(spellIndex, 2, 1, field);
    assert(castSuccess == true);
    assert(field.getEnemyAt(2, 1) == nullptr);
    assert(playerPtr->getScore() >= 10);

    bool invalidIndex = playerPtr->castSpell(999, 2, 1, field);
    assert(invalidIndex == false);

    bool emptyTarget = playerPtr->castSpell(spellIndex, 3, 1, field);
    assert(emptyTarget == false);

    // Ensure original random spells remain accessible
    assert(hand.getSpellCount() >= initialCount);

    std::cout << "Player casting tests passed!" << std::endl;
}

void testControlsConfig() {
    std::cout << "Testing controls configuration..." << std::endl;

    // 1) Valid custom config should be applied
    writeControlsFile(
        "MOVE_UP=I\n"
        "MOVE_DOWN=K\n"
        "MOVE_LEFT=J\n"
        "MOVE_RIGHT=L\n"
        "SWITCH_MODE=U\n"
        "CAST_SPELL=O\n"
        "SAVE_GAME=P\n"
        "QUIT=Z\n"
    );
    {
        Game game;
        auto controls = game.getControlsForTest();
        assert(controls["MOVE_UP"] == 'I');
        assert(controls["MOVE_DOWN"] == 'K');
        assert(controls["MOVE_LEFT"] == 'J');
        assert(controls["MOVE_RIGHT"] == 'L');
        assert(controls["SWITCH_MODE"] == 'U');
        assert(controls["CAST_SPELL"] == 'O');
        assert(controls["SAVE_GAME"] == 'P');
        assert(controls["QUIT"] == 'Z');
    }

    // 2) Duplicate key should revert to defaults
    writeControlsFile(
        "MOVE_UP=W\n"
        "MOVE_DOWN=W\n"
        "MOVE_LEFT=A\n"
        "MOVE_RIGHT=D\n"
        "SWITCH_MODE=M\n"
        "CAST_SPELL=C\n"
        "SAVE_GAME=V\n"
        "QUIT=Q\n"
    );
    {
        Game game;
        auto controls = game.getControlsForTest();
        auto defaults = defaultControls();
        for (const auto& kv : defaults) {
            assert(controls[kv.first] == kv.second);
        }
    }

    // 3) Missing command should revert to defaults
    writeControlsFile(
        "MOVE_UP=W\n"
        "MOVE_DOWN=S\n"
        "MOVE_LEFT=A\n"
        "MOVE_RIGHT=D\n"
        "SWITCH_MODE=M\n"
        "CAST_SPELL=C\n"
        "SAVE_GAME=V\n"
        // QUIT is missing
    );
    {
        Game game;
        auto controls = game.getControlsForTest();
        auto defaults = defaultControls();
        for (const auto& kv : defaults) {
            assert(controls[kv.first] == kv.second);
        }
    }

    // Restore default controls file for future runs
    writeControlsFile(
        "MOVE_UP=W\n"
        "MOVE_DOWN=S\n"
        "MOVE_LEFT=A\n"
        "MOVE_RIGHT=D\n"
        "SWITCH_MODE=M\n"
        "CAST_SPELL=C\n"
        "SAVE_GAME=V\n"
        "QUIT=Q\n"
    );

    std::cout << "Controls configuration tests passed!" << std::endl;
}

void testGameStateSerialization() {
    std::cout << "Testing GameState serialization..." << std::endl;

    const std::string path = "test_save.dat";

    // Build state
    GameState state;
    state.setPlayer(3, 4, 75, 100, 50, 12, 7, 1);
    state.setHandMaxSize(3);
    state.addSpell({0, 2, 10});
    state.addSpell({1, 3, 15});
    state.setFieldSize(8, 9);
    state.addWall(1, 1);
    state.addWall(2, 2);
    state.addEnemy(5, 5, 30, 6);
    state.addEnemy(6, 6, 25, 5);
    state.setProgress(2, 4, 1);

    std::string error;
    bool saved = state.saveToFile(path, error);
    assert(saved && error.empty());

    auto loaded = GameState::loadFromFile(path, error);
    assert(loaded.has_value());
    const GameState& s = *loaded;

    assert(s.getPlayerX() == 3);
    assert(s.getPlayerY() == 4);
    assert(s.getPlayerHP() == 75);
    assert(s.getPlayerMaxHP() == 100);
    assert(s.getPlayerScore() == 50);
    assert(s.getMeleeDamage() == 12);
    assert(s.getRangedDamage() == 7);
    assert(s.getCombatMode() == 1);
    assert(s.getHandMaxSize() == 3);
    assert(s.getSpells().size() == 2);
    assert(s.getSpells()[0].type == 0);
    assert(s.getSpells()[1].type == 1);
    assert(s.getFieldWidth() == 8);
    assert(s.getFieldHeight() == 9);
    assert(s.getWalls().size() == 2);
    assert(s.getEnemies().size() == 2);
    assert(s.getEnemyHP()[0] == 30);
    assert(s.getEnemyDamage()[1] == 5);
    assert(s.getCurrentLevel() == 2);
    assert(s.getInitialEnemyCount() == 4);
    assert(s.getEnemiesKilled() == 1);

    // Invalid header should fail
    writeFile(path, "BAD_HEADER\n");
    error.clear();
    auto badHeader = GameState::loadFromFile(path, error);
    assert(!badHeader.has_value());

    // Invalid enemy count should fail
    writeFile(path,
        "GAME_SAVE_V1\n"
        "PLAYER\n"
        "0 0 1 1 0 1 1 0\n"
        "HAND\n"
        "1 0\n"
        "FIELD\n"
        "5 5\n"
        "0\n"
        "ENEMIES\n"
        "-1\n"
        "GAME\n"
        "1 0 0\n"
        "END\n"
    );
    auto badEnemies = GameState::loadFromFile(path, error);
    assert(!badEnemies.has_value());

    // Clean up
    std::remove(path.c_str());

    std::cout << "GameState serialization tests passed!" << std::endl;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::cout << "=== Starting Simple Game Tests ===" << std::endl;
    std::cout << "Running basic functionality tests...\n" << std::endl;
    
    try {
        testCell();
        testEntity();
        testPlayer();
        testEnemy();
        testHand();
        testField();
        testMovement();
        testDirectDamageSpell();
        testAreaDamageSpell();
        testPlayerCasting();
        testControlsConfig();
        testGameStateSerialization();
        
        std::cout << "\n=== ALL TESTS PASSED! ===" << std::endl;
        std::cout << "Basic game functionality is working correctly!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\nUnknown test failure" << std::endl;
        return 1;
    }
    
    return 0;
}