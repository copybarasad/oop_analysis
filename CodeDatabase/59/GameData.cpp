#include "GameData.h"
#include "json.hpp"
#include "Constants.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <random>


GameData::GameData() : 
    player(0, 0, {0,0}, 0), 
    field(10, 10, 0),
    spellShop(0, {0,0}, {0,0}, 0), 
    playerHand(0) {}


json GameData::toJson() const {
    json j;
    j["player"] = player.toJson();
    j["field"] = field.toJson();
    j["spellShop"] = spellShop.toJson();
    j["playerHand"] = playerHand.toJson();
    
    j["enemies"] = json::array();
    for(const auto& enemy : enemies) {
        j["enemies"].push_back(enemy.toJson());
    }
    
    j["towers"] = json::array();
    for(const auto& tower : towers) {
        j["towers"].push_back(tower.toJson());
    }
    
    j["active_traps"] = json::array();
    for(const auto& trap : active_traps) {
        j["active_traps"].push_back(trap.toJson());
    }

    j["current_level"] = current_level;
    
    return j;
}

void GameData::fromJson(const json& j) {
    player.fromJson(j.at("player"));
    field.fromJson(j.at("field"));
    spellShop.fromJson(j.at("spellShop"));
    playerHand.fromJson(j.at("playerHand"));
    j.at("current_level").get_to(current_level);
    
    enemies.clear();
    for (const auto& enemy_json : j.at("enemies")) {
        enemies.emplace_back(0, Coordinates{0,0}, 0);
        enemies.back().fromJson(enemy_json);
    }
    
    towers.clear();
    for (const auto& tower_json : j.at("towers")) {
        towers.emplace_back(0, 0, 0, Coordinates{0,0}, 0);
        towers.back().fromJson(tower_json);
    }
    
    active_traps.clear();
    for (const auto& trap_json : j.at("active_traps")) {
        active_traps.emplace_back(BasicSpell::Type::Trap, 0, 0, "");
        active_traps.back().fromJson(trap_json);
    }
}

GameData::GameData(bool isNewGame) : player(PLAYER_DAMAGE, PLAYER_BOW_DAMAGE, {PLAYER_START_X, PLAYER_START_Y}, PLAYER_HEALTH),
                                     field(SIZE_X, SIZE_Y, WALL_DENSITY),
                                     spellShop(SHOP_SPELLS_COUNT, {PRICE_RANGE_MIN, PRICE_RANGE_MAX},
                                              {DAMAGE_RANGE_MIN, DAMAGE_RANGE_MAX}, AOE_DAMAGE_MULTIPLIER),
                                     playerHand(0) {
    if (!isNewGame) return;
    current_level = 1;
    generateLevel(1);
}

void GameData::generateLevel(int level) {
    level--;
    level = std::max(0, level);
    int new_size_x = SIZE_X + level * LEVEL_BASE_SIZE_STEP;
    int new_size_y = SIZE_Y + level * LEVEL_BASE_SIZE_STEP;
    int enemy_damage = ENEMY_DAMAGE + level * LEVEL_ENEMY_DAMAGE_STEP;
    int enemy_health = ENEMY_HEALTH + level * LEVEL_ENEMY_HEALTH_STEP;
    int tower_damage = TOWER_DAMAGE + level * LEVEL_TOWER_DAMAGE_STEP;
    int tower_health = TOWER_HEALTH + level * LEVEL_TOWER_HEALTH_STEP;

    field.resize(new_size_x, new_size_y);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> wall_dist(0.0, 1.0);

    for (int y = 0; y < field.getHeight(); ++y) {
        for (int x = 0; x < field.getWidth(); ++x) {
            Coordinates pos{x, y};
            if (wall_dist(gen) < WALL_DENSITY && !(pos.x == PLAYER_START_X && pos.y == PLAYER_START_Y)) {
                field.setCellType(pos, Cell::Type::Wall);
            } else {
                field.setCellType(pos, Cell::Type::Empty);
            }
        }
    }

    Coordinates start_pos = {PLAYER_START_X, PLAYER_START_Y};
    player.setPosition(start_pos);
    field.setCellType(start_pos, Cell::Type::Player);

    std::uniform_int_distribution<> x_dist(0, field.getWidth() - 1);
    std::uniform_int_distribution<> y_dist(0, field.getHeight() - 1);

    enemies.clear();
    for (int i = 0; i < ENEMY_COUNT + level; i++) {
        Coordinates pos;
        do {
            pos.x = x_dist(gen);
            pos.y = y_dist(gen);
        } while (!field.isCellFree(pos));
        enemies.emplace_back(enemy_damage, pos, enemy_health);
        field.setCellType(pos, Cell::Type::Enemy);
    }

    towers.clear();
    for (int i = 0; i < TOWER_COUNT + level / 2; i++) {
        Coordinates pos;
        do {
            pos.x = x_dist(gen);
            pos.y = y_dist(gen);
        } while (!field.isCellFree(pos));
        towers.emplace_back(tower_damage, TOWER_RANGE, TOWER_DELAY, pos, tower_health);
        field.setCellType(pos, Cell::Type::Tower);
    }

    active_traps.clear();
    if (level == 0) {
        std::vector<std::unique_ptr<BasicSpell>> spells;
        for (int i = 0; i < PLAYER_START_SPELL_COUNT; i++) {
            spells.push_back(spellShop.createRandomSpell());
        }
        playerHand = PlayerHand(PLAYER_HAND_SIZE, std::move(spells));
    }

    spellShop.regenerate();
}

void GameData::goToNextLevel() {
    current_level++;
    player.setHealth((float)player.getMaxHealth());

    int spells_to_delete = playerHand.getSpellCount() / 2;
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < spells_to_delete; ++i) {
        if (playerHand.getSpellCount() == 0) break;
        std::uniform_int_distribution<> distrib(0, playerHand.getSpellCount() - 1);
        int index_to_delete = distrib(gen);
        playerHand.removeSpell(index_to_delete);
    }

    std::cout << "\n\n--- Уровень " << current_level << " ---\n";
    std::cout << "Пока вы добирались до уровня, половина ваших заклинаний потерялись.\n";

    char choice = 0;
    bool valid_input = false;
    while (!valid_input) {
        std::cout << "Выберите характеристику для улучшения:\n";
        std::cout << "1. Урон мечом (" << player.getBaseDamage() << " -> " << player.getBaseDamage() + LEVEL_PLAYER_UPGRADE_DAMAGE_STEP << ")\n";
        std::cout << "2. Макс. здоровье (" << player.getMaxHealth() << " -> " << player.getMaxHealth() + LEVEL_PLAYER_UPGRADE_HEALTH_STEP << ")\n";
        std::cout << "3. Урон луком (" << player.getBowDamage() << " -> " << player.getBowDamage() + LEVEL_PLAYER_UPGRADE_BOW_STEP << ")\n";
        std::cout << "Введите номер: ";
        std::string input;
        std::getline(std::cin, input);
        if (input.length() == 1) {
            choice = input[0];
            if (choice >= '1' && choice <= '3') {
                valid_input = true;
                continue;
            }
        }
        std::cout << "Некорректный ввод. Попробуйте ещё раз.\n";
    }

    switch (choice) {
        case '1':
            player.setDamage(player.getBaseDamage() + LEVEL_PLAYER_UPGRADE_DAMAGE_STEP);
            std::cout << "Урон мечом увеличен на " << LEVEL_PLAYER_UPGRADE_DAMAGE_STEP << ".\n";
            break;
        case '2':
            player.setMaxHealth(player.getMaxHealth() + LEVEL_PLAYER_UPGRADE_HEALTH_STEP);
            player.setHealth((float)player.getMaxHealth());
            std::cout << "Макс. здоровье увеличено на " << LEVEL_PLAYER_UPGRADE_HEALTH_STEP << ".\n";
            break;
        case '3':
            player.setBowDamage(player.getBowDamage() + LEVEL_PLAYER_UPGRADE_BOW_STEP);
            std::cout << "Урон луком увеличен на " << LEVEL_PLAYER_UPGRADE_BOW_STEP << ".\n";
            break;
        default:
            break;
    }

    std::cout << "Новый уровень загружается...\n";

    generateLevel(current_level);
}