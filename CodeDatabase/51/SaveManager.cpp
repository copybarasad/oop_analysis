#include "SaveManager.h"
#include "Exceptions.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>

using json = nlohmann::json;

// djb2
size_t computeStableHash(const std::string& str) {
    size_t hash = 5381;
    for (char c : str) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

void SaveManager::saveGame(const Game& game, const std::string& filename) {
    try {
        json gameData;
        const Board& board = game.getBoard();

        // метаданные
        gameData["meta"]["level"] = game.getLevel();
        gameData["meta"]["turn"] = game.getTurnCounter();
        gameData["meta"]["seed"] = game.getSeed();
        gameData["meta"]["width"] = board.getWidth();
        gameData["meta"]["height"] = board.getHeight();

		// распределение типов клеток
        for (int y = 0; y < board.getHeight(); ++y) {
            for (int x = 0; x < board.getWidth(); ++x) {
                const Cell& c = board.getCell(x, y);
                std::string t;
                switch (c.getType()) {
                case CellType::Normal: t = "normal"; break;
                case CellType::Wall:   t = "wall";   break;
                case CellType::Slow:   t = "slow";   break;
                }
                gameData["board"].push_back({ {"x", x}, {"y", y}, {"type", t} });
            }
        }

        // данные игрока
        if (auto p = game.getPlayer()) {
            auto [px, py] = p->getPosition();
            gameData["player"]["x"] = px;
            gameData["player"]["y"] = py;
            gameData["player"]["hp"] = p->getHealth();
            gameData["player"]["attack_mode"] = (p->getAttackMode() == AttackMode::Melee ? "melee" : "ranged");

            gameData["player"]["stats"]["melee"] = p->getMeleeAttackPower();
            gameData["player"]["stats"]["ranged"] = p->getRangedAttackPower();

            for (auto& sp : p->getSpellHand().getSpells()) {
                gameData["player"]["spells"].push_back(sp->name());
            }
        }

        // враги
        for (auto& e : game.getEnemies()) {
            if (e->isAlive()) {
                auto [ex, ey] = e->getPosition();
                gameData["enemies"].push_back({ {"x", ex}, {"y", ey}, {"hp", e->getHealth()} });
            }
        }

        // спавнеры
        for (auto& s : game.getSpawners()) {
            if (s->isAlive()) {
                auto [sx, sy] = s->getPosition();
                gameData["spawners"].push_back({ {"x", sx}, {"y", sy} });
            }
        }

        // башни
        for (auto& t : game.getTowers()) {
            if (t->isAlive()) {
                auto [tx, ty] = t->getPosition();
                gameData["towers"].push_back({ {"x", tx}, {"y", ty}, {"range", t->getRange()} });
            }
        }

        // логи
        gameData["logs"] = game.getLogs();

        // хэш и запись
        std::string serializedData = gameData.dump(-1);
        size_t hash = computeStableHash(serializedData);

        json finalJson;
        finalJson["hash"] = hash;
        finalJson["data"] = gameData;

        std::ofstream file(filename);
        if (!file.is_open()) throw FileException(filename, "Cannot open for writing");

        file << std::setw(4) << finalJson;
        std::cout << "[SaveManager] Saved successfully.\n";
    }
    catch (const json::exception& e) { throw SaveDataException(e.what()); }
    catch (const GameException&) { throw; }
    catch (const std::exception& e) { throw FileException(filename, e.what()); }
}

void SaveManager::loadGame(Game& game, const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) throw FileException(filename, "File not found");

        json finalJson;
        file >> finalJson;

        if (!finalJson.contains("hash") || !finalJson.contains("data")) {
            throw CorruptedSaveException("Invalid format (missing hash/data)");
        }

        size_t storedHash = finalJson["hash"];
        json gameData = finalJson["data"];

        std::string currentDataString = gameData.dump(-1);
        size_t calculatedHash = computeStableHash(currentDataString);

        if (storedHash != calculatedHash) {
            throw CorruptedSaveException("File has been modified manually!");
        }

        // загрузка
        if (!gameData.contains("meta")) throw SaveDataException("Missing meta data");

        int level = gameData["meta"].value("level", 1);
        int seed = gameData["meta"]["seed"];
        int savedTurn = gameData["meta"]["turn"];
        int width = gameData["meta"].value("width", 15);
        int height = gameData["meta"].value("height", 15);

        game.setLevel(level);
        game.setSeed(seed);

        game.clearEntities(); // Чистим всё перед загрузкой

        // Создаем доску нужного размера
        game.board = Board(width, height, game.rng);

        // загрузка игрока
        auto p = std::make_shared<Player>();
        p->setHealth(gameData["player"]["hp"]);
        p->setAttackMode(gameData["player"]["attack_mode"] == "melee" ? AttackMode::Melee : AttackMode::Ranged);

        if (gameData["player"].contains("stats")) {
            p->setAttackPowers(
                gameData["player"]["stats"].value("melee", Config::PLAYER_MELEE_DAMAGE),
                gameData["player"]["stats"].value("ranged", Config::PLAYER_RANGED_DAMAGE)
            );
        }

        p->getSpellHand().clear();
        for (auto& s : gameData["player"]["spells"]) {
            std::string nm = s.get<std::string>();
            if (nm == "Firebolt") p->getSpellHand().addSpell(std::make_shared<Firebolt>(5, 3));
            else if (nm == "EnhanceSpell") p->getSpellHand().addSpell(std::make_shared<EnhanceSpell>());
            else if (nm == "Explosion") p->getSpellHand().addSpell(std::make_shared<Explosion>());
            else if (nm == "TrapSpell") p->getSpellHand().addSpell(std::make_shared<TrapSpell>());
            else if (nm == "SummonAlly") p->getSpellHand().addSpell(std::make_shared<SummonAlly>());
        }
        game.setPlayer(p);

        // защита от "Cell Occupied" для Игрока
        try {
            game.placeEntityAt(p, gameData["player"]["x"], gameData["player"]["y"]);
        }
        catch (...) {
            std::cerr << "[WARNING] Player pos blocked. Spawning randomly.\n";
            game.spawnEntity(p);
        }

        // враги
        if (gameData.contains("enemies")) {
            for (auto& e : gameData["enemies"]) {
                auto enemy = std::make_shared<Enemy>();
                enemy->setHealth(e["hp"]);
                game.addEnemy(enemy);
                // защита
                try {
                    game.placeEntityAt(enemy, e["x"], e["y"]);
                }
                catch (...) {
                    std::cerr << "[WARNING] Skipped overlapping enemy.\n";
                }
            }
        }

        // спавнеры
        if (gameData.contains("spawners")) {
            for (auto& s : gameData["spawners"]) {
                auto spawner = std::make_shared<EnemySpawner>();
                game.addSpawner(spawner);
                // защита
                try {
                    game.placeEntityAt(spawner, s["x"], s["y"]);
                }
                catch (...) {
                    std::cerr << "[WARNING] Skipped overlapping spawner.\n";
                }
            }
        }

        // === БАШНИ ===
        if (gameData.contains("towers")) {
            for (auto& t : gameData["towers"]) {
                auto tower = std::make_shared<EnemyTower>(std::make_shared<Firebolt>(5, 3), t["range"]);
                tower->setPosition(t["x"], t["y"]);
                // защита
                try {
                    game.addTower(tower);
                }
                catch (...) {
                    std::cerr << "[WARNING] Skipped overlapping tower.\n";
                }
            }
        }

        // логи
        if (gameData.contains("logs")) {
            std::vector<std::string> loadedLogs = gameData["logs"].get<std::vector<std::string>>();
            game.setLogs(loadedLogs);
        }

        game.setTurnCounter(savedTurn);
        std::cout << "[SaveManager] Integrity Verified. Level " << level << " loaded.\n";
    }
    catch (const json::exception& e) {
        throw SaveDataException(std::string("JSON Error: ") + e.what());
    }
}