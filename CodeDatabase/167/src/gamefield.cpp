#include "gamefield.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace
{

  int clampValue(int value, int min, int max)
  {
    if (value < min)
      return min;
    if (value > max)
      return max;
    return value;
  }
}

GameField::GameField(int fieldWidth, int fieldHeight)
    : width(clampValue(fieldWidth, 10, 25)),
      height(clampValue(fieldHeight, 10, 25)),
      player(nullptr)
{
  grid.resize(height, std::vector<Cell>(width));
}

GameField::GameField(const GameField &other)
{
  copyFrom(other);
}

GameField::GameField(GameField &&other) noexcept
{
  moveFrom(std::move(other));
}

GameField &GameField::operator=(const GameField &other)
{
  if (this != &other)
  {
    clearField();
    copyFrom(other);
  }
  return *this;
}

GameField &GameField::operator=(GameField &&other) noexcept
{
  if (this != &other)
  {
    clearField();
    moveFrom(std::move(other));
  }
  return *this;
}

void GameField::copyFrom(const GameField &other)
{
  width = other.width;
  height = other.height;
  player = other.player;

  grid = other.grid;

  enemies.clear();
  for (const auto &enemy : other.enemies)
  {
    enemies.push_back(std::unique_ptr<Enemy>(new Enemy(*enemy)));
  }
}

void GameField::moveFrom(GameField &&other) noexcept
{
  width = other.width;
  height = other.height;
  player = other.player;
  grid = std::move(other.grid);
  enemies = std::move(other.enemies);

  other.width = 0;
  other.height = 0;
  other.player = nullptr;
}

void GameField::clearField()
{
  grid.clear();
  enemies.clear();
  player = nullptr;
  width = 0;
  height = 0;
}

bool GameField::isValidPosition(int x, int y) const
{
  return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::isWithinBounds(int x, int y) const
{
  return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::placePlayer(Player *playerPtr, int x, int y)
{
  if (!isValidPosition(x, y) || !grid[y][x].isEmpty())
  {
    return false;
  }

  player = playerPtr;
  player->setPosition(x, y);
  grid[y][x].setPlayer(player);
  return true;
}

bool GameField::addEnemy(std::unique_ptr<Enemy> enemy, int x, int y)
{
  if (!isValidPosition(x, y) || !grid[y][x].isEmpty())
  {
    return false;
  }

  enemy->setPosition(x, y);
  grid[y][x].setEnemy(enemy.get());
  enemies.push_back(std::move(enemy));
  return true;
}

bool GameField::placeEnemy(Enemy *enemy, int x, int y)
{
  if (!isValidPosition(x, y) || !grid[y][x].isEmpty())
  {
    delete enemy;
    return false;
  }

  enemy->setPosition(x, y);
  grid[y][x].setEnemy(enemy);
  enemies.push_back(std::unique_ptr<Enemy>(enemy));
  return true;
}

bool GameField::movePlayer(Direction direction)
{
  if (!player || !player->isAlive())
  {
    std::cout << "Player is not alive!" << std::endl;
    return false;
  }

  int currentX = player->getPosX();
  int currentY = player->getPosY();
  int newX = currentX;
  int newY = currentY;

  switch (direction)
  {
  case Direction::UP:
    newY--;
    break;
  case Direction::DOWN:
    newY++;
    break;
  case Direction::LEFT:
    newX--;
    break;
  case Direction::RIGHT:
    newX++;
    break;
  case Direction::NONE:
    std::cout << "Invalid direction!" << std::endl;
    return false;
  }

  if (!isValidPosition(newX, newY))
  {
    std::cout << "Cannot move there - out of bounds!" << std::endl;
    return false;
  }

  if (grid[newY][newX].hasEnemy())
  {

    for (const auto &enemy : enemies)
    {
      if (enemy->getPosX() == newX && enemy->getPosY() == newY && enemy->isAlive())
      {
        std::cout << "Player encounters " << enemy->getType() << "!" << std::endl;
        player->attack(*enemy);

        if (!enemy->isAlive())
        {
          player->addScore(100);
          player->enemyDefeated();
          grid[newY][newX].clear();
          std::cout << "Gained 100 points!" << std::endl;
        }
        return true;
      }
    }
  }

  if (grid[newY][newX].isEmpty())
  {
    grid[currentY][currentX].clear();
    player->move(newX, newY);
    grid[newY][newX].setPlayer(player);
    std::cout << "Player moves to (" << newX << ", " << newY << ")" << std::endl;
    return true;
  }

  std::cout << "Cannot move there - cell is occupied!" << std::endl;
  return false;
}

void GameField::moveEnemies()
{
  static std::random_device rd;
  static std::mt19937 gen(rd());
  std::uniform_int_distribution<> dirDist(0, 3);

  bool anyEnemyMoved = false;

  for (auto &enemy : enemies)
  {
    if (!enemy->isAlive())
      continue;

    int currentX = enemy->getPosX();
    int currentY = enemy->getPosY();

    Direction directions[4] = {Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT};
    Direction chosenDirection = directions[dirDist(gen)];

    int newX = currentX;
    int newY = currentY;

    switch (chosenDirection)
    {
    case Direction::UP:
      newY--;
      break;
    case Direction::DOWN:
      newY++;
      break;
    case Direction::LEFT:
      newX--;
      break;
    case Direction::RIGHT:
      newX++;
      break;
    default:
      break;
    }

    if (!isValidPosition(newX, newY))
    {
      continue;
    }

    if (grid[newY][newX].hasPlayer())
    {

      std::cout << enemy->getType() << " approaches player!" << std::endl;
      enemy->attack(*player);
      if (!player->isAlive())
      {
        grid[currentY][currentX].clear();
      }
      anyEnemyMoved = true;
      continue;
    }

    if (grid[newY][newX].isEmpty())
    {
      grid[currentY][currentX].clear();
      enemy->move(newX, newY);
      grid[newY][newX].setEnemy(enemy.get());
      anyEnemyMoved = true;
    }
  }

  removeDeadEnemies();

  if (!anyEnemyMoved)
  {
    std::cout << "Enemies are considering their next move..." << std::endl;
  }
}

int GameField::getWidth() const
{
  return width;
}

int GameField::getHeight() const
{
  return height;
}

const Player *GameField::getPlayer() const
{
  return player;
}

const std::vector<std::unique_ptr<Enemy>> &GameField::getEnemies() const
{
  return enemies;
}

CellType GameField::getCellType(int x, int y) const
{
  if (!isValidPosition(x, y))
  {
    return CellType::WALL;
  }
  return grid[y][x].getType();
}

bool GameField::isPlayerAlive() const
{
  return player && player->isAlive();
}

bool GameField::isCellEmpty(int x, int y) const
{
  if (!isValidPosition(x, y))
  {
    return false;
  }
  return grid[y][x].isEmpty();
}

void GameField::display() const
{
  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      char symbol = '.';
      switch (grid[y][x].getType())
      {
      case CellType::PLAYER:
        symbol = 'P';
        break;
      case CellType::ENEMY:
        symbol = 'E';
        break;
      case CellType::WALL:
        symbol = '#';
        break;
      case CellType::EMPTY:
        symbol = '.';
        break;
      }
      std::cout << symbol << ' ';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void GameField::removeDeadEnemies()
{

  auto it = enemies.begin();
  while (it != enemies.end())
  {
    if (!(*it)->isAlive())
    {

      if (player && player->isAlive())
      {
        player->enemyDefeated();
      }
      it = enemies.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

Enemy *GameField::getEnemyAt(int x, int y)
{
  if (!isValidPosition(x, y))
  {
    return nullptr;
  }

  for (auto &enemy : enemies)
  {
    if (enemy->getPosX() == x && enemy->getPosY() == y && enemy->isAlive())
    {
      return enemy.get();
    }
  }

  return nullptr;
}

void GameField::applyAreaDamage(int centerX, int centerY, int areaSize, int damage, Player &caster)
{
  if (!isValidPosition(centerX, centerY))
  {
    std::cout << "Invalid target coordinates!" << std::endl;
    return;
  }

  int halfSize = areaSize / 2;
  int enemiesDamaged = 0;

  std::cout << "Casting area damage at (" << centerX << ", " << centerY << ")..." << std::endl;

  for (int y = centerY - halfSize; y <= centerY + halfSize; y++)
  {
    for (int x = centerX - halfSize; x <= centerX + halfSize; x++)
    {
      if (isValidPosition(x, y) && grid[y][x].hasEnemy())
      {
        Enemy *enemy = getEnemyAt(x, y);
        if (enemy && enemy->isAlive())
        {
          std::cout << "Area damage hits " << enemy->getType()
                    << " for " << damage << " damage!" << std::endl;
          enemy->takeDamage(damage);
          enemiesDamaged++;

          if (!enemy->isAlive())
          {
            caster.addScore(30);
            caster.enemyDefeated();
            grid[y][x].clear();
            std::cout << "Gained 30 points!" << std::endl;
          }
        }
      }
    }
  }

  if (enemiesDamaged > 0)
  {
    std::cout << "Area damage hit " << enemiesDamaged << " enemies total!" << std::endl;
  }
  else
  {
    std::cout << "Area damage hit no enemies." << std::endl;
  }
}

bool GameField::hasEnemyInRadius(int centerX, int centerY, int radius) const
{
  for (int y = centerY - radius; y <= centerY + radius; y++)
  {
    for (int x = centerX - radius; x <= centerX + radius; x++)
    {
      if (isValidPosition(x, y) && grid[y][x].hasEnemy())
      {
        return true;
      }
    }
  }
  return false;
}

bool GameField::hasEnemyAt(int x, int y) const
{
  if (!isValidPosition(x, y))
  {
    return false;
  }
  return grid[y][x].hasEnemy();
}

bool GameField::applyDirectDamage(int targetX, int targetY, int damage, Player &caster)
{
  if (!isValidPosition(targetX, targetY))
  {
    std::cout << "Invalid target coordinates!" << std::endl;
    return false;
  }

  if (!grid[targetY][targetX].hasEnemy())
  {
    std::cout << "No enemy at target location!" << std::endl;
    return false;
  }

  Enemy *enemy = getEnemyAt(targetX, targetY);
  if (enemy && enemy->isAlive())
  {
    std::cout << "Direct damage spell targeting " << enemy->getType() << "..." << std::endl;
    enemy->takeDamage(damage);

    if (!enemy->isAlive())
    {
      caster.addScore(50);
      caster.enemyDefeated();
      grid[targetY][targetX].clear();
      std::cout << "Gained 50 points!" << std::endl;
    }
    return true;
  }

  std::cout << "Target enemy is already defeated!" << std::endl;
  return false;
}