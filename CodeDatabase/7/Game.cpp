#include "Game.h"
#include "GameException.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

// Конструктор
Game::Game() : currentLevel(0), isGameOver(false), logger(nullptr) {}

// Сеттер логгера
void Game::setLogger(Logger* newLogger) {
    logger = newLogger;
}

// Геттеры
const Field* Game::getField() const { return field.get(); }
const Player* Game::getPlayer() const { return player.get(); }
const std::vector<Enemy>& Game::getEnemies() const { return enemies; }
int Game::getCurrentLevel() const { return currentLevel; }
bool Game::isGameRunning() const { return !isGameOver; }
void Game::setGameOver(bool over) { isGameOver = over; }

// Настройка уровня
void Game::setupLevel() {
    currentLevel++;
    if (logger) logger->log("Начало уровня " + std::to_string(currentLevel), LogLevel::L_INFO);
    
    enemies.clear();

    // Генерация уровня
    if (currentLevel == 1) {
        field = std::make_unique<Field>(12, 12);
        field->generateRandomObstacles(15);
        enemies.emplace_back("Орк", field.get(), 8, 3, currentLevel);
        enemies.emplace_back("Гоблин", field.get(), 4, 9, currentLevel);
    } else {
        int fieldSize = std::min(25, 15 + (currentLevel - 1) * 2);
        field = std::make_unique<Field>(fieldSize, fieldSize);
        field->generateRandomObstacles(20);
        
        int enemyCount = 2 + currentLevel;
        for (int i = 0; i < enemyCount; ++i) {
            int ex = rand() % field->getWidth();
            int ey = rand() % field->getLength();
            if (field->getNode(ex, ey).getType() == Type::EMPTY) {
                enemies.emplace_back("Враг", field.get(), ex, ey, currentLevel);
            }
        }
    }

    // Игрок
    if (currentLevel == 1) {
        player = std::make_unique<Player>("Герой", field.get(), 1, 1, 5);
        player->addRandomSpell();
    } else {
        player->setField(field.get()); 
        player->resetPosition(1, 1);     
        player->restoreHealth();
        player->getHand().removeRandomSpells();
        if (logger) logger->log("Часть заклинаний сброшена.", LogLevel::L_WARNING);
    }
    
    // Обновление карты
    field->getNode(player->getX(), player->getY()).setType(Type::PLAYER);
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            field->getNode(enemy.getX(), enemy.getY()).setType(Type::ENEMY);
        }
    }
}

// Ход врагов
void Game::processEnemyTurn() {
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            if (std::abs(enemy.getX() - player->getX()) <= 1 && 
                std::abs(enemy.getY() - player->getY()) <= 1) {
                
                std::cout << enemy.getName() << " атакует!" << std::endl;
                player->takeDamage(enemy.getDamagePower());
                
                if (logger) logger->logAttack(enemy.getName(), player->getName(), enemy.getDamagePower());
            } else {
                enemy.moveTowards(*player);
            }
        }
    }
}

// Проверка условий
void Game::checkEndConditions() {
    auto it = std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e){ return !e.isAlive(); });
    if (it != enemies.end()) {
        enemies.erase(it, enemies.end());
        if (logger) logger->log("Враг убит.", LogLevel::L_GAME_EVENT);
    }
    
    if (!player->isAlive()) {
        if (logger) logger->log("Игрок погиб.", LogLevel::L_ERROR);
        std::cout << "Конец игры! Начать заново? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice == 'y') {
            currentLevel = 0;
            setupLevel();
        } else {
            isGameOver = true;
        }
    } else if (enemies.empty()) {
        nextLevel();
    }
}

void Game::nextLevel() {
    if (logger) logger->log("Уровень пройден!", LogLevel::L_INFO);
    std::cout << "\nУровень пройден!" << std::endl;
    processLevelUp(); 
    setupLevel();
}

void Game::processLevelUp() {
    std::cout << "--- ПРОКАЧКА ---\n1. +HP\n2. +Урон\n3. Заклинание\nВыбор: ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (choice == '1') player->upgradeMaxHealth(20);
    else if (choice == '2') player->upgradeDamage(5);
    else player->addRandomSpell();
    
    if (logger) logger->log("Игрок получил улучшение.", LogLevel::L_INFO);
}

// --- Реализация команд (возвращают string) ---

std::string Game::movePlayer(int dx, int dy) {
    if (!player || !player->isAlive()) return "Невозможно двигаться.";
    
    int oldX = player->getX();
    int oldY = player->getY();
    
    if (player->move(dx, dy)) {
        if (logger) logger->logPlayerMove(player->getName(), oldX, oldY, player->getX(), player->getY());
        return "Перемещение успешно.";
    }
    return "Путь заблокирован.";
}

std::string Game::playerAttack() {
    if (!player || !player->isAlive()) return "";
    
    for (auto& enemy : enemies) {
        if (enemy.isAlive() && player->attack(enemy)) {
            if (logger) logger->logAttack(player->getName(), enemy.getName(), player->getCombatPower());
            return "Атака успешна!";
        }
    }
    return "Враг далеко.";
}

std::string Game::switchCombatMode() {
    if (player) {
        player->switchCombatMode();
        if (logger) logger->log("Режим боя изменен.", LogLevel::L_GAME_EVENT);
        return "Смена стойки.";
    }
    return "";
}

std::string Game::useSpell(int index, int targetX, int targetY) {
    if (!player || !player->isAlive()) return "";
    
    if (!field->areCoordinatesValid(targetX, targetY)) return "Неверные координаты.";

    if (index >= 0 && index < player->getHand().getSpellCount()) {
        std::string sName = player->getHand().getSpellName(index);
        player->getHand().useSpell(index, *player, enemies, *field);
        if (logger) logger->logSpellCast(player->getName(), sName, targetX, targetY);
        return "Заклинание использовано.";
    }
    return "Нет такого заклинания.";
}

std::string Game::buySpell() {
    if (!player) return "";
    int pts = player->getPoints();
    player->buyRandomSpell();
    if (player->getPoints() < pts) {
        if (logger) logger->log("Куплено заклинание.", LogLevel::L_GAME_EVENT);
        return "Покупка успешна.";
    }
    return "Не хватает очков.";
}

// Save / Load
void Game::saveGame(const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) throw SaveGameException("Ошибка записи");
    
    ofs << currentLevel << "\n";
    field->save(ofs);
    player->save(ofs);
    
    ofs << enemies.size() << "\n";
    for (const auto& enemy : enemies) enemy.save(ofs);
    
    if (logger) logger->log("Игра сохранена.", LogLevel::L_INFO);
}

void Game::loadGame(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) throw LoadGameException("Файл не найден");
    
    ifs >> currentLevel;
    
    field = std::make_unique<Field>(10, 10);
    field->load(ifs);
    
    player = std::make_unique<Player>("", field.get(), 0, 0, 0);
    player->load(ifs, field.get());
    
    enemies.clear();
    size_t count;
    ifs >> count;
    ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    for (size_t i = 0; i < count; ++i) {
        enemies.emplace_back(ifs, field.get());
    }

    if (logger) logger->log("Игра загружена.", LogLevel::L_INFO);
}

void Game::run() {}