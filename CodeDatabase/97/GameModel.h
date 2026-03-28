#ifndef UNTITLED_GAMEMODEL_H
#define UNTITLED_GAMEMODEL_H

#include "Field.h"
#include <memory>
#include <optional>

class Player;

class GameModel {
public:
    GameModel(int rows = 14, int cols = 14);

    Field& field() noexcept { return field_; }
    const Field& field() const noexcept { return field_; }

    void setPlayer(std::shared_ptr<Player> p, std::optional<Coord> pos);
    std::shared_ptr<Player> player() const noexcept { return player_; }
    std::optional<Coord> playerCoord() const noexcept { return playerPos_; }
    void setPlayerCoord(std::optional<Coord> pos) noexcept { playerPos_ = pos; }

private:
    Field field_;
    std::shared_ptr<Player> player_;
    std::optional<Coord> playerPos_;
};

#endif
