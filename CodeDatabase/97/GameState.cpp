#include "GameState.h"
#include "Player.h"

GameState::GameState(int rows, int cols)
        : model_(rows, cols), player_(nullptr), level_(1), grantsGiven_(0), nextSpellThreshold_(4) {}

void GameState::setPlayer(std::shared_ptr<Player> p, std::optional<Coord> pos) {
    player_ = std::move(p);
    model_.setPlayer(player_, pos);
}

int GameState::level() const noexcept {
    return level_;
}

void GameState::setLevel(int l) { level_ = l; }
