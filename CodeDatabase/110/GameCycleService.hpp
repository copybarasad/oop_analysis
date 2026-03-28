#ifndef GAME_CYCLE_S_H
#define GAME_CYCLE_S_H

#include "../../GameBlock/GameLevel/GameLevel.hpp"

enum class GameProgress {
    PLAYER_DEAD,
    LEVEL_COMPLETED,
    ON_GOING
};

class GameCycleService {
    public:
    struct GameState {
        GameProgress progress;
        size_t currentLevel;
        const Field& field;
        const SpellHand& hand;
        EntityManager& manager;
        const SpellStore& store;

        GameState(size_t l, const Field& f, SpellHand& h, EntityManager& m, const SpellStore& s) : progress(GameProgress::ON_GOING), currentLevel(l),
        field(f), hand(h), manager(m), store(s) {}
    };

    private:
    std::map<size_t, GameLevel*> levels;
    std::map<size_t, std::vector<Item*>> boosts;
    size_t currentLevel;
    GameLevel* level;
    size_t boost;

    public:
    GameCycleService (std::map<size_t, GameLevel*> levels, std::map<size_t, std::vector<Item*>> boosts, size_t currentLevel);

    GameState runTurn ();
    GameState getState ();
    bool isPlayerTurn ();
    void setPlayerAction (const std::string& action, ChangeableFields* changeableFields);
    bool allLevelsDone ();
    void furtherLevel ();
    void setBoost (size_t choice);

    private:
    void produceBoost (size_t index);
    void transferPlayer ();
    void updatePlayersCards ();
};

#endif