#include "GameSaver.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include "Spells/SpellHand.hpp"
#include "Entities/Slime.hpp"
#include "Entities/Demon.hpp"
#include "Entities/Shadow.hpp"
#include "Entities/Knight.hpp"
#include "Entities/MagicStatue.hpp"
#include "Entities/Portal.hpp"
#include "Entities/Goblin.hpp"
#include "Field/TrapCell.hpp"
#include "Field/OrdinaryCell.hpp"
#include "Field/SlowingCell.hpp"
#include "Field/ImpassableCell.hpp"


void GameSaver::saveGame(Hero* hero, Entity_Manager* manager, Field* field, const std::string& filename, int current_level) {
    json saveData;

    saveData["game_info"] = {
        {"current_level", current_level}
    };

    saveData["hero"] = {
        {"position", {hero->get_location().x, hero->get_location().y}},
        {"hp", hero->get_helpth()},
        {"level", hero->getLevel()},
        {"LevelEnhancementSpell", hero->getLevelEnhancementSpellHero()},
        {"weapon", int(hero->get_weapon())},
        {"total_kills", hero->get_total_kills()}
    };

    json spellsJson = json::array();
    auto& spellHand = hero->get_SpellHand();
    for (auto spell : spellHand.getVectorSpells()) {
        if (spell) spellsJson.push_back(spell->getName());
    }
    saveData["hero"]["spells"] = spellsJson;

    json unindictedSpellsJson = json::array();
    for (auto spell : spellHand.getUnindictedSpells()) {
        if (spell) unindictedSpellsJson.push_back(spell->getName());
    }
    saveData["hero"]["unindicted_spells"] = unindictedSpellsJson;

    json enemiesJson = json::array();
    for (auto enemy : manager->get_enemies()) {
        if (enemy && enemy->get_helpth() > 0) {
            enemiesJson.push_back({
                {"type", enemy->get_name()},
                {"position", {enemy->get_location().x, enemy->get_location().y}},
                {"hp", enemy->get_helpth()},
                {"level", enemy->getLevel()}
            });
        }
    }
    saveData["enemies"] = enemiesJson;

    json alliesJson = json::array();
    for (auto ally : manager->get_allies()) {
        if (ally && ally->get_helpth() > 0) {
            alliesJson.push_back({
                {"type", ally->get_name()},
                {"position", {ally->get_location().x, ally->get_location().y}},
                {"hp", ally->get_helpth()},
                {"level", ally->getLevel()}
            });
        }
    }
    saveData["allies"] = alliesJson;

    if (manager->get_portal() && manager->get_portal()->get_helpth() > 0) {
        saveData["portal"] = {
            {"position", {manager->get_portal()->get_location().x, manager->get_portal()->get_location().y}},
            {"hp", manager->get_portal()->get_helpth()},
            {"level", manager->get_portal()->getLevel()},
            {"frequency", manager->get_portal()->get_frequency()}
        };
    }

    if (manager->get_statue() && manager->get_statue()->get_helpth() > 0) {
        saveData["statue"] = {
            {"position", {manager->get_statue()->get_location().x, manager->get_statue()->get_location().y}},
            {"hp", manager->get_statue()->get_helpth()},
            {"level", manager->get_statue()->getLevel()}
        };
    }

    json cellsJson = json::array();
    auto cells = field->get_cells();
    size_t width = cells.size();
    size_t height = cells[0].size();

    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            Cell* cell = cells[x][y];
            if (!cell) continue;

            json entry = {
                {"x", int(x)},
                {"y", int(y)},
                {"type", int(cell->get_status_cell())}
            };

            if (cell->get_status_cell() == trap) {
                TrapCell* trap_cell = static_cast<TrapCell*>(cell);
                entry["damage"] = trap_cell->get_damage();
            }

            cellsJson.push_back(entry);
        }
    }

    saveData["field"] = {
        {"width", int(width)},
        {"height", int(height)},
        {"cells", cellsJson}
    };

    std::ofstream file(filename);
    if (!file) throw std::runtime_error("Error when opening the file for saving!\n");
    file << saveData.dump(4);
    file.close();
}

void GameSaver::loadGame(Entity_Manager*& manager, Field*& field, const std::string& filename, int& current_level) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("The save file was not found!\n");

    json saveData;
    file >> saveData;
    file.close();

    if (saveData.contains("game_info") && saveData["game_info"].contains("current_level")) {
        current_level = saveData["game_info"]["current_level"];
    }

    Field* newField = nullptr;
    Entity_Manager* newManager = nullptr;

    try {
        if (!saveData.contains("field")) throw std::runtime_error("No field data in save file!");

        int width = saveData["field"]["width"];
        int height = saveData["field"]["height"];
        newField = new Field(sf::Vector2u(width, height));

        auto& newCells = newField->get_cells();
        auto& cellsJson = saveData["field"]["cells"];

        for (auto& c : cellsJson) {
            int x = c["x"];
            int y = c["y"];
            int t = c["type"];

            if (x < 0 || x >= width || y < 0 || y >= height) continue;

            delete newCells[x][y];
            newCells[x][y] = nullptr;

            Status st = static_cast<Status>(t);
            switch (st) {
                case ordinary:
                    newCells[x][y] = new OrdinaryCell();
                    break;
                case slow:
                    newCells[x][y] = new SlowingCell();
                    break;
                case impassable:
                    newCells[x][y] = new ImpassableCell();
                    break;
                case trap: {
                    int dmg = 0;
                    if (c.contains("damage")) dmg = int(c["damage"]);
                    newCells[x][y] = new TrapCell(dmg);
                    break;
                }
                default:
                    newCells[x][y] = new OrdinaryCell();
            }

            newCells[x][y]->getSprite().setPosition(
                newField->get_offset().x + x * newField->get_cell_size(),
                newField->get_offset().y + y * newField->get_cell_size()
            );
        }


        newManager = new Entity_Manager(newField, current_level, true);
        sf::Vector2u offsets = newField->get_offset();
        auto& cells = newField->get_cells();

        if (saveData.contains("hero")) {
            int hero_x = saveData["hero"]["position"][0];
            int hero_y = saveData["hero"]["position"][1];
            int hero_hp = saveData["hero"]["hp"];
            int hero_level = saveData["hero"]["level"];
            int hero_total_kill = saveData["hero"]["total_kills"];
            int weapon = saveData["hero"]["weapon"];
            int LevelEnhancementSpell = saveData["hero"]["LevelEnhancementSpell"];

            Hero* hero = new Hero(hero_x, hero_y, offsets.x, offsets.y, hero_level);
            hero->update_damage_rang();
            hero->setLevel(hero_level, true);
            hero->UpgradeEnhancementSpellHero(LevelEnhancementSpell - 1);
            hero->set_helpth(hero_hp);
            hero->setTotalKill(hero_total_kill);
            hero->set_weapon(static_cast<Weapons>(weapon));

            newManager->set_hero(hero);
            hero->updatePosition();

            if (saveData["hero"].contains("spells") || saveData["hero"].contains("unindicted_spells")) {
                auto& spellHand = hero->get_SpellHand();
                std::vector<std::string> loadedSpells;
                std::vector<std::string> loadedUnindictedSpells;

                if (saveData["hero"].contains("spells")) {
                    for (auto& name : saveData["hero"]["spells"]) loadedSpells.push_back(name);
                }
                if (saveData["hero"].contains("unindicted_spells")) {
                    for (auto& name : saveData["hero"]["unindicted_spells"]) loadedUnindictedSpells.push_back(name);
                }

                spellHand.clearAllSpells();
                spellHand.setSpellsByName(loadedSpells);
                spellHand.setUnindictedSpellsByName(loadedUnindictedSpells);
            }

            if (hero_x >= 0 && hero_x < (int)cells.size() && hero_y >= 0 && hero_y < (int)cells[hero_x].size()) {
                cells[hero_x][hero_y]->Cell_send_use(true, newManager->get_hero());
            }
        }

        if (saveData.contains("enemies")) {
            for (auto& e : saveData["enemies"]) {
                std::string type = e["type"];
                int x = e["position"][0];
                int y = e["position"][1];
                int hp = e["hp"];
                int level = e["level"];

                if (x < 0 || x >= (int)cells.size() || y < 0 || y >= (int)cells[x].size()) continue;

                Enemy* enemy = nullptr;
                if (type == "Slime") enemy = new Slime(x, y, offsets.x, offsets.y, level);
                else if (type == "Demon") enemy = new Demon(x, y, offsets.x, offsets.y, level);
                else if (type == "Shadow") enemy = new Shadow(x, y, offsets.x, offsets.y, level);
                else if (type == "Goblin") enemy = new Goblin(x, y, offsets.x, offsets.y, level);
                else {
                    std::cerr << "Unknown enemy type in save: " << type << std::endl;
                    continue;
                }

                
                enemy->setLevel(level, true);
                enemy->set_helpth(hp);
                newManager->get_enemies().push_back(enemy);
                cells[x][y]->Cell_send_use(true, enemy);
            }
        }

        if (saveData.contains("allies")) {
            for (auto& a : saveData["allies"]) {
                std::string type = a["type"];
                int x = a["position"][0];
                int y = a["position"][1];
                int hp = a["hp"];
                int level = a["level"];

                if (x < 0 || x >= (int)cells.size() || y < 0 || y >= (int)cells[x].size()) continue;

                Knight* knight = new Knight(x, y, offsets.x, offsets.y, level);
                
                knight->setLevel(level, true);
                knight->set_helpth(hp);
                newManager->get_allies().push_back(knight);
                cells[x][y]->Cell_send_use(true, knight);
            }
        }

        if (saveData.contains("portal")) {
            int x = saveData["portal"]["position"][0];
            int y = saveData["portal"]["position"][1];
            int hp = saveData["portal"]["hp"];
            int level = saveData["portal"]["level"];
            int frequency = saveData["portal"]["frequency"];

            if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
                Portal* portal = new Portal(x, y, offsets.x, offsets.y, frequency, level);
                
                portal->setLevel(level, true);
                portal->set_helpth(hp);
                newManager->set_portal(portal);
                cells[x][y]->Cell_send_use(true, portal);
            }
        }

        if (saveData.contains("statue")) {
            int x = saveData["statue"]["position"][0];
            int y = saveData["statue"]["position"][1];
            int hp = saveData["statue"]["hp"];
            int level = saveData["statue"]["level"];

            if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
                MagicStatue* statue = new MagicStatue(x, y, offsets.x, offsets.y, level);
                
                statue->setLevel(level, true);
                statue->set_helpth(hp);
                newManager->set_statue(statue);
                cells[x][y]->Cell_send_use(true, statue);
            }
        }

        delete manager;
        delete field;

        manager = newManager;
        field = newField;

        std::cout << "Game loaded successfully. Level: " << current_level
                  << ", Field size: " << cells.size() << "x" << (cells.empty() ? 0 : cells[0].size()) << std::endl;
    }
    catch (const std::exception& ex) {
        delete newManager;
        delete newField;
        throw;
    }
}
