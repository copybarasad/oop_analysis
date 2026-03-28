#include "GameModel.h"
#include "Player.h"

GameModel::GameModel(int rows, int cols) : field_(rows, cols), player_(nullptr), playerPos_(std::nullopt) {}

void GameModel::setPlayer(std::shared_ptr<Player> p, std::optional<Coord> pos) {
    player_ = std::move(p);
    playerPos_ = pos;
    if (player_ && playerPos_) {
        field_.placeEntity(player_, *playerPos_);
    }
}
