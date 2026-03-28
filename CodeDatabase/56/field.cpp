#include "field.h"
#include <iostream>
#include <utility>
#include "combat_system.h"
#include <cmath>
#include "freezy_system.h"


Field::Field(const Field& other) {
    width = other.width;
    height = other.height;
    table = other.table;  
    
    if (other.gamer) {
        gamer = std::make_unique<Player>(
            other.gamer->getX(), 
            other.gamer->getY(), 
            other.gamer->getHP(), 
            other.gamer->getDamage(),
            0,
            100,
            100
        );
        gamer->setTypeCombat(other.gamer->getTypeCombat());
        gamer->setRange(other.gamer->getRange());
    }
    
    buildings.clear();
    for (
        auto& building : other.buildings) {
        buildings.push_back(std::make_unique<Building>(*building));
    }
    
    enemies.clear();
    for (auto& enemy : other.enemies) {
        enemies.push_back(std::make_unique<Enemy>(*enemy));
    }
}

Field& Field::operator=(const Field& other) {
    if (this != &other) { 
        width = other.width;
        height = other.height;
        table = other.table;
        
        if (other.gamer) {
            gamer = std::make_unique<Player>(
                other.gamer->getX(), 
                other.gamer->getY(), 
                other.gamer->getHP(), 
                other.gamer->getDamage(),
                0,
                100,
                100
            );
            gamer->setTypeCombat(other.gamer->getTypeCombat());
            gamer->setRange(other.gamer->getRange());
        } else {
            gamer.reset();
        }
        
        buildings.clear();
        for (auto& building : other.buildings) {
            buildings.push_back(std::make_unique<Building>(*building));
        }
        
        enemies.clear();
        for (auto& enemy : other.enemies) {
            enemies.push_back(std::make_unique<Enemy>(*enemy));
        }
    }
    return *this;
}

Field::Field(Field& other) {
    width = other.width;
    height = other.height;
    table = std::move(other.table);
    gamer = std::move(other.gamer);
    buildings = std::move(other.buildings);
    enemies = std::move(other.enemies);
    
    other.width = 0;
    other.height = 0;
}

Field& Field::operator=(Field& other) {
    if (this != &other) {
        buildings.clear();
        enemies.clear();
        gamer.reset();
        
        width = other.width;
        height = other.height;
        table = std::move(other.table);
        gamer = std::move(other.gamer);
        buildings = std::move(other.buildings);
        enemies = std::move(other.enemies);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

Field::Field(int w, int h) : width(w), height(h) {
    table.resize(height);
    for (int i = 0; i < height; i++) {
        table[i].resize(width);
    }
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            table[i][j].setType(PLACE_EMPTY);
        }
    }
    
    int cnt_freeze = random_numb(MIN_FREEZE_PLACE, MAX_FREEZE_PLACE);
    for (int i = 0; i < cnt_freeze; i++) {
        int x_freeze = random_numb(0, w-1);
        int y_freeze = random_numb(0, h-1);
        while (table[y_freeze][x_freeze].getEffect() == EFFECT_FREEZE) {
            x_freeze = random_numb(0, w-1);
            y_freeze = random_numb(0, h-1);
        }
        table[y_freeze][x_freeze].setEffect(EFFECT_FREEZE);
    }

    int cnt_wall = random_numb(MIN_WALL_PLACE, MAX_WALL_PLACE);
    for (int i = 0; i < cnt_wall; i++) {
        int x_wall = random_numb(0, w-1);
        int y_wall = random_numb(0, h-1);
        while (table[y_wall][x_wall].getEffect() == EFFECT_WALL) {
            x_wall = random_numb(0, w-1);
            y_wall = random_numb(0, h-1);
        }
        table[y_wall][x_wall].setEffect(EFFECT_WALL);
    }
    
    int x_pl = random_numb(0, width-1);
    int y_pl = random_numb(0, height-1);
    gamer = std::make_unique<Player>(x_pl, y_pl, PLAYER_HP, PLAYER_MELEE_DAMAGE, PLAYER_POINTS, PLAYER_MANA, PLAYER_HP);
    while (table[y_pl][x_pl].getEffect() == EFFECT_WALL) {
        x_pl = random_numb(0, width-1);
        y_pl = random_numb(0, height-1);
    }
    table[y_pl][x_pl].setType(PLACE_PLAYER);
}

Field::Field() : width(DEFAULT_WIDTH), height(DEFAULT_HEIGHT) {
    table.resize(height);
    for (int i = 0; i < height; i++) {
        table[i].resize(width);
    }
    
    int cnt_freeze = random_numb(MIN_FREEZE_PLACE, MAX_FREEZE_PLACE);
    for (int i = 0; i < cnt_freeze; i++) {
        int x_freeze = random_numb(0, width-1);
        int y_freeze = random_numb(0, height-1);
        while (table[y_freeze][x_freeze].getEffect() == EFFECT_FREEZE) {
            x_freeze = random_numb(0, width-1);
            y_freeze = random_numb(0, height-1);
        }
        table[y_freeze][x_freeze].setEffect(EFFECT_FREEZE);
    }
    
    int cnt_wall = random_numb(MIN_WALL_PLACE, MAX_WALL_PLACE);
    for (int i = 0; i < cnt_wall; i++) {
        int x_wall = random_numb(0, width-1);
        int y_wall = random_numb(0, height-1);
        while (table[y_wall][x_wall].getEffect() == EFFECT_WALL) {
            x_wall = random_numb(0, width-1);
            y_wall = random_numb(0, height-1);
        }
        table[y_wall][x_wall].setEffect(EFFECT_WALL);
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            table[i][j].setType(PLACE_EMPTY);
        }
    }
    
    int x_pl = random_numb(0, width-1);
    int y_pl = random_numb(0, height-1);
    gamer = std::make_unique<Player>(x_pl, y_pl, PLAYER_HP, PLAYER_MELEE_DAMAGE, PLAYER_POINTS, PLAYER_MANA, PLAYER_HP);
    while (table[y_pl][x_pl].getEffect() == EFFECT_WALL) {
        x_pl = random_numb(0, width-1);
        y_pl = random_numb(0, height-1);
    }
    table[y_pl][x_pl].setType(PLACE_PLAYER);
}


nlohmann::json Field::toJson() const {
    std::vector<nlohmann::json> enemiesJson;
    for (const auto& enemy : enemies) {
        enemiesJson.push_back(enemy->toJson());
    }
    
    std::vector<nlohmann::json> buildingsJson;
    for (const auto& building : buildings) {
        buildingsJson.push_back(building->toJson());
    }
    
    std::vector<nlohmann::json> towersJson;
    for (const auto& tower : spell_towers) {
        towersJson.push_back(tower->toJson());
    }
    
    std::vector<std::vector<nlohmann::json>> tableJson;
    for (const auto& row : table) {
        std::vector<nlohmann::json> rowJson;
        for (const auto& cell : row) {
            rowJson.push_back(cell.toJson());
        }
        tableJson.push_back(rowJson);
    }
    
    return {
        {"width", width},
        {"height", height},
        {"player", gamer->toJson()},
        {"enemies", enemiesJson},
        {"buildings", buildingsJson},
        {"spell_towers", towersJson},
        {"table", tableJson}
    };
}

void Field::setPlayer(std::unique_ptr<Player> fighter){
    if (gamer) {
        int old_x = gamer->getX();
        int old_y = gamer->getY();
        if (old_x >= 0 && old_x < width && old_y >= 0 && old_y < height) {
            table[old_y][old_x].setType(PLACE_EMPTY);
        }
    }
    gamer = std::move(fighter);
    int new_x = gamer->getX();
    int new_y = gamer->getY();
    
    if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height) {
        table[new_y][new_x].setType(PLACE_PLAYER);
    }
}


void Field::fromJson(const nlohmann::json& j) {
    width = j["width"];
    height = j["height"];
    
    gamer->fromJson(j["player"]);
    
    enemies.clear();
    for (const auto& enemyJson : j["enemies"]) {
        auto enemy = std::make_unique<Enemy>(0, 0, 0, 0, 0); 
        enemy->fromJson(enemyJson);
        enemies.push_back(std::move(enemy));
    }
    
    buildings.clear();
    for (const auto& buildingJson : j["buildings"]) {
        auto building = std::make_unique<Building>(0, 0, 0, 0, 0);
        building->fromJson(buildingJson);
        buildings.push_back(std::move(building));
    }
    
    spell_towers.clear();
    for (const auto& towerJson : j["spell_towers"]) {
        auto tower = std::make_unique<SpellTower>(0, 0, 0, "", 0, 0, 0);
        tower->fromJson(towerJson);
        spell_towers.push_back(std::move(tower));
    }
    
    table.clear();
    table.resize(height);
    for (int i = 0; i < height; i++) {
        table[i].resize(width);
        for (int t = 0; t < width; t++) {
            table[i][t].fromJson(j["table"][i][t]);
        }
    }
}

std::string Field::getEffectPlace(int x, int y){
    return table[y][x].getEffect();
}
void Field::delEffectPlace(int x, int y){
    table[y][x].setEffect("none");
}
void Field::setEffectPlace(int x, int y, std::string ef){
    table[y][x].setEffect(ef);
}
const std::vector<std::unique_ptr<Enemy>>& Field::getEnemies() { return enemies; }
int Field::getWidth() { return width; }
int Field::getHeight() { return height; }
int Field::getCntEnemy() { return enemies.size(); }
Player* Field::getPlayer() { return gamer.get(); }
int Field::getCntBuilding() { return buildings.size(); }


void Field::setTypePlace(int x, int y, int type){
    return table[y][x].setType(type);
}

int Field::getTypePlace(int x, int y){
    return table[y][x].getType();
}

bool Field::Place_is_valid(int new_x, int new_y){
    if (table[new_y][new_x].getEffect() != EFFECT_WALL && table[new_y][new_x].getType() == PLACE_EMPTY){
        return true;
    }
    return false;
}

SpellTower* Field::getSpellTower(int id){
    if (id >= 0 && id < spell_towers.size()) {
        return spell_towers[id].get();
    }
    return nullptr;
}
Building* Field::getBuild(int id){
    if (id >= 0 && id < buildings.size()) {
        return buildings[id].get();
    }
    return nullptr;
}

void Field::addEnemy(std::unique_ptr<Enemy> enemy) {
    if (!enemy) return;
    
    int x = enemy->getX();
    int y = enemy->getY();
    
    if (Place_is_valid(x, y)) {
        table[y][x].setType(PLACE_ENEMY);
        enemies.push_back(std::move(enemy)); 
    }
}
void Field::switch_ranged() { 
    gamer->setTypeCombat(COMBAT_RANGED);
    gamer->setDamage(PLAYER_RANGED_DAMAGE);
    gamer->setRange(RANGED_RANGE);
}
void Field::switch_melee() { 
    gamer->setTypeCombat(COMBAT_MELEE);
    gamer->setDamage(PLAYER_MELEE_DAMAGE);
    gamer->setRange(MELEE_RANGE);
}

void Field::initilization_field(int parametr) {
    int cnt_enemy = random_numb(MIN_ENEMIES, MAX_ENEMIES);
    for (int i = 0; i < cnt_enemy; i++) {
        int x_pos = random_numb(0, width - 1);
        int y_pos = random_numb(0, height - 1);

        while (table[y_pos][x_pos].getType() != PLACE_EMPTY || table[y_pos][x_pos].getEffect() == EFFECT_WALL) {
            x_pos = random_numb(0, width - 1);
            y_pos = random_numb(0, height - 1);
        }
        
        table[y_pos][x_pos].setType(PLACE_ENEMY);
        enemies.push_back(std::make_unique<Enemy>(x_pos, y_pos, ENEMY_HP * parametr, ENEMY_DAMAGE, ENEMY_STRENGTH));
    }
    
    int cnt_building = random_numb(MIN_BUILDINGS, MAX_BUILDINGS);
    for (int i = 0; i < cnt_building; i++) {
        int x_pos = random_numb(0, width - 1);
        int y_pos = random_numb(0, height - 1);

        while (table[y_pos][x_pos].getType() != PLACE_EMPTY || table[y_pos][x_pos].getEffect() == EFFECT_WALL) {
            x_pos = random_numb(0, width - 1);
            y_pos = random_numb(0, height - 1);
        }
        table[y_pos][x_pos].setType(PLACE_BUILDING);
        buildings.push_back(std::make_unique<Building>(x_pos, y_pos, BUILDING_HP, BUILDING_COOLDOWN, BUILDING_SPAWN_TIMER));
    }
    int cnt_spell_tower = random_numb(MIN_SPELL_TOWER, MAX_SPELL_TOWER);
    for (int i = 0; i < cnt_spell_tower; i++) {
        int x_pos = random_numb(0, width - 1);
        int y_pos = random_numb(0, height - 1);

        while (table[y_pos][x_pos].getType() != PLACE_EMPTY || table[y_pos][x_pos].getEffect() == EFFECT_WALL) {
            x_pos = random_numb(0, width - 1);
            y_pos = random_numb(0, height - 1);
        }
        table[y_pos][x_pos].setType(PLACE_SPELL_TOWER);
        spell_towers.push_back(std::make_unique<SpellTower>(x_pos, y_pos, SPELL_TOWER_HP, "cast", SPELL_TOWER_RANGED, 0, SPELL_TOWER_TIME));
    }
}
int Field::getCntSpellTower(){
    return spell_towers.size();
}
bool Field::attack_player(int dx, int dy) {
    return CombatSystem::player_attack(gamer.get(), *this, dx, dy);
}
void Field::removeEnemy(int index) {
    if (index >= 0 && static_cast<size_t>(index) < enemies.size()) {
        enemies.erase(enemies.begin() + index);
    }
}

void Field::removeBuilding(int index) {
    if (index >= 0 && static_cast<size_t>(index) < buildings.size()) {
        buildings.erase(buildings.begin() + index);
    }
}
void Field::removeSpellTower(int index) {
    if (index >= 0 && static_cast<size_t>(index) < spell_towers.size()) {
        spell_towers.erase(spell_towers.begin() + index);
    }
}
void Field::print_field() {
    int height = table.size();
    int width = table[0].size();
    int row_width = width * 3 - 1;
    
    std::cout << "┌";
    for (int j = 0; j < row_width; j++) std::cout << "─";
    std::cout << "┐";
    std::cout << "\n";
    
    for (int i = 0; i < height; i++) {
        std::cout << "│";
        for (int j = 0; j < width; j++) {
            if (table[i][j].getType() == PLACE_PLAYER) {
                std::cout << "\033[1;32mP\033[0m";
            }
            else if (table[i][j].getType() == PLACE_ENEMY) {
                std::cout << "\033[1;31mE\033[0m";
            } 
            else if (table[i][j].getType() == PLACE_BUILDING) {
                std::cout << "\033[1;33mT\033[0m";
            }
            else if (table[i][j].getType() == PLACE_SPELL_TOWER) {
                std::cout << "\033[1;35mS\033[0m";
            }
            else if (table[i][j].getType() == PLACE_EMPTY) {
                if (table[i][j].getEffect() == EFFECT_WALL) {
                    std::cout << "\033[1;36mW\033[0m";
                }
                else if (table[i][j].getEffect() == EFFECT_FREEZE) {
                    std::cout << "\033[1;34mF\033[0m";
                }
                else if (table[i][j].getEffect() == "trap") {
                    std::cout << "\033[1;31mM\033[0m";
                }
                else {
                    std::cout << "·";
                }
            }
            
            if (j < width - 1) std::cout << " ";
        }
        std::cout << "│";
        
        if (i == 0) {
            std::cout << "     🔴 Враги: " << getCntEnemy();
        }
        else if (i == 1) {
            std::cout << "     🏠 Здания: " << getCntBuilding();
        }
        else if (i == 2) {
            std::cout << "     🏰 Башни: " << getCntSpellTower();
        }

        
        std::cout << "\n";
    }
    
    std::cout << "└";
    for (int j = 0; j < row_width; j++) std::cout << "─";
    std::cout << "┘\n";
}