#ifndef UNTITLED_GAMESTATE_H
#define UNTITLED_GAMESTATE_H

#include "GameModel.h"
#include <memory>
#include <optional>

class Player;

class GameState {
public:
    explicit GameState(int rows = 14, int cols = 14);

    GameModel& model() noexcept { return model_; }
    const GameModel& model() const noexcept { return model_; }

    std::shared_ptr<Player> player() const noexcept { return player_; }

    void setPlayer(std::shared_ptr<Player> p, std::optional<Coord> pos);

    std::optional<Coord> playerCoord() const noexcept { return model_.playerCoord(); }
    void setPlayerCoord(std::optional<Coord> pos) { model_.setPlayerCoord(pos); }

    int level() const noexcept;
    void setLevel(int l);

    int grantsGiven() const noexcept { return grantsGiven_; }
    void setGrantsGiven(int g) { grantsGiven_ = g; }

    int nextSpellThreshold() const noexcept { return nextSpellThreshold_; }
    void setNextSpellThreshold(int v) { nextSpellThreshold_ = v; }

private:
    GameModel model_;
    std::shared_ptr<Player> player_;
    int level_ = 1;
    int grantsGiven_ = 0;
    int nextSpellThreshold_ = 4;
};

#endif //UNTITLED_GAMESTATE_H
