#include "SaveManager.h"
#include "GameExceptions.h"
#include "Spell.h"
#include "Tower.h"
#include "Trap.h"
#include "Shield.h"
#include "ThornsArea.h"
#include "Ally.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

// --- Helper: Convert Objects to JSON ---

void SaveManager::saveGame(const Game& game, const std::string& filename) {
    std::string jsonFilename = filename;
    // FORCE .json extension logic
    if (jsonFilename.find(".dat") != std::string::npos) {
        jsonFilename.replace(jsonFilename.find(".dat"), 4, ".json");
    } else if (jsonFilename.find(".json") == std::string::npos) {
        if (jsonFilename.find(".") != std::string::npos) 
            jsonFilename = jsonFilename.substr(0, jsonFilename.find_last_of('.')) + ".json";
        else 
            jsonFilename += ".json";
    }

    json j;
    j["version"] = 2;
    j["level_index"] = game.m_currentLevelIndex;
    j["state"] = (int)game.m_gameState;
    
    // Player
    Player* p = game.getPlayer();
    j["player"]["score"] = p->getScore();
    j["player"]["pos"] = { p->getX(), p->getY() };
    j["player"]["stats"] = { 
        {"hp", p->getHealth()}, 
        {"max_hp", p->getMaxHealth()},
        {"mana", p->getMana()}, 
        {"max_mana", p->getMaxMana()} 
    };
    j["player"]["damage"] = { {"melee", p->getMeleeDamage()} };
    
    // Spells
    j["player"]["spells"] = json::array();
    for (const auto* s : p->getHand().getAllSpells()) {
        j["player"]["spells"].push_back({
            {"name", s->getName()},
            {"level", s->getLevel()}
        });
    }

    // Map Info
    j["map"]["width"] = game.m_field->getWidth();
    j["map"]["height"] = game.m_field->getHeight();
    j["map"]["floor_texture"] = game.m_field->getFloorTexture();

    // World Objects
    j["objects"] = json::array();
    for (const auto* obj : game.getWorldObjects()) {
        sf::Color c = obj->getColor();
        j["objects"].push_back({
            {"type", (int)obj->getType()},
            {"x", obj->getX()},
            {"y", obj->getY()},
            {"color", {c.r, c.g, c.b, c.a}}
        });
    }

    // Enemies
    j["enemies"] = json::array();
    for (const auto* e : game.getEnemies()) {
        if (!e->isAlive()) continue;
        j["enemies"].push_back({
            {"type", (int)e->getType()},
            {"x", e->getX()},
            {"y", e->getY()},
            {"hp", e->getHealth()},
            {"damage", e->getDamage()},
            {"name", e->getName()}
        });
    }

    // Towers
    j["towers"] = json::array();
    for (const auto* t : game.getTowers()) {
        j["towers"].push_back({
            {"x", t->getX()},
            {"y", t->getY()},
            {"damage", t->getDamage()},
            {"range", t->getAttackRange()}
        });
    }

    std::ofstream o(jsonFilename);
    if (!o.is_open()) throw SaveException("Failed to open file for writing.");
    o << j.dump(4);
}

void SaveManager::loadGame(Game& game, const std::string& filename) {
    std::string jsonFilename = filename;
    
    // --- CRITICAL FIX: AGGRESSIVELY FORCE JSON EXTENSION ---
    // Even if "savegame.dat" is passed (default argument), we switch to "savegame.json"
    if (jsonFilename.find(".dat") != std::string::npos) {
        jsonFilename.replace(jsonFilename.find(".dat"), 4, ".json");
    } else if (jsonFilename.find(".json") == std::string::npos) {
        jsonFilename += ".json";
    }
    // -----------------------------------------------------

    std::ifstream i(jsonFilename);
    if (!i.is_open()) throw FileOpenException(jsonFilename);

    json j;
    try {
        i >> j;
    } catch (const std::exception& e) {
        throw CorruptDataException("JSON Parse Error: " + std::string(e.what()));
    }

    // Clear current state
    for(auto* e : game.m_enemies) delete e; game.m_enemies.clear();
    for(auto* o : game.m_worldObjects) delete o; game.m_worldObjects.clear();
    for(auto* t : game.m_towers) delete t; game.m_towers.clear();
    for(auto* tr : game.m_traps) delete tr; game.m_traps.clear();
    for(auto* s : game.m_shields) delete s; game.m_shields.clear();
    for(auto* a : game.m_allies) delete a; game.m_allies.clear();
    for(auto* th : game.m_thornsAreas) delete th; game.m_thornsAreas.clear();

    // Basic Info
    try {
        game.m_currentLevelIndex = j.at("level_index");
        game.m_gameState = (Game::State)j.at("state");
        
        // Map Recreation
        int w = j["map"]["width"];
        int h = j["map"]["height"];
        std::string floorTex = j["map"]["floor_texture"];
        delete game.m_field;
        game.m_field = new GameField(w, h);
        game.m_field->setFloorTexture(floorTex);

        // Player
        auto& pJson = j["player"];
        game.m_player->addScore(pJson.value("score", 0));
        game.m_player->teleport(pJson["pos"][0], pJson["pos"][1]);
        game.m_player->setStats(
            pJson["stats"]["hp"], pJson["stats"]["max_hp"],
            pJson["stats"]["mana"], pJson["stats"]["max_mana"]
        );
        
        int loadedDmg = pJson["damage"]["melee"];
        while(game.m_player->getMeleeDamage() < loadedDmg) game.m_player->upgradeMelee();

        // Spells
        game.m_player->clearHand();
        for (const auto& s : pJson["spells"]) {
            std::string sName = s["name"];
            int sLvl = s["level"];
            
            SpellType t = SpellType::FIREBALL; 
            if (sName.find("Ice") != std::string::npos) t = SpellType::ICE_SHARD;
            else if (sName.find("Heal") != std::string::npos) t = SpellType::HEAL;
            else if (sName.find("Lightning") != std::string::npos) t = SpellType::LIGHTNING;
            else if (sName.find("Shield") != std::string::npos) t = SpellType::SHIELD;
            else if (sName.find("Trap") != std::string::npos) t = SpellType::TRAP;
            else if (sName.find("Thorns") != std::string::npos) t = SpellType::THORNS;
            else if (sName.find("Teleport") != std::string::npos) t = SpellType::TELEPORT;
            else if (sName.find("Nova") != std::string::npos) t = SpellType::NOVA;
            else if (sName.find("Drain") != std::string::npos) t = SpellType::LIFEDRAIN;
            else if (sName.find("Ally") != std::string::npos) t = SpellType::SUMMON_ALLY;
            else if (sName.find("Poison") != std::string::npos) t = SpellType::POISON_CLOUD;
            else if (sName.find("Time") != std::string::npos) t = SpellType::TIME_STOP;
            else if (sName.find("Meteor") != std::string::npos) t = SpellType::METEOR;
            else if (sName.find("Rage") != std::string::npos) t = SpellType::RAGE;

            Spell* newSpell = new Spell(t);
            for(int k=1; k<sLvl; ++k) newSpell->upgrade();
            game.m_player->addToHand(newSpell);
        }

        // World Objects
        for (const auto& obj : j["objects"]) {
            int r = obj["color"][0], g = obj["color"][1], b = obj["color"][2], a = obj["color"][3];
            WorldObject* newObj = new WorldObject(
                obj["x"], obj["y"], 
                (WorldObject::Type)obj["type"], 
                sf::Color(r, g, b, a)
            );
            game.m_worldObjects.push_back(newObj);
            newObj->drawOnField(*game.m_field);
        }

        // Enemies
        for (const auto& e : j["enemies"]) {
            Enemy* newEnemy = new Enemy(
                e["x"], e["y"], 
                (Enemy::Type)e["type"], 
                e["name"], 
                e["hp"], 
                e["damage"]
            );
            newEnemy->setGameField(game.m_field);
            game.m_enemies.push_back(newEnemy);
        }

        // Towers
        for (const auto& t : j["towers"]) {
            Tower* newTower = new Tower(
                t["x"], t["y"], 
                t["damage"], 
                t["range"], 
                2000
            );
            newTower->setGameField(game.m_field);
            game.m_towers.push_back(newTower);
            for(const auto& p : newTower->getBlocks())
                if(game.m_field->isInBounds(p.first, p.second))
                    game.m_field->getCell(p.first, p.second).setType(Cell::WALL);
        }

        // Hub Special
        if (game.m_gameState == Game::State::HUB) {
            delete game.m_portal;
            delete game.m_shop;
            game.m_portal = new Portal(10, 2);
            game.m_shop = new Shop(4, 7);
        }

    } catch (const json::exception& e) {
        throw CorruptDataException("JSON Key Error: " + std::string(e.what()));
    }
}

bool SaveManager::saveFileExists(const std::string& filename) {
    std::string checkName = filename;
    // Force check .json if .dat is passed
    if (checkName.find(".dat") != std::string::npos) {
        checkName.replace(checkName.find(".dat"), 4, ".json");
    } else if (checkName.find(".json") == std::string::npos) {
        checkName += ".json";
    }
    
    std::ifstream f(checkName);
    return f.good();
}
