#ifndef CHARACTER_MANAGER_H
#define CHARACTER_MANAGER_H

#include "Character.h"
#include "Position.h"
#include <unordered_map>
#include <memory>
#include <vector>

class CharacterManager {
private:
    std::unordered_map<Position, std::unique_ptr<Character>> characters;
    Position playerPosition;

public:
    CharacterManager() = default;
    
    CharacterManager(const CharacterManager& other);
    CharacterManager(CharacterManager&& other) noexcept;
    CharacterManager& operator=(const CharacterManager& other);
    CharacterManager& operator=(CharacterManager&& other) noexcept;
    ~CharacterManager() = default;
    
    // Методы доступа
    bool hasCharacterAt(const Position& pos) const;
    const Character* viewCharacterAt(const Position& pos) const;
    Character* getCharacterAt(const Position& pos);
    std::vector<Position> getAllCharacterPositions() const;
    Position getPlayerPosition() const { return playerPosition; }
    
    // Методы модификации
    void setPlayerPosition(const Position& pos) { playerPosition = pos; }
    void addCharacter(const Position& pos, std::unique_ptr<Character> character);
    void removeCharacter(const Position& pos);
    bool moveCharacter(const Position& from, const Position& to);
    
    // Для получения персонажа определенного типа
    template<typename T>
    T* getCharacterAs(const Position& pos) {
        auto it = characters.find(pos);
        if (it != characters.end()) {
            return dynamic_cast<T*>(it->second.get());
        }
        return nullptr;
    }
};

#endif