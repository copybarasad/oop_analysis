#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Grid.h"
#include "CharacterManager.h"
#include "Trap.h"
#include "EnemyTower.h"
#include <memory>
#include <vector>
#include <unordered_map>

class GameField {
private:
    std::unique_ptr<Grid> grid;
    std::unique_ptr<CharacterManager> characterManager;
    std::unordered_map<Position, Trap> traps;
    std::vector<EnemyTower> towers;

public:
    GameField(int fieldWidth, int fieldHeight);
    
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    ~GameField() = default;
    
    int getWidth() const { return grid->getWidth(); }
    int getHeight() const { return grid->getHeight(); }
    Position getPlayerPosition() const { return characterManager->getPlayerPosition(); }
    
    bool isValidPosition(const Position& pos) const { return grid->isValidPosition(pos); }
    bool isPositionEmpty(const Position& pos) const;
    bool isPositionPassable(const Position& pos) const;
    
    CellType getCellType(const Position& pos) const;
    bool hasCharacterAt(const Position& pos) const { return characterManager->hasCharacterAt(pos); }
    const Character* viewCharacterAt(const Position& pos) const { return characterManager->viewCharacterAt(pos); }
    Character* getCharacterAt(const Position& pos) { return characterManager->getCharacterAt(pos); }
    
    // Trap methods
    bool hasTrapAt(const Position& pos) const;
    void placeTrap(const Position& pos);
    void removeTrap(const Position& pos);
    const std::unordered_map<Position, Trap>& getTraps() const { return traps; }
    
    // Tower methods
    bool hasTowerAt(const Position& pos) const;
    void addTower(const EnemyTower& tower);
    void removeTower(const Position& pos) {
        for (auto it = towers.begin(); it != towers.end(); ++it) {
            if (it->getPosition() == pos) {
                towers.erase(it);
                break;
            }
        }
    }
    const std::vector<EnemyTower>& getTowers() const { return towers; }
    void updateTowers();
    
    // Player movement
    bool movePlayer(const Position& newPosition);
    void addCharacter(const Position& position, std::unique_ptr<Character> character);
    bool moveCharacter(const Position& from, const Position& to);
    void updateCell(const Position& pos, CellType cellType) { grid->setCellType(pos, cellType); }
    
    std::vector<Position> getAllCharacterPositions() const { return characterManager->getAllCharacterPositions(); }
    void removeCharacter(const Position& pos);
    
    Position findRandomEmptyPosition() const;
    
    // Для получения персонажа определенного типа
    template<typename T>
    T* getCharacterAs(const Position& pos) {
        return characterManager->getCharacterAs<T>(pos);
    }
};

#endif