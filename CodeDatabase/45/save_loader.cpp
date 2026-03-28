#include "save_loader.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

namespace {
    bool is_digits(const std::string &str) {
        return !str.empty() && str.find_first_not_of("0123456789") == std::string::npos;
    }

    std::vector<std::string> splitString(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        
        while (std::getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    int countLinesUniversal(std::ifstream& file) {
        int lineCount = 0;
        char prevChar = '\0';
        char currentChar;
        
        file.clear();
        file.seekg(0);
        
        while (file.get(currentChar)) {
            if (currentChar == '\n') {
                lineCount++;
            }
            prevChar = currentChar;
        }
        
        if (prevChar != '\0' && prevChar != '\n') {
            lineCount++;
        }
        
        file.clear();
        file.seekg(0);
        return lineCount;
    }
}

SaveLoader::SaveLoader() {

}

void SaveLoader::load_error(std::string error) {
    std::cout << "Error: " << error << "\n";
}

void SaveLoader::save(std::string filename, Map* map, Hand* hand, int level) {
    std::ofstream saveFile(filename, std::ios::binary);
    
    if (!saveFile.is_open()) {
        std::cout << "Error: cannot save game\n";
        return;
    }

    saveFile << level << "\n";
    saveFile << hand->handToString();
    saveFile << map->getHeight() << ";" << map->getWidth() << "\n";
    for (int i = 0; i < map->getHeight(); i++) {
        for (int j = 0; j < map->getWidth(); j++) {
            saveFile << map->getCellStatusAt(j, i) << ";";
        }
        saveFile << "\n";
    }
}

bool SaveLoader::load(std::string filename, Map*& map, Hand*& hand, Player*& player, int* level) {
    std::ifstream loadFile(filename, std::ios::binary);
    
    if (!loadFile.is_open()) {
        load_error("File not exist");
        return false;
    }

    std::string line;
    int lines = countLinesUniversal(loadFile);

    // Загружаем уровень
    std::getline(loadFile, line);
    if (!is_digits(line)) {
        load_error("Incorrect current level");
        return false;
    }
    *level = std::stoi(line);

    // Загружаем capacity руки
    std::getline(loadFile, line);
    if (!is_digits(line)) {
        load_error("Incorrect hand capacity");
        return false;
    }
    int capacity = std::stoi(line);
    
    // Очищаем старую руку и создаем новую
    delete hand;
    hand = new Hand(capacity);

    // Загружаем заклинания
    std::getline(loadFile, line);
    if (static_cast<size_t>(capacity) < line.length()) {
        load_error("Too many spells");
        return false;  // Исправлено: было return без значения
    }
    
    for (size_t i = 0; i < line.length(); i++) {
        char sp = line[i];
        switch (sp) {
            case 'D':
                hand->addSpell(std::make_unique<DirectSpell>());
                break;
            case 'A':
                hand->addSpell(std::make_unique<AreaSpell>());
                break;
            case 'T':
                hand->addSpell(std::make_unique<TrapSpell>());
                break;
            default:
                load_error("Incorrect spell type");
                return false;
        }
    }

    // Загружаем размеры карты
    std::getline(loadFile, line);
    size_t pos = line.find(';');
    if (pos == std::string::npos) {
        load_error("Incorrect map dimensions format");
        return false;
    }
    
    int height = std::stoi(line.substr(0, pos));
    int width = std::stoi(line.substr(pos + 1));

    // Очищаем старую карту и создаем новую
    delete map;
    map = new Map(width, height);

    // Проверяем количество строк
    if (lines - 4 != map->getHeight()) {
        load_error("Incorrect rows count");
        return false;
    }

    // Загружаем данные карты
    for (int i = 0; i < map->getHeight(); i++) {
        std::getline(loadFile, line);
        std::vector<std::string> line_split = splitString(line, ';');
        
        if (line_split.size() != static_cast<size_t>(map->getWidth())) {
            load_error("Incorrect cols count");
            return false;
        }
        
        for (int j = 0; j < map->getWidth(); j++) {
            std::unique_ptr<Entity> entity;
            CellState cs = CellState::EMPTY;
            std::vector<std::string> cell_split = splitString(line_split[j], ',');
            
            if (cell_split[0] == "Empty") {
                map->setWallAt(j, i, false);
            } else if (cell_split[0] == "Wall") {
                map->setWallAt(j, i, true);
            } else if (cell_split[0] == "Player") {
                if (cell_split.size() != 5 || !(is_digits(cell_split[1]) && is_digits(cell_split[2]) && is_digits(cell_split[3])) || !(cell_split[4].length() == 1)) {
                    load_error("Incorrect player data");
                    return false;
                }
                
                int player_hp = std::stoi(cell_split[1]);
                int player_damage = std::stoi(cell_split[2]);
                int player_max_hp = std::stoi(cell_split[3]);
                DamageType dt;
                
                if (cell_split[4] == "M") {
                    dt = DamageType::MELEE;
                } else if (cell_split[4] == "R") {
                    dt = DamageType::RANGE;
                    player_damage *= 2;
                } else {
                    load_error("Incorrect damage type");
                    return false;
                }
                
                // Очищаем старого игрока и создаем нового
                delete player;
                player = new Player(player_hp, player_damage, player_max_hp);
                player->setDamageType(dt);
                player->set_hand(hand);
                map->addPlayer(j, i, player);
                
            } else if (cell_split[0] == "Monster") {
                if (cell_split.size() != 3 || !(is_digits(cell_split[1]) && is_digits(cell_split[2]))) {
                    load_error("Incorrect monster data");
                    return false;
                }
                int monster_hp = std::stoi(cell_split[1]);
                int monster_damage = std::stoi(cell_split[2]);
                entity = std::make_unique<Monster>(monster_hp, monster_damage);
                cs = CellState::MONSTER;
                
            } else if (cell_split[0] == "Trap") {
                map->setTrapAt(j, i);
                
            } else if (cell_split[0] == "GuardTower") {
                if (cell_split.size() != 4 || !(is_digits(cell_split[1]) && is_digits(cell_split[2]) && is_digits(cell_split[3]))) {
                    load_error("Incorrect tower data");
                    return false;
                }
                int tower_hp = std::stoi(cell_split[1]);
                int tower_damage = std::stoi(cell_split[2]);
                int charge = std::stoi(cell_split[3]);
                entity = std::make_unique<GuardTower>(tower_hp, tower_damage);
                
                for (int k = 0; k < charge; k++) {
                    entity->step(nullptr, -1, -1);
                }
                cs = CellState::TOWER;
                
            } else {
                load_error("Incorrect cell type");
                return false;
            }
            
            if (entity != nullptr) {
                map->addEntity(j, i, std::move(entity), cs);
            }
        }
    }

    return true;
}