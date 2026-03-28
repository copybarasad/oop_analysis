#ifndef TYPES
#define TYPES

#include "Serialization.h"
#include "Command.h"
// #include "Game.h"  // Убираем циклическое включение

class Game;  // Forward declaration

enum class AppState {
    MainMenu,
    InGame,
    Exit
};

enum class AttackMode {
    Melee,
    Ranged
};
struct Coords {
    int x;
    int y;
};
enum class Turn {
    Player,
    Enemies
};
enum class PlayerTurn {
    ATTACK,
    SWITCH_MODE,
    MOVE
};
enum class PlayerActionMode {
    Normal,
    Targeting,
    SplashTargeting,
    TrapTargeting
};

enum class SpellType {
    Damage,
    Splash,
    Trap
};

struct CastInfo {
    SpellType type;
    int damage = 0;
    int distance = 0;
    int radius = 0;
    Coords pos = { -1, -1 };
};

struct Area {
    int lx, ly;
    int rx, ry;
    bool contains(const Coords& c) const {
        return c.x >= lx && c.x <= rx && c.y >= ly && c.y <= ry;
    }
};

struct Trap {
    Coords pos;
    int damage;

    json serialize() const {
        return json{ {"x", pos.x}, {"y", pos.y}, {"damage", damage} };
    }

    void deserialize(const json& j) {
        pos.x = j.at("x").get<int>();
        pos.y = j.at("y").get<int>();
        damage = j.at("damage").get<int>();
    }
};


struct LevelConfig {
    int numEnemies;
    int numTowers;
    int width;
    int height;
};

class IInputReader {
public:
    virtual Command poll() = 0;
};

class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void render(const Game& game) = 0;
};

// const
const int PLAYER_MELEE_DAMAGE = 5;
const int PLAYER_RANGED_DAMAGE = 10;
const int PLAYER_HAND_SIZE = 5;
const AppState DEFAULT_APP_STATE = AppState::MainMenu;
const int DEFAULT_LEVEL_INDEX = 0;
const int DEFAULT_ENEMY_HP = 10;
const int DEFAULT_ENEMY_DAMAGE = 3;
const int DEFAULT_TOWER_RANGE = 4;
const int DEFAULT_TOWER_SKIP = 3;
const int STARTING_MOVE = 0;
const int PLAYER_MAX_HEALTH = 30;


#endif // TYPES

