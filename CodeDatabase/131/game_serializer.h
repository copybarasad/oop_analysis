#ifndef OOP_GAME_SERIALIZER_H_
#define OOP_GAME_SERIALIZER_H_

#include "game_memento.h"
#include <fstream>

namespace rpg {

class GameSerializer final {
public:
    GameSerializer() = default;
    
    void Serialize(const GameMemento& memento, std::fstream& file) const;
    
    [[nodiscard]] GameMemento Deserialize(std::fstream& file) const;

private:
    static void WriteInt(std::fstream& file, int value);
    static void WriteSize(std::fstream& file, size_t value);
    static void WritePosition(std::fstream& file, const Position& pos);
    
    static int ReadInt(std::fstream& file);
    static size_t ReadSize(std::fstream& file);
    static Position ReadPosition(std::fstream& file);

    static void SerializeEnemy(std::fstream& file, const Enemy& enemy);
    static void SerializeBuilding(std::fstream& file, const EnemyBuilding& building);
    static void SerializeTrap(std::fstream& file, const Trap& trap);
    static void SerializeAlly(std::fstream& file, const Ally& ally);
    static void SerializeTower(std::fstream& file, const EnemyTower& tower);
    
    static Enemy DeserializeEnemy(std::fstream& file);
    static EnemyBuilding DeserializeBuilding(std::fstream& file);
    static Trap DeserializeTrap(std::fstream& file);
    static Ally DeserializeAlly(std::fstream& file);
    static EnemyTower DeserializeTower(std::fstream& file);

    // для валидации
    static constexpr int kMagicNumber = 0x52504700;  // "RPG\0"
    static constexpr int kVersionNumber = 2;  // Версия 2
};

}

#endif

