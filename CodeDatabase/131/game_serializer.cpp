#include "game_serializer.h"
#include "game_exceptions.h"

namespace rpg {

void GameSerializer::Serialize(const GameMemento& memento, std::fstream& file) const {
    WriteInt(file, kMagicNumber);
    WriteInt(file, kVersionNumber);
    
    WriteInt(file, memento.turn_number);
    WriteInt(file, memento.current_level);
    WriteSize(file, memento.field_width);
    WriteSize(file, memento.field_height);
    
    WriteInt(file, memento.player_health);
    WriteInt(file, memento.player_max_health);
    WriteInt(file, memento.player_melee_damage);
    WriteInt(file, memento.player_ranged_damage);
    WriteInt(file, memento.player_score);
    WritePosition(file, memento.player_position);
    WriteInt(file, static_cast<int>(memento.player_combat_mode));
    WriteInt(file, memento.player_slowed_turns);
    
    WriteSize(file, memento.spell_types.size());
    for (int spell_type : memento.spell_types) {
        WriteInt(file, spell_type);
    }
    
    WriteSize(file, memento.field_impassable_cells.size());
    for (const auto& [x, y] : memento.field_impassable_cells) {
        WriteSize(file, x);
        WriteSize(file, y);
    }
    
    WriteSize(file, memento.field_slowing_cells.size());
    for (const auto& [x, y] : memento.field_slowing_cells) {
        WriteSize(file, x);
        WriteSize(file, y);
    }
    
    WriteSize(file, memento.enemies.size());
    for (const auto& enemy : memento.enemies) {
        SerializeEnemy(file, enemy);
    }
    
    WriteSize(file, memento.buildings.size());
    for (const auto& building : memento.buildings) {
        SerializeBuilding(file, building);
    }
    
    WriteSize(file, memento.traps.size());
    for (const auto& trap : memento.traps) {
        SerializeTrap(file, trap);
    }
    
    WriteSize(file, memento.allies.size());
    for (const auto& ally : memento.allies) {
        SerializeAlly(file, ally);
    }
    
    WriteSize(file, memento.towers.size());
    for (const auto& tower : memento.towers) {
        SerializeTower(file, tower);
    }
}

GameMemento GameSerializer::Deserialize(std::fstream& file) const {
    int magic = ReadInt(file);
    if (magic != kMagicNumber) {
        throw FileCorruptedException("savegame", "неверный формат файла");
    }
    
    int version = ReadInt(file);
    if (version != kVersionNumber) {
        throw FileCorruptedException("savegame", 
            "несовместимая версия (ожидается " + std::to_string(kVersionNumber) + 
            ", получено " + std::to_string(version) + ")");
    }
    
    GameMemento memento;
    
    memento.turn_number = ReadInt(file);
    memento.current_level = ReadInt(file);
    memento.field_width = ReadSize(file);
    memento.field_height = ReadSize(file);
    
    memento.player_health = ReadInt(file);
    memento.player_max_health = ReadInt(file);
    memento.player_melee_damage = ReadInt(file);
    memento.player_ranged_damage = ReadInt(file);
    memento.player_score = ReadInt(file);
    memento.player_position = ReadPosition(file);
    memento.player_combat_mode = static_cast<CombatMode>(ReadInt(file));
    memento.player_slowed_turns = ReadInt(file);
    
    size_t spell_count = ReadSize(file);
    memento.spell_types.reserve(spell_count);
    for (size_t i = 0; i < spell_count; ++i) {
        memento.spell_types.push_back(ReadInt(file));
    }
    
    size_t impassable_count = ReadSize(file);
    memento.field_impassable_cells.reserve(impassable_count);
    for (size_t i = 0; i < impassable_count; ++i) {
        size_t x = ReadSize(file);
        size_t y = ReadSize(file);
        memento.field_impassable_cells.emplace_back(x, y);
    }
    
    size_t slowing_count = ReadSize(file);
    memento.field_slowing_cells.reserve(slowing_count);
    for (size_t i = 0; i < slowing_count; ++i) {
        size_t x = ReadSize(file);
        size_t y = ReadSize(file);
        memento.field_slowing_cells.emplace_back(x, y);
    }
    
    size_t enemy_count = ReadSize(file);
    memento.enemies.reserve(enemy_count);
    for (size_t i = 0; i < enemy_count; ++i) {
        memento.enemies.push_back(DeserializeEnemy(file));
    }
    
    size_t building_count = ReadSize(file);
    memento.buildings.reserve(building_count);
    for (size_t i = 0; i < building_count; ++i) {
        memento.buildings.push_back(DeserializeBuilding(file));
    }
    
    size_t trap_count = ReadSize(file);
    memento.traps.reserve(trap_count);
    for (size_t i = 0; i < trap_count; ++i) {
        memento.traps.push_back(DeserializeTrap(file));
    }
    
    size_t ally_count = ReadSize(file);
    memento.allies.reserve(ally_count);
    for (size_t i = 0; i < ally_count; ++i) {
        memento.allies.push_back(DeserializeAlly(file));
    }
    
    size_t tower_count = ReadSize(file);
    memento.towers.reserve(tower_count);
    for (size_t i = 0; i < tower_count; ++i) {
        memento.towers.push_back(DeserializeTower(file));
    }
    
    return memento;
}


void GameSerializer::WriteInt(std::fstream& file, int value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void GameSerializer::WriteSize(std::fstream& file, size_t value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

void GameSerializer::WritePosition(std::fstream& file, const Position& pos) {
    WriteSize(file, pos.x);
    WriteSize(file, pos.y);
}

int GameSerializer::ReadInt(std::fstream& file) {
    int value;
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (file.fail()) {
        throw FileCorruptedException("savegame", "ошибка чтения целого числа");
    }
    return value;
}

size_t GameSerializer::ReadSize(std::fstream& file) {
    size_t value;
    file.read(reinterpret_cast<char*>(&value), sizeof(value));
    if (file.fail()) {
        throw FileCorruptedException("savegame", "ошибка чтения размера");
    }
    return value;
}

Position GameSerializer::ReadPosition(std::fstream& file) {
    size_t x = ReadSize(file);
    size_t y = ReadSize(file);
    return Position(x, y);
}


void GameSerializer::SerializeEnemy(std::fstream& file, const Enemy& enemy) {
    WriteInt(file, enemy.GetHealth());
    WriteInt(file, enemy.GetDamage());
    WritePosition(file, enemy.GetPosition());
}

void GameSerializer::SerializeBuilding(std::fstream& file, const EnemyBuilding& building) {
    WritePosition(file, building.GetPosition());
    WriteInt(file, building.GetSpawnInterval());
    WriteInt(file, building.GetEnemyHealth());
    WriteInt(file, building.GetEnemyDamage());
    WriteInt(file, building.GetTurnsUntilSpawn());
}

void GameSerializer::SerializeTrap(std::fstream& file, const Trap& trap) {
    WritePosition(file, trap.GetPosition());
    WriteInt(file, trap.GetDamage());
}

void GameSerializer::SerializeAlly(std::fstream& file, const Ally& ally) {
    WriteInt(file, ally.GetHealth());
    WriteInt(file, ally.GetDamage());
    WritePosition(file, ally.GetPosition());
}

void GameSerializer::SerializeTower(std::fstream& file, const EnemyTower& tower) {
    WritePosition(file, tower.GetPosition());
    WriteInt(file, tower.GetCooldown());
    WriteInt(file, tower.GetTurnsUntilAttack());
}

Enemy GameSerializer::DeserializeEnemy(std::fstream& file) {
    int health = ReadInt(file);
    int damage = ReadInt(file);
    Position pos = ReadPosition(file);
    return Enemy(health, damage, pos);
}

EnemyBuilding GameSerializer::DeserializeBuilding(std::fstream& file) {
    Position pos = ReadPosition(file);
    int spawn_interval = ReadInt(file);
    int enemy_health = ReadInt(file);
    int enemy_damage = ReadInt(file);
    int turns_until_spawn = ReadInt(file);
    return EnemyBuilding::FromSave(pos, spawn_interval, enemy_health, enemy_damage, turns_until_spawn);
}

Trap GameSerializer::DeserializeTrap(std::fstream& file) {
    Position pos = ReadPosition(file);
    int damage = ReadInt(file);
    return Trap(pos, damage);
}

Ally GameSerializer::DeserializeAlly(std::fstream& file) {
    int health = ReadInt(file);
    int damage = ReadInt(file);
    Position pos = ReadPosition(file);
    return Ally(health, damage, pos);
}

EnemyTower GameSerializer::DeserializeTower(std::fstream& file) {
    Position pos = ReadPosition(file);
    int cooldown = ReadInt(file);
    int turns_until_attack = ReadInt(file);
    return EnemyTower::FromSave(pos, cooldown, turns_until_attack);
}

}

