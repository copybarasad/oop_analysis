#include "save_system.h"
#include <iostream>
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"

SaveSystem::SaveSystem(const std::string& file) : filename(".//saves/" + file) {}

void SaveSystem::saveGame(GeneralManager& manager, int level) {
    std::ofstream file(filename);
    if (!file) throw std::runtime_error("Cannot open save file: " + filename);

    try {
        file << "LEVEL " << level << "\n";
        
        Field& field = manager.getField();
        file << "FIELD " << field.getWidth() << " " << field.getHeight() << "\n";
        
        auto cells = field.getCellsData();
        for (const auto& cell : cells) {
            file << std::get<0>(cell) << " " << std::get<1>(cell) << " " 
                 << static_cast<int>(std::get<2>(cell)) << " " << std::get<3>(cell) << "\n";
        }
        
        PlayerManager& playerMan = manager.getPlayerManager();
        auto pos = playerMan.getPlayerPosition();
        file << "PLAYER " << pos.first << " " << pos.second << " "
             << playerMan.getPlayerHealth() << " " << playerMan.getPlayerDamage() << " "
             << playerMan.getPlayerPoints() << " " << playerMan.getPlayerFightType() << "\n";
        
        auto& spellHand = playerMan.getSpellHandRef();
        auto spells = spellHand.getSpellsData();
        file << "SPELLS " << spells.size() << "\n";
        for (const auto& spell : spells) {
            file << std::get<0>(spell) << " " << std::get<1>(spell) << " "
            << std::get<2>(spell) << " " << std::get<3>(spell) << "\n";
        }
        
        auto enemies = manager.getEnemyManager().getEnemiesData();
        file << "ENEMIES " << enemies.size() << "\n";
        for (const auto& enemy : enemies) {
            file << std::get<0>(enemy) << " " << std::get<1>(enemy) << " "
                 << std::get<2>(enemy) << " " << std::get<3>(enemy) << "\n";
        }
        
        auto towers = manager.getTowerManager().getTowersData();
        file << "TOWERS " << towers.size() << "\n";
        for (const auto& tower : towers) {
            file << std::get<0>(tower) << " " << std::get<1>(tower) << " "
                 << std::get<2>(tower) << " " << std::get<3>(tower) << " "
                 << std::get<4>(tower) << "\n";
        }

    } catch (const std::exception& e) {
        throw std::runtime_error("Save failed: " + std::string(e.what()));
    }
}

bool SaveSystem::loadGame(GeneralManager& manager, int& level) {
    try {
        std::ifstream file(filename);
        if (!file){
            throw std::invalid_argument("File: " + filename + " not found");
        }
        int check = 0;
        manager = GeneralManager();
        
        std::string type;
        
        file >> type;
        if (type != "LEVEL") throw std::invalid_argument("Expected LEVEL, got: " + type);
        check++;
        file >> level;
        if (level < 1) throw std::invalid_argument("Incorrect level numerate");
        
        file >> type;
        if (type != "FIELD") throw std::invalid_argument("Expected FIELD, got: " + type);
        check++;
        int width, height;
        file >> width >> height;
        manager.setField(new Field(width, height));
        
        std::vector<std::tuple<int, int, CellType, int>> cellsData;
        for (int i = 0; i < width * height; i++) {
            int x, y, typeInt, trapDamage;
            file >> x >> y >> typeInt >> trapDamage;
            cellsData.emplace_back(x, y, static_cast<CellType>(typeInt), trapDamage);
        }
        manager.getField().loadCellsData(cellsData);
        
        file >> type;
        if (type != "PLAYER") throw std::invalid_argument("Expected PLAYER, got: " + type);
        check++;
        int x, y, health, damage, points, fightType;
        file >> x >> y >> health >> damage >> points >> fightType;
        Player* player = new Player(health, damage);
        player->addPoints(points);
        if (fightType != player->getFightType()) {
            player->switchFightType();
        }
        Field& f = manager.getField();
        if ((x < 0 || y < 0 || f.getWidth() < x || f.getHeight() < y)) {
            throw std::invalid_argument("Incorrect player coordinates");
        }
        manager.getPlayerManager().setPlayer(player, x, y);
        
        file >> type;
        if (type != "SPELLS") throw std::invalid_argument("Expected SPELLS, got: " + type);
        check++;
        int count;
        file >> count;
        auto& spellHand = manager.getPlayerManager().getSpellHandRef();
        for (int i = 0; i < count; i++) {
            std::string name;
            int damage, cost, radius;
            file >> name >> damage >> cost >> radius;
            spellHand.addSpellByName(name, damage, cost, radius);
        }
        
        file >> type;
        if (type != "ENEMIES") throw std::invalid_argument("Expected ENEMIES, got: " + type);
        check++;
        file >> count;
        for (int i = 0; i < count; i++) {
            file >> x >> y >> health >> damage;
            Enemy* enemy = new Enemy(health, damage);
            if ((x < 0 || y < 0 || f.getWidth() < x || f.getHeight() < y)) {
                throw std::invalid_argument("Incorrect enemy coordinates");
            }
            manager.getEnemyManager().addEnemy(enemy, x, y);
        }
        
        file >> type;
        if (type != "TOWERS") throw std::invalid_argument("Expected TOWERS, got: " + type);
        check++;
        file >> count;
        for (int i = 0; i < count; i++) {
            int radius;
            file >> x >> y >> health >> damage >> radius;
            Tower* tower = new Tower(health, radius, {x, y}, damage);
            manager.getTowerManager().addTower(tower);
        }
        
        if (file >> type) {
            throw std::invalid_argument("Extra data after TOWERS: " + type);
        }
        
        if (check != 6) {
            throw std::invalid_argument("Incorrect structure for savefile");
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Load failed: " << e.what() << std::endl;
        return false;
    }
}

bool SaveSystem::saveExists() const {
    std::ifstream file(filename);
    return file.good();
}

void SaveSystem::setName(const std::string& name){
    filename = "../saves/" + name;
}