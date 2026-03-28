#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <random>
#include <ctime>

namespace BoardConfig
{
    inline constexpr int Size{5};
    inline constexpr int CellPixels{60};
    inline constexpr std::pair<int,int> HealthPointPos{5, 0};
    inline constexpr int cellHealAmount{20};

}

namespace CharacterConfig
{
    inline constexpr int Health{100};
    inline constexpr int Mana{2000};
    inline constexpr int MeleeDamage{10};
    inline constexpr int RangedDamage{5};
    inline constexpr int MeleeAttackPoints{10};
    inline constexpr int RangedAttackPoints{5};
}

namespace CostCardsConfig
{
    inline constexpr int FireBall{10};
    inline constexpr int EarthQuake{15};
    inline constexpr int PoisonTrap{20};
    inline constexpr int SummonArcher{25};
    inline constexpr int Upgrade{30};
}

namespace SpellConfig
{
    inline constexpr int FireBallSpellRadius{5};
    inline constexpr int FireBallMaxRadius{20};
    inline constexpr int FireBallDamage{20};
    inline constexpr int FireBallArea{1};
    inline constexpr int EarthQuakeSpellRadius{5};
    inline constexpr int EarthQuakeArea{2};
    inline constexpr int EarthQuakeMaxArea{2};
    inline constexpr int EarthQuakeDamage{30};
    inline constexpr int PoisonTrapDamage{50};
    inline constexpr int PoisonTrapMaxDamage{100};
    inline constexpr int SummonArcherSpellRadius{5};
    inline constexpr int SummonArchersCount{1};
    inline constexpr int SummonArchersMaxCount{3};
    inline constexpr int MaxUpgradeLevel{10};
}

namespace BuildingsConfig
{
    inline constexpr int SpawnInterval{1000};
    inline constexpr int TowerDamageRadius{3};
    inline constexpr int TowerAttackInterval{3};
}

namespace MyRandom
{
    inline std::mt19937 Mersenne{static_cast<std::mt19937::result_type>(std::time(nullptr))};
}

enum class CardType
{
    FIREBALL,
    EARTHQUAKE,
    POISON_TRAP,
    SUMMON_ARCHER,
    UPGRADE_CARD,
    NONE
};

enum class CommandType
{
    PLAYER_MOVE,
    PLAYER_ATTACK,
    USE_CARD,
    BUY_CARD,
    UPGRADE_CARD,
    CHANGE_TYPE_BATTLE,
    SAVE_GAME,
    UPGRADE_HEALTH

};

enum class GameState
{
    PLAYING_STATE,
    LEVEL_COMPLETE,
    INIT_STATE,
    LEVEL_FAILED
};

enum class Actor
{
    PLAYER,
    ENEMY,
    ARCHER,
    DARK_TOWER,
    BUILDING
};

enum class CellType
{
    SLOWED,
    BLOCKED,
    POISON_TRAP,
    HEALTH_POINT,
    ORIGINAL
};

enum class Bar
{
    FIREBALL,
    EARTHQUAKE,
    POISON_TRAP,
    SUMMON_ARCHER,
    UPGRADE_CARD,
    HEALTH_PLAYER,
    HEALTH_TOWER,
    MANA
};

#endif //CONSTANTS_H
