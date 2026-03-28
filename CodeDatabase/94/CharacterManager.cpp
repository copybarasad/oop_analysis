#include "CharacterManager.h"

// ============================================================================
// РЕАЛИЗАЦИЯ КЛАССА CHARACTERMANAGER (МЕНЕДЖЕР ПЕРСОНАЖЕЙ)
// ============================================================================

// Конструктор копирования
CharacterManager::CharacterManager(const CharacterManager& other) : playerPosition(other.playerPosition) {
    // Глубокое копирование всех персонажей с помощью метода clone()
    for (const auto& pair : other.characters) {
        characters[pair.first].reset(pair.second->clone());
    }
}

// Конструктор перемещения
CharacterManager::CharacterManager(CharacterManager&& other) noexcept
    : characters(std::move(other.characters)), playerPosition(std::move(other.playerPosition)) {}

// Оператор присваивания копированием
CharacterManager& CharacterManager::operator=(const CharacterManager& other) {
    if (this != &other) {
        // Очищаем текущие данные
        characters.clear();
        playerPosition = other.playerPosition;
        // Копируем персонажей
        for (const auto& pair : other.characters) {
            characters[pair.first].reset(pair.second->clone());
        }
    }
    return *this;
}

// Оператор присваивания перемещением
CharacterManager& CharacterManager::operator=(CharacterManager&& other) noexcept {
    if (this != &other) {
        // Перемещаем данные
        characters = std::move(other.characters);
        playerPosition = std::move(other.playerPosition);
    }
    return *this;
}

// Проверка наличия персонажа в позиции
bool CharacterManager::hasCharacterAt(const Position& pos) const {
    return characters.find(pos) != characters.end();
}

// Получение константного указателя на персонажа (только для чтения)
const Character* CharacterManager::viewCharacterAt(const Position& pos) const {
    auto it = characters.find(pos);
    return (it != characters.end()) ? it->second.get() : nullptr;
}

// Получение указателя на персонажа для модификации
Character* CharacterManager::getCharacterAt(const Position& pos) {
    auto it = characters.find(pos);
    return (it != characters.end()) ? it->second.get() : nullptr;
}

// Получение всех позиций с персонажами
std::vector<Position> CharacterManager::getAllCharacterPositions() const {
    std::vector<Position> positions;
    for (const auto& pair : characters) {
        positions.push_back(pair.first);
    }
    return positions;
}

// Добавление персонажа на поле
void CharacterManager::addCharacter(const Position& pos, std::unique_ptr<Character> character) {
    characters[pos] = std::move(character);
}

// Удаление персонажа с поля
void CharacterManager::removeCharacter(const Position& pos) {
    characters.erase(pos);
}

// Перемещение персонажа между позициями
bool CharacterManager::moveCharacter(const Position& from, const Position& to) {
    auto it = characters.find(from);
    if (it == characters.end()) {
        return false; // Персонаж не найден в исходной позиции
    }
    
    // Проверяем что целевая позиция не занята
    if (characters.find(to) != characters.end()) {
        return false;
    }
    
    // Перемещаем персонажа
    std::unique_ptr<Character> character = std::move(it->second);
    characters.erase(it);
    characters[to] = std::move(character);
    return true;
}