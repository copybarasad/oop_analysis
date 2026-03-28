#pragma once
#include <stdexcept>

class GameException : public std::runtime_error {
public:
    GameException(const std::string& msg) : std::runtime_error(msg) {}
};

class PlayerDyeException : public GameException {
public:
    PlayerDyeException()
        : GameException("You died. Game over.") {
    }
};

class BlockedCell : public GameException {
public:
    BlockedCell()
        : GameException("Cell is blocked.") {
    }
};

class FriendInCell : public GameException {
public:
    FriendInCell()
        : GameException("Friend in this cell.") {
    }
};