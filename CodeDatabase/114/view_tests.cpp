#include <gtest/gtest.h>
#include "GeneralView.h"
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include <sstream>
#include <iostream>

// ========== GeneralView Tests ==========
class GeneralViewTest : public ::testing::Test {
protected:
    void SetUp() override {
        view = std::make_unique<GeneralView>();
        field = std::make_unique<GameField>(10, 10);
    }

    std::unique_ptr<GeneralView> view;
    std::unique_ptr<GameField> field;
};

TEST_F(GeneralViewTest, GeneralViewPrintField) {
    // Проверяем, что метод не выбрасывает исключений
    EXPECT_NO_THROW(view->printField(*field));
}

TEST_F(GeneralViewTest, GeneralViewPrintPlayer) {
    Player player;
    player.setPosition(5, 5);
    player.setHealth(75);
    
    // Проверяем, что метод не выбрасывает исключений
    EXPECT_NO_THROW(view->printPlayer(player));
}

TEST_F(GeneralViewTest, GeneralViewPrintEnemies) {
    std::vector<Enemy> enemies;
    Enemy enemy1(50, 15);
    enemy1.setPosition(3, 3);
    enemies.push_back(enemy1);
    
    Enemy enemy2(75, 20);
    enemy2.setPosition(7, 7);
    enemies.push_back(enemy2);
    
    // Проверяем, что метод не выбрасывает исключений
    EXPECT_NO_THROW(view->printEnemies(enemies));
}

TEST_F(GeneralViewTest, GeneralViewPrintEmptyEnemies) {
    std::vector<Enemy> enemies;
    // Проверяем, что метод работает с пустым вектором
    EXPECT_NO_THROW(view->printEnemies(enemies));
}

TEST_F(GeneralViewTest, GeneralViewPrintBuildings) {
    std::vector<EnemyBuilding> buildings;
    EnemyBuilding building1(2, 2, 3);
    buildings.push_back(building1);
    
    EnemyBuilding building2(8, 8, 5);
    buildings.push_back(building2);
    
    // Проверяем, что метод не выбрасывает исключений
    EXPECT_NO_THROW(view->printBuildings(buildings));
}

TEST_F(GeneralViewTest, GeneralViewPrintEmptyBuildings) {
    std::vector<EnemyBuilding> buildings;
    // Проверяем, что метод работает с пустым вектором
    EXPECT_NO_THROW(view->printBuildings(buildings));
}

TEST_F(GeneralViewTest, GeneralViewPrint) {
    // Проверяем, что метод не выбрасывает исключений
    EXPECT_NO_THROW(view->print("Test message"));
    EXPECT_NO_THROW(view->print("Another test message"));
    EXPECT_NO_THROW(view->print(""));
}

TEST_F(GeneralViewTest, GeneralViewPrintWithSpecialCharacters) {
    // Проверяем работу с различными строками
    EXPECT_NO_THROW(view->print("Message with numbers: 12345"));
    EXPECT_NO_THROW(view->print("Message with symbols: !@#$%"));
    EXPECT_NO_THROW(view->print("Message with newline\nand tab\t"));
}

TEST_F(GeneralViewTest, GeneralViewPrintLongMessage) {
    std::string longMessage(1000, 'A');
    // Проверяем работу с длинными сообщениями
    EXPECT_NO_THROW(view->print(longMessage));
}

TEST_F(GeneralViewTest, GeneralViewPrintFieldWithEntities) {
    // Настраиваем поле с различными сущностями
    field->getPlayer().setPosition(5, 5);
    field->getCells()[5][5].setHasEntity(true);
    
    Enemy enemy(50, 15);
    enemy.setPosition(3, 3);
    field->getEnemies().push_back(enemy);
    field->getCells()[3][3].setHasEntity(true);
    
    EnemyBuilding building(7, 7, 3);
    field->getBuilding().push_back(building);
    
    // Проверяем, что метод корректно обрабатывает поле с сущностями
    EXPECT_NO_THROW(view->printField(*field));
}

TEST_F(GeneralViewTest, GeneralViewPrintFieldWithWalls) {
    // Добавляем стены на поле
    field->getCells()[2][2].setType(CellType::WALL);
    field->getCells()[3][3].setType(CellType::WALL);
    field->getCells()[4][4].setType(CellType::SLOW);
    
    // Проверяем, что метод корректно обрабатывает поле с препятствиями
    EXPECT_NO_THROW(view->printField(*field));
}

TEST_F(GeneralViewTest, GeneralViewPrintPlayerWithFullStats) {
    Player player(150, 30, 25);
    player.setPosition(5, 5);
    player.setHealth(120);
    player.setMoney(5000);
    player.setCombatMode(CombatMode::RANGED);
    
    // Проверяем вывод игрока с полными характеристиками
    EXPECT_NO_THROW(view->printPlayer(player));
}

TEST_F(GeneralViewTest, GeneralViewPrintPlayerWithHand) {
    Player player;
    player.getHand().addSpell(SpellType::DIRECT_DAMAGE);
    player.getHand().addSpell(SpellType::AREA_DAMAGE);
    player.getHand().addSpell(SpellType::TRAP);
    
    // Проверяем вывод игрока с заклинаниями
    EXPECT_NO_THROW(view->printPlayer(player));
}

TEST_F(GeneralViewTest, GeneralViewMultipleCalls) {
    // Проверяем множественные вызовы методов
    for (int i = 0; i < 10; ++i) {
        EXPECT_NO_THROW(view->print("Message " + std::to_string(i)));
        EXPECT_NO_THROW(view->printField(*field));
        EXPECT_NO_THROW(view->printPlayer(field->getPlayer()));
    }
}


