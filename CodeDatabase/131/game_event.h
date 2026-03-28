#ifndef OOP_GAME_EVENT_H_
#define OOP_GAME_EVENT_H_

#include <string>
#include <map>

namespace rpg {
    enum class GameEventType {
        kPlayerMoved,
        kPlayerDamaged,
        kEnemyDamaged,
        kEnemyKilled,
        kAllyDamaged,
        kAllyKilled,
        kSpellCast,
        kSpellBought,
        kTrapActivated,
        kBuildingSpawned,
        kTowerAttacked,
        kCombatModeChanged,
        kLevelCompleted,
        kGameOver,
        kGameSaved,
        kGameLoaded
    };

    struct GameEvent {
        GameEventType type;
        std::string description;
        std::map<std::string, std::string> data;

        GameEvent(GameEventType t, const std::string& desc)
            : type(t), description(desc) {}

        GameEvent(GameEventType t, const std::string& desc, 
                  const std::map<std::string, std::string>& d)
            : type(t), description(desc), data(d) {}
    };

    class IEventListener {
    public:
        virtual ~IEventListener() = default;
        virtual void OnEvent(const GameEvent& event) = 0;
    };
}

#endif

