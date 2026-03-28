#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include <random>


GameField::GameField(int fieldWidth, int fieldHeight) {
    grid = std::unique_ptr<Grid>(new Grid(fieldWidth, fieldHeight));
    characterManager = std::unique_ptr<CharacterManager>(new CharacterManager());
}

GameField::GameField(const GameField& other) {
    grid = std::unique_ptr<Grid>(new Grid(*other.grid));
    characterManager = std::unique_ptr<CharacterManager>(new CharacterManager(*other.characterManager));
    traps = other.traps;
    towers = other.towers;
}

GameField::GameField(GameField&& other) noexcept
    : grid(std::move(other.grid)), 
      characterManager(std::move(other.characterManager)),
      traps(std::move(other.traps)),
      towers(std::move(other.towers)) {}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        grid = std::unique_ptr<Grid>(new Grid(*other.grid));
        characterManager = std::unique_ptr<CharacterManager>(new CharacterManager(*other.characterManager));
        traps = other.traps;
        towers = other.towers;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        grid = std::move(other.grid);
        characterManager = std::move(other.characterManager);
        traps = std::move(other.traps);
        towers = std::move(other.towers);
    }
    return *this;
}

bool GameField::isPositionEmpty(const Position& pos) const {
    return grid->isPositionEmpty(pos) && 
           !characterManager->hasCharacterAt(pos) && 
           !hasTrapAt(pos) && 
           !hasTowerAt(pos);
}

bool GameField::isPositionPassable(const Position& pos) const {
    return grid->isPositionPassable(pos) && !hasTrapAt(pos);
}

CellType GameField::getCellType(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return CellType::WALL;
    }
    return grid->getCellType(pos);
}

bool GameField::hasTrapAt(const Position& pos) const {
    auto it = traps.find(pos);
    return it != traps.end() && it->second.isActive();
}

void GameField::placeTrap(const Position& pos) {
    traps[pos] = Trap(pos);
}

void GameField::removeTrap(const Position& pos) {
    traps.erase(pos);
}

bool GameField::hasTowerAt(const Position& pos) const {
    for (const auto& tower : towers) {
        if (tower.getPosition() == pos) {
            return true;
        }
    }
    return false;
}

void GameField::addTower(const EnemyTower& tower) {
    towers.push_back(tower);
}

void GameField::updateTowers() {
    for (auto& tower : towers) {
        tower.updateCooldown();
    }
}

bool GameField::movePlayer(const Position& newPosition) {
    if (!isValidPosition(newPosition) || !isPositionPassable(newPosition)) {
        return false;
    }
    
    Position currentPos = characterManager->getPlayerPosition();
    
    if (!characterManager->moveCharacter(currentPos, newPosition)) {
        return false;
    }
    
    updateCell(currentPos, CellType::EMPTY);
    characterManager->setPlayerPosition(newPosition);
    updateCell(newPosition, CellType::PLAYER);
    
    return true;
}

void GameField::addCharacter(const Position& position, std::unique_ptr<Character> character) {
    if (isValidPosition(position) && isPositionEmpty(position)) {
        characterManager->addCharacter(position, std::move(character));
        Player* playerChar = dynamic_cast<Player*>(characterManager->getCharacterAt(position));
        if (playerChar) {
            updateCell(position, CellType::PLAYER);
            characterManager->setPlayerPosition(position);
        } else {
            updateCell(position, CellType::ENEMY);
        }
    }
}


bool GameField::moveCharacter(const Position& from, const Position& to) {
    if (!isValidPosition(from) || !isValidPosition(to) || !grid->isPositionPassable(to)) {
        return false;
    }
    
    Character* character = characterManager->getCharacterAt(from);
    if (!character) {
        return false;
    }
    
    if (hasTrapAt(to)) {
        auto trapIt = traps.find(to);
        if (trapIt != traps.end()) {
            Trap& trap = trapIt->second;
            
            Enemy* enemy = dynamic_cast<Enemy*>(character);
            if (enemy) {
                enemy->takeDamage(trap.getDamage());
                trap.trigger();
                
                traps.erase(trapIt);
                
                if (!enemy->isAlive()) {
                    removeCharacter(from);
                    return false;
                }
            } else {
                return false;
            }
        }
    }
    
    if (hasCharacterAt(to) || hasTowerAt(to)) {
        return false;
    }
    
    if (!characterManager->moveCharacter(from, to)) {
        return false;
    }
    
    updateCell(from, CellType::EMPTY);
    
    Player* playerChar = dynamic_cast<Player*>(characterManager->getCharacterAt(to));
    if (playerChar) {
        updateCell(to, CellType::PLAYER);
        characterManager->setPlayerPosition(to);
    } else {
        updateCell(to, CellType::ENEMY);
    }
    
    return true;
}
void GameField::removeCharacter(const Position& pos) {
    characterManager->removeCharacter(pos);
    updateCell(pos, CellType::EMPTY);
}

Position GameField::findRandomEmptyPosition() const {
    return grid->findRandomEmptyPosition();
}