#ifndef GAME_H_
#define GAME_H_

#include "field.h"
#include "player.h"
#include "input_handler.h"
#include "renderer.h"
#include "game_renderer.h"
#include "game_controller.h"
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include "json.hpp"

class FileException : public std::runtime_error {
public:
    FileException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileNotFoundException : public FileException {
public:
    FileNotFoundException(const std::string& filename) : FileException("File not found: " + filename) {}
};

class InvalidSaveException : public FileException {
public:
    InvalidSaveException(const std::string& details) : FileException("Invalid save data: " + details) {}
};

class SaveFailedException : public FileException {
public:
    SaveFailedException(const std::string& details) : FileException("Save failed: " + details) {}
};


struct LevelConfig {
    int width, height, turns_to_spawn_enemy, obstacle_count, slow_count, building_count;
    int enemy_hp, enemy_damage, building_hp, building_damage, tower_hp, tower_damage;
};

class Game {
public:
    Game();
    ~Game() = default;
    void StartGame();
    void LoadGame();
    void SaveGame() const;
    bool IsLevelCompleted(const Field& field) const;
    void NextLevel();
    void UpgradePlayer();
    void RunLevel(int level_index);
    std::unique_ptr<Field> CreateFieldFromConfig(int width, int height, int turns_to_spawn_enemy, int obstacle_count, int slow_count, int building_count) const;

private:
    std::vector<LevelConfig> levels_;
    std::shared_ptr<Field> field_; 
    InputHandler input_handler_;
    Renderer renderer_;
    int current_level_;
    std::unique_ptr<Field> current_field_;
    std::shared_ptr<Player> player_;
    bool game_over_;
    bool is_loaded_;

    void InitializeLevels();
    nlohmann::json Serialize() const;
    void Deserialize(const nlohmann::json& j);
};

#endif // GAME_H_