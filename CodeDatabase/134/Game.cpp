#include "Game.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

Game::Game()
    : field(10),
      player(100, 20),
      hand(3),
      isRunning(true),
      levelActive(false),
      levelStarted(false),
      levelCompleted(false),
      gameLoaded(false),
      gameOver(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Game::Run() {
    StartGame();

    if (!gameLoaded) {
        StartLevel();  
    }

    while (isRunning) {
        while (levelActive && isRunning) {
            bool playerMovedThisTurn = false;

            PlayerTurn(playerMovedThisTurn); 
            CheckWinLoseConditions();
            if (!levelActive) break;

            if (playerMovedThisTurn) {
                EnemyTurn();
                CheckWinLoseConditions();
            }
        }
    }
}

void Game::StartGame() {
    std::cout << "GAME START\n";
    std::cout << "1 - New Game\n2 - Load Game\n3 - Exit\n";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        levelActive = true;
    }
    if (choice == 2) {
        std::string filename;
        std::cout << "Enter savefile name: ";
        std::cin >> filename;
        LoadGame(filename);
        gameLoaded = true;
    }
    if (choice == 3) {
        isRunning = false;
        levelActive = false;
    }
}

void Game::StartLevel() {
    std::cout << "\n LEVEL START \n";

    field.Clear();

    player = Player(100, 20);
    hand = Hand(3);
    field.InitializeField(); 
    field.PlacePlayer(&player, 0, 0);
    Enemy* e = new Enemy(50, 5);
    field.PlaceEnemy(e, 3, 3);
    levelActive = true;
    levelCompleted = false;
    gameOver = false;
}
void Game::PlayerTurn(bool& playerMovedThisTurn) {
    std::cout << "\n--- Player Turn ---\n";
    std::cout << "Player HP: " << player.GetHealth()
              << " | Damage: " << player.GetDamage()
              << " | Enemies: " << field.GetEnemiesCount() << "\n";

    field.DisplayField();

    HandlePlayerCommand(playerMovedThisTurn);
}

void Game::EnemyTurn() {
    std::cout << "\n--- Enemy Turn ---\n";

    for (int i = 0; i < field.GetEnemiesCount(); ++i) {
        int dir = std::rand() % 4;

        int dx = 0;
        int dy = 0;

        switch (dir) {
            case 0: dx = 1;  break; 
            case 1: dx = -1; break; 
            case 2: dy = 1;  break; 
            case 3: dy = -1; break; 
        }

        field.MoveEnemy(i, dx, dy);
    }
    field.DisplayField();
}


void Game::HandlePlayerCommand(bool& playerMovedThisTurn) {
    std::cout << "Command (move / attack / spell / save / load / exit): ";
    std::string command;
    std::cin >> command;

    if (command == "move") {
        int dx, dy;
        std::cin >> dx >> dy;
        field.MovePlayer(dx, dy);
        playerMovedThisTurn = true;
    }
    else if (command == "spell") {
        int index, x, y;
        std::cin >> index >> x >> y;
        hand.UseSpell(index, player, field, x, y);
        playerMovedThisTurn = true;
    }
    else if (command == "exit") {
        isRunning = false;
        levelActive = false;
        playerMovedThisTurn = false;
    }
    else if (command == "attack") {
        player.Attack(field);
        playerMovedThisTurn = true;
    }
    else if (command == "save") {
        std::string filename;
        do {
            std::cout << "Enter filename to save: ";
            std::getline(std::cin >> std::ws, filename); 
            if (filename.empty()) {
                std::cout << "Filename cannot be empty.\n";
            }
        } while (filename.empty());
        SaveGame(filename);
        playerMovedThisTurn = false;
    }   
    else if (command == "load") {
        std::string filename;
        do {
            std::cout << "Enter filename to load: ";
            std::getline(std::cin >> std::ws, filename);
            if (filename.empty()) {
                std::cout << "Filename cannot be empty.\n";
            }
        } while (filename.empty());
        LoadGame(filename);
        playerMovedThisTurn = false;
    }
}

void Game::CheckWinLoseConditions() {
    if (field.GetEnemiesCount() == 0) {
        levelCompleted = true;
        std::cout << "Level is completed!\n";
        levelActive = false;
        return;
    }

    if (!player.IsAlive()) {
        gameOver = true;
        levelActive = false;
        HandleGameOver();
    }
}

void Game::HandleGameOver() {
    std::cout << "\n GAME OVER \n";
    std::cout << "1 - New Game\n2 - Load\n 3 - Exit\n";

    int choice;
    std::cin >> choice;

    if (choice == 1) {
        levelStarted = false;
        StartLevel();
    } else if (choice == 2){
        std::string filename;
        do {
            std::cout << "Enter filename to load: ";
            std::getline(std::cin >> std::ws, filename);
            if (filename.empty()) {
                std::cout << "Filename cannot be empty.\n";
            }
        } while (filename.empty());
        LoadGame(filename);
    } else {
        isRunning = false;
    }
}

void Game::SaveGame(const std::string& filename) {
    std::ofstream out(filename);
    if (!out) {
        std::cout << "Error: cannot open file for saving.\n";
        return;
    }

    try {
        out << player.GetHealth() << ' ' << player.GetDamage() << ' '
            << player.GetX() << ' ' << player.GetY() << '\n';

        int size = field.GetSize();
        out << size << '\n';

        int enemyCount = field.GetEnemiesCount();
        out << enemyCount << '\n';
        for (Enemy* e : field.GetEnemies()) {
            out << e->GetHealth() << ' ' << e->GetDamage() << ' '
                << e->GetX() << ' ' << e->GetY() << '\n';
        }

        std::cout << "Game saved successfully to " << filename << '\n';
    }
    catch (...) {
        std::cout << "Error: failed to save game.\n";
    }
}

void Game::LoadGame(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cout << "Error: file does not exist.\n";
        return;
    }

    try {
        int health, damage, x, y;

        in >> health >> damage >> x >> y;
        player = Player(health, damage);
        player.Move(x - player.GetX(), y - player.GetY());

        int size;
        in >> size;
        field.Clear();          
        field.SetSize(size);   
        field.PlacePlayer(&player, x, y);

        int enemyCount;
        in >> enemyCount;
        for (int i = 0; i < enemyCount; ++i) {
            int eHealth, eDamage, ex, ey;
            in >> eHealth >> eDamage >> ex >> ey;
            Enemy* e = new Enemy(eHealth, eDamage);
            field.PlaceEnemy(e, ex, ey);
        }

        levelActive = true;
        gameLoaded = true;
        std::cout << "Game loaded successfully from " << filename << '\n';
    }
    catch (...) {
        std::cout << "Error: failed to load game. File may be corrupted.\n";
    }
}
