// MainGame.cpp
#include "MainGame.h"
#include "LoadSaveGame.h"
#include <iostream>
#include <ctime>

MainGame::MainGame()
    : inputHandler("controls.config")
    , controller(inputHandler)
    , visualizer()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

MainGame::~MainGame() {
    delete field;
}

// === Сохранение ===
void MainGame::saveGame() {
    try {
        LoadSaveGame::save(*field, *player, currentLevel, "save.dat");
        std::cout << "Game saved successfully!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Save failed: " << e.what() << "\n";
    }
}

// === Переход на уровень ===
void MainGame::levelUp() {
    currentLevel++;
    std::cout << "\n=== LEVEL UP! ===\n";
    std::cout << "Entering level " << currentLevel << "...\n";

    Player* currentPlayer = field->getPlayer();

    // ������� �������� ���������� ��������� �������
    PlayerHand& hand = const_cast<PlayerHand&>(currentPlayer->getHand());
    int toRemove = hand.getSize() / 2;
    for (int i = 0; i < toRemove; i++) {
        int idx = std::rand() % hand.getSize();
        hand.removeSpell(idx);
    }

    // �������� ������
    int choice;
    std::cout << "Choose upgrade: 1) Health 2) Damage\n";
    std::cin >> choice;
    switch (choice) {
    case 1:
        currentPlayer->heal(20);
        break;
    case 2:
        currentPlayer->increaseDamage(5);
        break;
    default:
        std::cout << "No upgrade applied." << std::endl;
        break;
    }

    int newWidth = 15 + currentLevel * 2;
    int newHeight = 15 + currentLevel * 2;
    GameField* newField = new GameField(newWidth, newHeight);

    int startX = newWidth / 2;
    int startY = newHeight / 2;

    newField->setPlayer(currentPlayer);
    currentPlayer->setPosition({ startX, startY });
    newField->getCell({ startX, startY }).setHasPlayer(true);

    newField->addEnemy(2, 2);
    newField->addEnemy(newWidth - 3, 3);
    newField->addEnemy(3, newHeight - 4);
    newField->addEnemy(newWidth - 4, newHeight - 3);
    newField->addTower(EnemyTower(3, 3));
    newField->addTower(EnemyTower(newWidth - 4, newHeight - 4));

    currentPlayer->getHand().addSpell(PlayerHand::getRandomStarterSpell());

    // �������� ������ ���� �� �����
    delete field;
    field = newField;
}

// === Инициализация уровня ===
void MainGame::initLevel(int width, int height) {
    delete field;
    field = new GameField(width, height);
    field->placePlayer(width / 2, height / 2);

    field->addEnemy(2, 2);
    field->addEnemy(width - 3, 3);
    field->addEnemy(3, height - 4);
    field->addEnemy(width - 4, height - 3);
    field->addTower(EnemyTower(3, 3));
    field->addTower(EnemyTower(width - 4, height - 4));

    player = field->getPlayer();
    player->getHand().addSpell(PlayerHand::getRandomStarterSpell());
}

// === Меню старта ===
void MainGame::start() {
    char choice;
    std::cout << "=== MAGIC BATTLE ===\n";
    std::cout << "1 - New Game\n2 - Load Game\nChoice: ";
    std::cin >> choice;

    if (choice == '2') {
        try {
            GameField* loadedField = nullptr;
            Player* loadedPlayer = nullptr;
            int loadedLevel = 1;
            LoadSaveGame::load(loadedField, loadedPlayer, loadedLevel, "save.dat");

            delete field;
            field = loadedField;
            player = loadedPlayer;
            currentLevel = loadedLevel;

            // Важно: устанавливаем игрока в поле (если нужно)
            field->player = player;

            std::cout << "Game loaded!\n";
        }
        catch (...) {
            std::cout << "Load failed. Starting new game.\n";
            initLevel(15, 15);
        }
    }
    else {
        initLevel(15, 15);
    }

    gameLoop();
}

// === Основной цикл ===
void MainGame::gameLoop() {
    while (running && field && !field->isGameOver()) {
        visualizer.update(*field, *player);
        controller.processInput(*field, *player);

        // Обновление игры
        if (player && player->isAlive()) {
            player->addMana(10);
            field->moveEnemies();
            field->updateTowers();
            field->checkTraps();
            field->removeDeadEnemies();
            field->removeDeactivatedTraps();
        }

        if (!player || !player->isAlive()) {
            askRestartOrQuit();
            if (!running) break;
        }

        if (field->isLevelCleared()) {
            levelUp();
        }
    }

    if (!running) {
        std::cout << "Game exited.\n";
    }
    else {
        std::cout << "\n=== GAME OVER ===\nFinal Score: " << player->getScore() << "\n";
    }
}

void MainGame::askRestartOrQuit() {
    char ch;
    std::cout << "You died! Restart? (y/n): ";
    std::cin >> ch;
    if (ch == 'y' || ch == 'Y') {
        currentLevel = 1;
        initLevel(15, 15);
        gameLoop();
    }
    else {
        running = false;
    }
}