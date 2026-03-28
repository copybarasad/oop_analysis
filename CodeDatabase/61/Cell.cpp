#include "Cell.h"
#include <sstream>
#include <string>

Cell::Cell() : type(CellType::EMPTY), hasPlayer(false), hasEnemy(false), 
               hasBuilding(false), hasAlly(false), hasTrap(false), trapDamage(0) {}

Cell::Cell(CellType cellType) : type(cellType), hasPlayer(false), hasEnemy(false), 
                                hasBuilding(false), hasAlly(false), hasTrap(false), trapDamage(0) {}

CellType Cell::getType() const {
    return type;
}

bool Cell::hasPlayerOnCell() const {
    return hasPlayer;
}

bool Cell::hasEnemyOnCell() const {
    return hasEnemy;
}

bool Cell::hasBuildingOnCell() const {
    return hasBuilding;
}

bool Cell::hasAllyOnCell() const {
    return hasAlly;
}

bool Cell::hasTrapOnCell() const {
    return hasTrap;
}

bool Cell::isEmpty() const {
    return !hasPlayer && !hasEnemy && !hasBuilding && !hasAlly;
}

int Cell::getTrapDamage() const {
    return trapDamage;
}

bool Cell::isPassable() const {
    return type != CellType::OBSTACLE;
}

void Cell::setType(CellType cellType) {
    type = cellType;
}

void Cell::setPlayerPresence(bool presence) {
    hasPlayer = presence;
}

void Cell::setEnemyPresence(bool presence) {
    hasEnemy = presence;
}

void Cell::setBuildingPresence(bool presence) {
    hasBuilding = presence;
}

void Cell::setAllyPresence(bool presence) {
    hasAlly = presence;
}

void Cell::setTrap(bool trap, int damage) {
    hasTrap = trap;
    trapDamage = trap ? damage : 0;
}

void Cell::clearOccupants() {
    hasPlayer = false;
    hasEnemy = false;
    hasBuilding = false;
    hasAlly = false;
}

void Cell::clearTrap() {
    hasTrap = false;
    trapDamage = 0;
}

std::string Cell::serialize() const {
    std::ostringstream oss;
    // Формат: type:hasPlayer:hasEnemy:hasBuilding:hasAlly:hasTrap:trapDamage
    // НЕ добавляем \n в конце, так как это будет добавлено при записи в файл
    int typeValue = static_cast<int>(type);
    oss << typeValue << ":" 
        << (hasPlayer ? 1 : 0) << ":" 
        << (hasEnemy ? 1 : 0) << ":" 
        << (hasBuilding ? 1 : 0) << ":" 
        << (hasAlly ? 1 : 0) << ":" 
        << (hasTrap ? 1 : 0) << ":" 
        << trapDamage;
    return oss.str();
}

bool Cell::deserialize(const std::string& data) {
    if (data.empty()) {
        return false;
    }
    
    // Удаляем пробельные символы из начала и конца строки
    std::string cleanData = data;
    while (!cleanData.empty() && (cleanData.front() == ' ' || cleanData.front() == '\t')) {
        cleanData.erase(0, 1);
    }
    while (!cleanData.empty() && (cleanData.back() == ' ' || cleanData.back() == '\t' || 
                                  cleanData.back() == '\n' || cleanData.back() == '\r')) {
        cleanData.pop_back();
    }
    
    if (cleanData.empty()) {
        return false;
    }
    
    std::istringstream iss(cleanData);
    std::string token;
    
    // Читаем type (первое значение до ':')
    if (!std::getline(iss, token, ':')) return false;
    if (token.empty()) return false;
    
    // Удаляем пробельные символы из токена
    while (!token.empty() && (token.front() == ' ' || token.front() == '\t')) {
        token.erase(0, 1);
    }
    while (!token.empty() && (token.back() == ' ' || token.back() == '\t')) {
        token.pop_back();
    }
    
    if (token.empty()) return false;
    
    int typeValue = std::stoi(token);
    type = static_cast<CellType>(typeValue);
    
    // Читаем hasPlayer
    if (!std::getline(iss, token, ':')) {
        // Если не удалось прочитать, устанавливаем значения по умолчанию
        hasPlayer = false;
        hasEnemy = false;
        hasBuilding = false;
        hasAlly = false;
        hasTrap = false;
        trapDamage = 0;
        return true; // Тип клетки уже установлен, это нормально
    }
    if (token.empty()) {
        hasPlayer = false;
    } else {
        hasPlayer = (std::stoi(token) == 1);
    }
    
    // Читаем hasEnemy
    if (!std::getline(iss, token, ':')) {
        hasEnemy = false;
        hasBuilding = false;
        hasAlly = false;
        hasTrap = false;
        trapDamage = 0;
        return true; // Тип клетки уже установлен
    }
    if (token.empty()) {
        hasEnemy = false;
    } else {
        hasEnemy = (std::stoi(token) == 1);
    }
    
    // Читаем hasBuilding
    if (!std::getline(iss, token, ':')) {
        hasBuilding = false;
        hasAlly = false;
        hasTrap = false;
        trapDamage = 0;
        return true; // Тип клетки уже установлен
    }
    if (token.empty()) {
        hasBuilding = false;
    } else {
        hasBuilding = (std::stoi(token) == 1);
    }
    
    // Читаем hasAlly
    if (!std::getline(iss, token, ':')) {
        hasAlly = false;
        hasTrap = false;
        trapDamage = 0;
        return true; // Тип клетки уже установлен
    }
    if (token.empty()) {
        hasAlly = false;
    } else {
        hasAlly = (std::stoi(token) == 1);
    }
    
    // Читаем hasTrap
    if (!std::getline(iss, token, ':')) {
        hasTrap = false;
        trapDamage = 0;
        return true; // Тип клетки уже установлен
    }
    if (token.empty()) {
        hasTrap = false;
    } else {
        hasTrap = (std::stoi(token) == 1);
    }
    
    // Читаем trapDamage (последнее значение после последнего ':')
    // После всех getline с ':' остаток строки содержит trapDamage
    // Используем getline без разделителя, чтобы прочитать остаток строки
    std::string remaining;
    std::getline(iss, remaining);
    
    // Удаляем пробельные символы из конца
    while (!remaining.empty() && (remaining.back() == '\n' || remaining.back() == '\r' || 
                                  remaining.back() == ' ' || remaining.back() == '\t')) {
        remaining.pop_back();
    }
    
    if (!remaining.empty()) {
        trapDamage = std::stoi(remaining);
    } else {
        trapDamage = 0;
    }
    
    return true;
}

