#include "GameField.h"
#include <iostream>
#include <vector>
#include <algorithm>

GameField::GameField(int size) : fieldSize(size), playerPtr(nullptr) {
    if (fieldSize < 10) {
        fieldSize = 10;
    }
    if (fieldSize > 25) {
        fieldSize = 25;
    }

    field.resize(fieldSize, std::vector<Cell>(fieldSize));

    for (int i = 0; i < fieldSize; ++i) {
            for (int j = 0; j < fieldSize; ++j) {
                field[i][j].SetContent(Cell::CellType::Empty);
            }
        }
}

void GameField::SetSize(int size) {
    fieldSize = size;
    InitializeField();  
}

void GameField::InitializeField() {
    field.clear();
    field.resize(fieldSize);
    for (int y = 0; y < fieldSize; ++y) {
        field[y].resize(fieldSize);
        for (int x = 0; x < fieldSize; ++x) {
            field[y][x] = Cell(); 
        }
    }
}

void GameField::PlacePlayer(Player* player, int x, int y) {
    if (!IsInside(x, y))
        return;
    if (playerPtr) {
        field[playerPtr->GetY()][playerPtr->GetX()].SetContent(Cell::CellType::Empty);
    }

    playerPtr = player;
    playerPtr->Move(x - playerPtr->GetX(), y - playerPtr->GetY());
    field[y][x].SetContent(Cell::CellType::Player);
}

void GameField::MovePlayer(int dx, int dy) {
    if (!playerPtr)
        return;

    int newX = playerPtr->GetX() + dx;
    int newY = playerPtr->GetY() + dy;

    if (!IsInside(newX, newY) || !IsCellFree(newX, newY))
        return;

    field[playerPtr->GetY()][playerPtr->GetX()].SetContent(Cell::CellType::Empty);
    playerPtr->Move(dx, dy);
    field[playerPtr->GetY()][playerPtr->GetX()].SetContent(Cell::CellType::Player);
}



void GameField::PlaceEnemy(Enemy* enemy, int x, int y) {
    if (IsInside(x, y) && IsCellFree(x, y)) {
        enemy->Move(x, y);
        field[x][y].SetContent(Cell::CellType::Enemy);
        enemies.push_back(enemy);
    } else {
        std::cout << "Can't place enemy at (" << x << ", " << y << "). Cell is not free or out of bounds!" << std::endl;
    }
}

void GameField::MoveEnemy(int index, int dx, int dy) {
    if (index < 0 || index >= static_cast<int>(enemies.size())) return;
    Enemy* enemy = enemies[index];

    int oldX = enemy->GetX();
    int oldY = enemy->GetY();

    int newX = oldX + dx;
    int newY = oldY + dy;

    if (!IsInside(newX, newY)) return;

    field[oldX][oldY].SetContent(Cell::CellType::Empty);
    if (field[newX][newY].GetContent() == Cell::CellType::Player) {
        DamagePlayer(enemy->GetDamage());
        field[oldX][oldY].SetContent(Cell::CellType::Enemy);
        return;
    }

    if (!IsCellFree(newX, newY)) {
        field[oldX][oldY].SetContent(Cell::CellType::Enemy);
        return;
    }

    enemy->Move(dx, dy);
    field[newX][newY].SetContent(Cell::CellType::Enemy);
}

void GameField::RemoveEnemy(Enemy* enemy) {
    auto it = std::find(enemies.begin(), enemies.end(), enemy);
    if (it != enemies.end()) {
        enemies.erase(it);
        delete enemy;
    }
}

bool GameField::IsInside(int x, int y) const {
    return x >= 0 && x < fieldSize && y >= 0 && y < fieldSize;
}

bool GameField::IsCellFree(int x, int y) const {
    return field[x][y].GetContent() == Cell::CellType::Empty;
}

bool GameField::GameIsOver() const {
    return playerPtr != nullptr && !playerPtr->IsAlive();
}

void GameField::CheckAndApplyDamage(int x, int y) {
    if (field[x][y].GetContent() == Cell::CellType::Enemy) {
        for (Enemy* enemy : enemies) {
            if (enemy->GetX() == x && enemy->GetY() == y) {
                DamagePlayer(enemy->GetDamage());
                break;  
            }
        }
    }
}

void GameField::DamagePlayer(int damage) {
    if (playerPtr != nullptr) {
        playerPtr->TakeDamage(damage);
        if (!playerPtr->IsAlive()) {
            std::cout << "Game Over!" << std::endl;
        }
    }
}

std::vector<Enemy*> GameField::GetEnemies() const {
    return enemies;
}

void GameField::Clear() {
    for (auto* e : enemies)
        delete e;

    enemies.clear();

    for (int i = 0; i < fieldSize; ++i)
        for (int j = 0; j < fieldSize; ++j)
            field[i][j].SetContent(Cell::CellType::Empty);

    playerPtr = nullptr;
}

GameField::GameField(const GameField& other)
    : fieldSize(other.fieldSize),
      field(other.field)
{
    if (other.playerPtr)
        playerPtr = new Player(*other.playerPtr);
    else
        playerPtr = nullptr;

    for (auto* e : other.enemies)
        enemies.push_back(new Enemy(*e));
}

GameField& GameField::operator=(const GameField& other) {
    if (this == &other)
        return *this;

    Clear();

    fieldSize = other.fieldSize;
    field = other.field;

    if (other.playerPtr)
        playerPtr = new Player(*other.playerPtr);
    else
        playerPtr = nullptr;

    for (auto* e : other.enemies)
        enemies.push_back(new Enemy(*e));

    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : fieldSize(other.fieldSize),
      field(std::move(other.field)),
      playerPtr(other.playerPtr),
      enemies(std::move(other.enemies))
{
    other.playerPtr = nullptr;
    other.enemies.clear();
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this == &other)
        return *this;

    Clear();

    fieldSize = other.fieldSize;
    field = std::move(other.field);
    playerPtr = other.playerPtr;
    enemies = std::move(other.enemies);

    other.playerPtr = nullptr;
    other.enemies.clear();

    return *this;
}

GameField::~GameField() {
    Clear();
}

int GameField::GetEnemiesCount() const {
    return static_cast<int>(enemies.size());
}

void GameField::DisplayField() const {
    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            if (field[i][j].GetContent() == Cell::CellType::Player) {
                std::cout << "P ";
            } else if (field[i][j].GetContent() == Cell::CellType::Enemy) {
                std::cout << "E ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
}

int GameField::GetSize() const {
    return fieldSize;
}