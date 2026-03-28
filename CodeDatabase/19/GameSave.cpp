#include "GameSave.h"

#include <fstream>
#include <iostream>
#include <filesystem>
#include <memory>

#include "Game.h"
#include "GameLogic.h"
#include "entity/types/Ally.h"
#include "entity/types/Enemy.h"
#include "entity/types/EnemyHouse.h"
#include "entity/types/EnemyTower.h"
#include "entity/types/Trap.h"
#include "entity/types/Player.h"

bool GameSave::save(Game &game) const {
    const json j = serialize(game);
    std::filesystem::create_directories(file_.parent_path());
    std::ofstream o(file_);
    if (!o) {
        std::cerr << "Failed to open save file: " << file_ << std::endl;
        return false;
    }

    o << std::setw(4) << j;
    if (!o) {
        std::cerr << "Failed to write to save file: " << file_ << std::endl;
        return false;
    }

    return true;
}

bool GameSave::load(Game &game) const {
    if (!std::filesystem::exists(file_)) {
        std::cerr << "File " << file_ << " does not exist" << std::endl;
        return false;
    }

    std::ifstream i(file_);
    if (!i) {
        std::cerr << "Failed to open save file for reading: " << file_ << std::endl;
        return false;
    }

    json j;
    try {
        i >> j;
    } catch (const std::exception &e) {
        std::cerr << "Invalid JSON in save file: " << e.what() << std::endl;
        return false;
    }

    return deserialize(game, j);
}

bool GameSave::delete_file() const {
    try {
        if (std::filesystem::exists(file_)) {
            return std::filesystem::remove(file_);
        }
        return false;
    } catch (const std::exception &e) {
        std::cerr << "Failed to delete save file '" << file_ << "': " << e.what() << "\n";
        return false;
    }
}

json GameSave::serialize(Game &game) const {
    const GameLogic &logic = game.logic();
    const World &world = game.world();
    json core;
    core["wave"] = logic.get_wave();
    core["world_width"] = world.width();
    core["world_height"] = world.height();
    core["current_available_upgrades"] = logic.current_available_upgrades_;
    core["current_purchased_upgrades"] = logic.current_purchased_upgrades_;
    core["upgrades"] = logic.upgrades_;

    json tiles;

    for (int x = 0; x < world.width(); ++x) {
        for (int y = 0; y < world.height(); ++y) {
            json tile_info;
            const WorldTile &tile = world.get_tile(x, y);

            tile_info["type"] = tile.type();
            if (tile.entity() != nullptr) {
                tile_info["entity"] = {
                    {"data", tile.entity()->serialize()},
                    {"type", tile.entity()->type()}
                };
            }
            tiles[x][y] = tile_info;
        }
    }

    core["tiles"] = tiles;
    core["state"] = game.state();
    return core;
}

bool GameSave::deserialize(Game &game, json &data) const {
    std::vector<std::string> errors;
    int wave = 1;
    int world_width = 0;
    int world_height = 0;
    GameState state = GameState::PlayerMove;
    std::vector<std::string> current_available_upgrades;
    std::unordered_set<int> current_purchased_upgrades;
    std::map<std::string, int> upgrades;

    try {
        wave = data.at("wave");
        world_width = data.at("world_width");
        world_height = data.at("world_height");
        state = data.at("state");
        current_available_upgrades = data.at("current_available_upgrades").get<std::vector<std::string> >();
        current_purchased_upgrades = data.at("current_purchased_upgrades").get<std::unordered_set<int> >();
        upgrades = data.at("upgrades");
    } catch (const std::exception &e) {
        errors.push_back(std::string("Missing or invalid data: ") + e.what());
    }

    auto world = std::make_unique<World>(world_width, world_height);
    std::vector<std::unique_ptr<Entity> > entities;
    bool has_player = false;

    if (errors.empty()) {
        try {
            auto &tiles = data.at("tiles");
            for (int x = 0; x < world_width; ++x) {
                for (int y = 0; y < world_height; ++y) {
                    try {
                        auto &tile_info = tiles.at(x).at(y);
                        auto &tile = world->get_tile(x, y);
                        tile.set_type(tile_info.at("type"));

                        if (tile_info.contains("entity")) {
                            auto &entity_data = tile_info.at("entity");
                            const EntityType entity_type = entity_data.at("type");

                            std::unique_ptr<Entity> entity;
                            switch (entity_type) {
                                case EntityType::Player:
                                    entity = std::make_unique<Player>(x, y);
                                    has_player = true;
                                    break;
                                case EntityType::Enemy:
                                    entity = std::make_unique<Enemy>(x, y);
                                    break;
                                case EntityType::EnemyTower:
                                    entity = std::make_unique<EnemyTower>(x, y);
                                    break;
                                case EntityType::EnemyHouse:
                                    entity = std::make_unique<EnemyHouse>(x, y);
                                    break;
                                case EntityType::Ally:
                                    entity = std::make_unique<Ally>(x, y);
                                    break;
                                case EntityType::Trap:
                                    entity = std::make_unique<Trap>(x, y);
                                    break;
                                default:
                                    errors.push_back(
                                        "Unknown entity type at (" + std::to_string(x) + "," + std::to_string(y) + ")");
                                    break;
                            }

                            if (entity) {
                                if (!entity->deserialize(entity_data.at("data"))) {
                                    errors.push_back(
                                        "Failed to deserialize entity at (" + std::to_string(x) + "," + std::to_string(
                                            y) + ")");
                                } else {
                                    entities.push_back(std::move(entity));
                                }
                            }
                        }
                    } catch (const std::exception &e) {
                        errors.push_back("Tile (" + std::to_string(x) + "," + std::to_string(y) + "): " + e.what());
                    }
                }
            }
        } catch (const std::exception &e) {
            errors.push_back(std::string("Invalid or missing tiles: ") + e.what());
        }
    }

    if (!has_player) {
        errors.push_back("Game has no player entity");
    }

    if (!errors.empty()) {
        std::cerr << "Deserialization errors:\n";
        for (const auto &err: errors)
            std::cerr << "  - " << err << "\n";
        return false;
    }

    if (game.world_ != nullptr) {
        const auto entities = game.world_->entities();
        for (Entity *entity: entities) {
            game.world_->remove_entity(entity);
            delete entity;
        }
        delete game.world_;
        game.world_ = nullptr;
    }

    game.world_ = world.release();

    for (auto &entity: entities) {
        game.logic().add_entity(entity.release());
    }

    game.logic().wave_ = wave;
    game.logic().upgrades_ = upgrades;
    game.logic().current_purchased_upgrades_ = current_purchased_upgrades;
    game.logic().current_available_upgrades_ = current_available_upgrades;
    game.player_won_ = false;
    game.state_ = state;
    game.hud_.reload_buttons();
    return true;
}
