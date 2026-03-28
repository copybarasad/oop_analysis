#include "EnemyTower.h"
#include "Game.h"
#include "Player.h"
#include "GameField.h"
#include <cmath>
#include <sstream>
#include <string>

EnemyTower::EnemyTower(int startX, int startY, int attackRadius, 
                       int attackDamage, int attackCooldown, int buildingId)
    : EnemyBuilding(startX, startY, 999, buildingId), // Башни не спавнят врагов
      attackRadius(attackRadius), attackDamage(attackDamage), 
      attackCooldown(attackCooldown), currentCooldown(0), canAttackThisTurn(true) {}

int EnemyTower::getAttackRadius() const {
    return attackRadius;
}

int EnemyTower::getAttackDamage() const {
    return attackDamage;
}

bool EnemyTower::canAttack() const {
    return currentCooldown <= 0 && canAttackThisTurn;
}

bool EnemyTower::tryAttackPlayer(Game& game, const Player& player) {
    if (!canAttack()) {
        return false;
    }
    
    int playerX = player.getPositionX();
    int playerY = player.getPositionY();
    
    // Вычисляем расстояние до игрока
    int dx = playerX - getPositionX();
    int dy = playerY - getPositionY();
    double distance = std::sqrt(dx * dx + dy * dy);
    
    if (distance > attackRadius) {
        return false;
    }
    
    // Атакуем игрока ослабленной версией заклинания прямого урона
    const_cast<Player&>(player).takeDamage(attackDamage);
    
    // Устанавливаем кулдаун
    currentCooldown = attackCooldown;
    canAttackThisTurn = false;
    
    return true;
}

void EnemyTower::update() {
    EnemyBuilding::updateCooldown();
    if (currentCooldown > 0) {
        currentCooldown--;
    }
    canAttackThisTurn = true; // Разрешаем атаку на следующий ход
}

bool EnemyTower::trySpawnEnemy(GameField& field, std::vector<Enemy>& enemies) {
    // Башни не спавнят врагов
    return false;
}

std::string EnemyTower::serialize() const {
    std::ostringstream oss;
    // Сначала сериализуем базовый класс
    std::string baseData = EnemyBuilding::serialize();
    oss << baseData;
    
    // Затем добавляем данные башни
    // Формат: attackRadius:attackDamage:attackCooldown:currentCooldown:canAttackThisTurn
    oss << "[TowerData]\n";
    oss << "attackRadius=" << attackRadius << "\n";
    oss << "attackDamage=" << attackDamage << "\n";
    oss << "attackCooldown=" << attackCooldown << "\n";
    oss << "currentCooldown=" << currentCooldown << "\n";
    oss << "canAttackThisTurn=" << (canAttackThisTurn ? 1 : 0) << "\n";
    
    return oss.str();
}

bool EnemyTower::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    std::string baseData;
    bool inTowerSection = false;
    
    // Читаем данные базового класса
    while (std::getline(iss, line)) {
        if (line.empty()) continue;
        
        if (line == "[TowerData]") {
            inTowerSection = true;
            break;
        }
        
        baseData += line + "\n";
    }
    
    // Десериализуем базовый класс
    if (!EnemyBuilding::deserialize(baseData)) {
        return false;
    }
    
    // Читаем данные башни
    if (inTowerSection) {
        while (std::getline(iss, line)) {
            if (line.empty()) continue;
            
            size_t pos = line.find('=');
            if (pos == std::string::npos) continue;
            
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            if (key == "attackRadius") {
                attackRadius = std::stoi(value);
            } else if (key == "attackDamage") {
                attackDamage = std::stoi(value);
            } else if (key == "attackCooldown") {
                attackCooldown = std::stoi(value);
            } else if (key == "currentCooldown") {
                currentCooldown = std::stoi(value);
            } else if (key == "canAttackThisTurn") {
                canAttackThisTurn = (std::stoi(value) == 1);
            }
        }
    }
    
    return true;
}

