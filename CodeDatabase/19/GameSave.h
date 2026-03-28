#pragma once

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Game;

class GameSave {
public:
    GameSave(const std::filesystem::path &filePath) : file_(filePath) {
    }

    bool save(Game &game) const;

    bool load(Game &game) const;

    bool delete_file() const;

private:
    std::filesystem::path file_;

    json serialize(Game &game) const;

    bool deserialize(Game &game, json &json) const;
};
