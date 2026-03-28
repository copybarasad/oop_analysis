#pragma once
#include <string>

class Game;

class SaveManager {
public:
    explicit SaveManager(std::string default_path = "save.txt");

    void SaveToFile(const Game& game, const std::string& path) const;
    void LoadFromFile(Game& game, const std::string& path) const;

    void Save(const Game& game) const;
    void Load(Game& game) const;

private:
    std::string path_;
    static constexpr const char* kMagic() { return "GAME_SAVE"; }
};
