#pragma once
#include <string>

enum class GameEventType {
    DamageDealt,
    PlayerMoved,
    SpellReceived,
    EnemySpawned,
    EnemyDied
};

struct GameEvent {
    GameEventType type;
    std::string source;   
    std::string target;   
    int amount = 0;       
    int x = -1, y = -1;   
    std::string extra;
};
