#pragma once

#include <optional>
#include <stdexcept>
#include <string>

#include "Field.hpp"

// Whole game wrapper around Field plus save/load.
struct GameState {
    FieldState field;
    bool playerSkipMove{false};
};

class SaveLoadError : public std::runtime_error {
public:
    explicit SaveLoadError(const std::string& msg) : std::runtime_error(msg) {}
};
class SaveError : public SaveLoadError {
public:
    explicit SaveError(const std::string& msg) : SaveLoadError(msg) {}
};
class LoadError : public SaveLoadError {
public:
    explicit LoadError(const std::string& msg) : SaveLoadError(msg) {}
};

class Game {
    Field field_;
    bool playerSkipMove_{false};

    bool cast_spell_(std::size_t slot, std::optional<Pos> target);

public:
    Game(int w, int h, const GameParams& params = GameParams());

    bool step(const std::string& input);
    bool level_finished() const;
    bool player_alive() const { return field_.player().alive(); }

    GameState snapshot() const;
    void apply_state(const GameState& state);
    void save_to_file(const std::string& path) const;
    void load_from_file(const std::string& path);

    void print() const;

    const Field& field() const { return field_; }
};
