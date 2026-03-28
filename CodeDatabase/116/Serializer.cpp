#include "Serializer.h"
#include "GameWorld.h"
#include "FieldGenerator.h"
#include "../spells/ISpell.h"
#include "../spells/DirectFightSpell.h"
#include "../spells/AreaDamageSpell.h"
#include "../spells/TrapSpell.h"
#include "../spells/SummonSpell.h"
#include "../spells/EnhancingSpell.h"
#include <fstream>
#include <filesystem>

nlohmann::json Serializer::serializeGameWorld(const GameWorld& world, const FieldGenerator& generator) {
    nlohmann::json j;

    j["generator_params"] = {
        {"enemiesCount", generator.getEnemiesCount()},
        {"wallsCount", generator.getWallsCount()},
        {"slowingCellsCount", generator.getSlowingCellsCount()},
        {"fortressesCount", generator.getFortressesCount()}
    };

    j["field"] = serializeGameField(world.getField());
    j["player"] = serializePlayer(world.getPlayer());
    j["object_positions"] = serializeObjectPositions(world);
    
    return j;
}

std::unique_ptr<GameWorld> Serializer::deserializeGameWorld(const nlohmann::json& j) {
    try {
        validateGameWorldJson(j);

        int width = j["field"]["width"];
        int height = j["field"]["height"];
        auto world = std::make_unique<GameWorld>(width, height);

        GameField field = deserializeGameField(j["field"]);
        world->getField() = std::move(field);

        deserializePlayer(world->getPlayer(), j["player"]);

        const auto& params = j["generator_params"];
        FieldGenerator generator(
            params["enemiesCount"],
            params["slowingCellsCount"],
            params["wallsCount"], 
            params["fortressesCount"]
        );

        world->clearWorld();
        deserializeObjectPositions(*world, generator, j["object_positions"]);

        auto& player = world->getPlayer();
        world->getField().getCell(player.getX(), player.getY()).setType(Cell::CellType::PLAYER);
        
        return world;
        
    } catch (const nlohmann::json::exception& e) {
        throw InvalidDataException("Ошибка парсинга JSON: " + std::string(e.what()));
    } catch (const std::exception& e) {
        throw InvalidDataException(std::string(e.what()));
    }
}

nlohmann::json Serializer::serializeObjectPositions(const GameWorld& world) {
    nlohmann::json j;

    j["tower"] = {
        {"x", world.getTower().getX()},
        {"y", world.getTower().getY()},
        {"health", world.getTower().getHealth()},
        {"attackFrequency", world.getTower().getAttackFrequency()}
    };

    nlohmann::json enemiesArray = nlohmann::json::array();
    for (const auto& enemy : world.getEnemies()) {
        enemiesArray.push_back({
            {"x", enemy->getX()},
            {"y", enemy->getY()},
            {"health", enemy->getHealth()},
            {"damage", enemy->getDamage()}
        });
    }
    j["enemies"] = enemiesArray;

    nlohmann::json alliesArray = nlohmann::json::array();
    for (const auto& ally : world.getAllies()) {
        alliesArray.push_back({
            {"x", ally->getX()},
            {"y", ally->getY()},
            {"health", ally->getHealth()},
            {"damage", ally->getDamage()}
        });
    }
    j["allies"] = alliesArray;

    nlohmann::json trapsArray = nlohmann::json::array();
    for (const auto& trap : world.getTraps()) {
        trapsArray.push_back({
            {"x", trap->getX()},
            {"y", trap->getY()},
            {"damage", trap->getDamage()}
        });
    }
    j["traps"] = trapsArray;

    nlohmann::json fortressesArray = nlohmann::json::array();
    for (const auto& fortress : world.getFortresses()) {
        fortressesArray.push_back({
            {"x", fortress->getX()},
            {"y", fortress->getY()},
            {"health", fortress->getHealth()},
            {"attackRadius", fortress->getAttackRadius()},
            {"damage", fortress->getDamage()},
            {"attackFrequency", fortress->getAttackFrequency()}
        });
    }
    j["fortresses"] = fortressesArray;
    
    return j;
}

void Serializer::deserializeObjectPositions(GameWorld& world, FieldGenerator& generator, const nlohmann::json& j) {
    const auto& tower = j["tower"];
    generator.spawnBuilding(world, tower["x"], tower["y"], tower["health"], tower["attackFrequency"]);
    
    for (const auto& enemy : j["enemies"]) {
        generator.spawnEnemy(world, enemy["x"], enemy["y"], enemy["health"], enemy["damage"]);
    }

    for (const auto& ally : j["allies"]) {
        generator.spawnAlly(world, ally["x"], ally["y"], ally["health"], ally["damage"]);
    }

    for (const auto& trap : j["traps"]) {
        generator.spawnTrap(world, trap["x"], trap["y"], trap["damage"]);
    }

    for (const auto& fortress : j["fortresses"]) {
        generator.spawnFortress(world, fortress["x"], fortress["y"], 
                               fortress["health"], fortress["attackRadius"], 
                               fortress["damage"], fortress["attackFrequency"]);
    }
}

nlohmann::json Serializer::serializeCell(const Cell& cell) {
    return {
        {"type", static_cast<int>(cell.getType())},
        {"slowdown", cell.isSlowing()}
    };
}

Cell Serializer::deserializeCell(const nlohmann::json& j) {
    Cell::CellType type = static_cast<Cell::CellType>(j["type"]);
    Cell cell(type);
    
    if (j["slowdown"]) {
        cell.setSlowing();
    }
    
    return cell;
}

nlohmann::json Serializer::serializeGameField(const GameField& field) {
    nlohmann::json j;
    j["width"] = field.getWidth();
    j["height"] = field.getHeight();
    
    nlohmann::json gridArray = nlohmann::json::array();
    for (int y = 0; y < field.getHeight(); ++y) {
        nlohmann::json rowArray = nlohmann::json::array();
        for (int x = 0; x < field.getWidth(); ++x) {
            rowArray.push_back(serializeCell(field.getCell(x, y)));
        }
        gridArray.push_back(rowArray);
    }
    j["grid"] = gridArray;
    
    return j;
}

GameField Serializer::deserializeGameField(const nlohmann::json& j) {
    int width = j["width"];
    int height = j["height"];
    GameField field(width, height);
    
    const auto& gridArray = j["grid"];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell cell = deserializeCell(gridArray[y][x]);
            field.setCell(x, y, cell);
        }
    }
    
    return field;
}

nlohmann::json Serializer::serializePlayerHand(const PlayerHand& hand) {
    nlohmann::json j;
    j["maxCapacity"] = hand.getMaxCapacity();

    nlohmann::json slotsArray = nlohmann::json::array();
    const auto& spells = hand.getSpells();
    
    for (int slotIndex = 0; slotIndex < spells.size(); ++slotIndex) {
        nlohmann::json slotArray = nlohmann::json::array();
        
        for (const auto& spell : spells[slotIndex]) {
            if (spell) {
                slotArray.push_back({
                    {"name", spell->getName()},
                    {"level", spell->getLVL()}
                });
            }
        }
        slotsArray.push_back(slotArray);
    }
    j["slots"] = slotsArray;
    
    return j;
}

void Serializer::deserializePlayerHand(PlayerHand& hand, const nlohmann::json& j) {
    hand.clearAndResize();

    const auto& slotsArray = j["slots"];
    for (int slotIndex = 0; slotIndex < slotsArray.size(); ++slotIndex) {
        const auto& slotArray = slotsArray[slotIndex];
        for (const auto& spellJson : slotArray) {
            std::string name = spellJson["name"];
            int level = spellJson["level"];
            
            auto spell = createSpellByName(name, level);
            if (spell) {
                hand.addSpell(std::move(spell), slotIndex);
            }
        }
    }
}

std::unique_ptr<ISpell> Serializer::createSpellByName(const std::string& name, int level) {
    int index = getSpellIndexByName(name);
    
    switch (index) {
        case 0:
            return std::make_unique<DirectFightSpell>(level);
        case 1:
            return std::make_unique<AreaDamageSpell>(level);
        case 2:
            return std::make_unique<TrapSpell>(level);
        case 3:
            return std::make_unique<SummonSpell>(level);
        case 4:
            return std::make_unique<EnhancingSpell>(level);
        default:
            return nullptr;
    }
}

int Serializer::getSpellIndexByName(const std::string& name) {
    DirectFightSpell directSpell;
    AreaDamageSpell areaSpell;
    TrapSpell trapSpell;
    SummonSpell summonSpell;
    EnhancingSpell enhanceSpell;

    if (name == directSpell.getName()) {
        return 0;
    } else if (name == areaSpell.getName()) {
        return 1;
    } else if (name == trapSpell.getName()) {
        return 2;
    } else if (name == summonSpell.getName()) {
        return 3;
    } else if (name == enhanceSpell.getName()) {
        return 4;
    }
    
    return -1;
}

nlohmann::json Serializer::serializePlayer(const Player& player) {
    return {
        {"x", player.getX()},
        {"y", player.getY()},
        {"health", player.getHealth()},
        {"maxHealth", player.getMaxHealth()},
        {"score", player.getScore()},
        {"coins", player.getCoins()},
        {"meleeDamage", player.getPlayerDamages().first},
        {"rangedDamage", player.getPlayerDamages().second},
        {"combatMode", static_cast<int>(player.getCombatMode())},
        {"slowed", player.isSlowed()},
        {"hand", serializePlayerHand(player.getPlayerHand())}
    };
}

void Serializer::deserializePlayer(Player& player, const nlohmann::json& j) {
    player.setPosition(j["x"], j["y"]);

    player.setHealth(j["health"]);
    player.setMaxHealth(j["maxHealth"]);
    player.setScore(j["score"]);
    player.setCoins(j["coins"]);
    player.setMeleeDamage(j["meleeDamage"]);
    player.setRangedDamage(j["rangedDamage"]);

    Player::CombatMode combatMode = static_cast<Player::CombatMode>(j["combatMode"]);
    player.setCombatMode(combatMode);

    player.setSlowed(j["slowed"]);

    deserializePlayerHand(player.getPlayerHand(), j["hand"]);
}

void Serializer::validateGameWorldJson(const nlohmann::json& j) {
    if (!j.contains("field") || !j.contains("player") || !j.contains("object_positions") || !j.contains("generator_params")) {
        throw InvalidDataException("Отсутствуют обязательные поля в JSON");
    }
    
    validateFieldJson(j["field"]);
    validatePlayerJson(j["player"]);
    validateObjectPositionsJson(j["object_positions"]);

    const auto& params = j["generator_params"];
    std::vector<std::string> requiredParams = {"enemiesCount", "wallsCount", "slowingCellsCount", "fortressesCount"};
    for (const auto& param : requiredParams) {
        if (!params.contains(param)) {
            throw InvalidDataException("Отсутствует параметр генератора: " + param);
        }
        if (!params[param].is_number()) {
            throw InvalidDataException("Параметр генератора должен быть числом: " + param);
        }
    }
}

void Serializer::validateFieldJson(const nlohmann::json& j) {
    if (!j.contains("width") || !j.contains("height") || !j.contains("grid")) {
        throw InvalidDataException("Отсутствуют обязательные поля поля");
    }
    
    if (!j["width"].is_number() || !j["height"].is_number()) {
        throw InvalidDataException("Ширина и высота поля должны быть числами");
    }
    
    int width = j["width"];
    int height = j["height"];
    
    if (width <= 0 || height <= 0 || width > 100 || height > 100) {
        throw InvalidDataException("Некорректные размеры поля: " + std::to_string(width) + "x" + std::to_string(height));
    }
    
    if (!j["grid"].is_array() || j["grid"].size() != height) {
        throw InvalidDataException("Некорректная структура сетки поля");
    }
    
    for (const auto& row : j["grid"]) {
        if (!row.is_array() || row.size() != width) {
            throw InvalidDataException("Некорректная структура строки в сетке поля");
        }
        for (const auto& cell : row) {
            if (!cell.contains("type") || !cell["type"].is_number()) {
                throw InvalidDataException("Некорректные данные ячейки");
            }
        }
    }
}

void Serializer::validatePlayerJson(const nlohmann::json& j) {
    std::vector<std::string> requiredFields = {
        "x", "y", "health", "maxHealth", "score", "coins", 
        "meleeDamage", "rangedDamage", "combatMode", "slowed", "hand"
    };
    
    for (const auto& field : requiredFields) {
        if (!j.contains(field)) {
            throw InvalidDataException("Отсутствует поле игрока: " + field);
        }
    }

    int x = j["x"];
    int y = j["y"];
    if (x < 0 || y < 0) {
        throw InvalidDataException("Некорректные координаты игрока: " + std::to_string(x) + ", " + std::to_string(y));
    }

    int health = j["health"];
    int maxHealth = j["maxHealth"];
    if (health < 0 || maxHealth <= 0 || health > maxHealth) {
        throw InvalidDataException("Некорректное здоровье игрока: " + std::to_string(health) + "/" + std::to_string(maxHealth));
    }

    int meleeDamage = j["meleeDamage"];
    int rangedDamage = j["rangedDamage"];
    if (meleeDamage < 0 || rangedDamage < 0) {
        throw InvalidDataException("Некорректный урон игрока");
    }
}

void Serializer::validateObjectPositionsJson(const nlohmann::json& j) {
    if (!j.contains("tower") || !j.contains("enemies") || !j.contains("allies") || 
        !j.contains("traps") || !j.contains("fortresses")) {
        throw InvalidDataException("Отсутствуют обязательные поля объектов");
    }

    const auto& tower = j["tower"];
    if (!tower.contains("x") || !tower.contains("y") || !tower.contains("health") || !tower.contains("attackFrequency")) {
        throw InvalidDataException("Некорректные данные башни");
    }

    std::vector<std::string> objectTypes = {"enemies", "allies", "traps", "fortresses"};
    for (const auto& type : objectTypes) {
        if (!j[type].is_array()) {
            throw InvalidDataException("Поле " + type + " должно быть массивом");
        }
        
        for (const auto& obj : j[type]) {
            if (!obj.contains("x") || !obj.contains("y")) {
                throw InvalidDataException("Объект в " + type + " не имеет координат");
            }
        }
    }
}