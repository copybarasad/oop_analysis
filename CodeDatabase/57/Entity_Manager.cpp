#include "Entity_Manager.hpp"
#include <random>      
#include <algorithm>   

Entity_Manager::Entity_Manager(Field* field, int level, bool from_file) : field(field), portal(nullptr), hero(nullptr), statue(nullptr) {
    this->field = field;
    initializeEntities(level, from_file);
}

Entity_Manager::~Entity_Manager() {
    clearAllEntities(true);
}

void Entity_Manager::removeDeadEnemies() {
    auto cells = field->get_cells();
    if(statue != nullptr && statue->get_helpth() <= 0) {
        cells[statue->get_location().x][statue->get_location().y]->Cell_send_use(false);
        delete statue;
        statue = nullptr;
    }
    if(portal != nullptr && portal->get_helpth() <= 0) {
        cells[portal->get_location().x][portal->get_location().y]->Cell_send_use(false);
        delete portal;
        portal = nullptr;
    }
    
    for(auto& enemy : enemies) {
        if (enemy != nullptr && enemy->get_helpth() <= 0) {
            cells[enemy->get_location().x][enemy->get_location().y]->Cell_send_use(false);
            delete enemy;
            enemy = nullptr;
        }
    }
    enemies.erase(std::remove(enemies.begin(), enemies.end(), nullptr), enemies.end());

    for(auto& ally : allies) {
        if (ally != nullptr && ally->get_helpth() <= 0) {
            cells[ally->get_location().x][ally->get_location().y]->Cell_send_use(false);
            ally = nullptr;
        }
    }
    allies.erase(std::remove(allies.begin(), allies.end(), nullptr), allies.end());
}

void Entity_Manager::removeDeadAllies() {
    for(auto it = allies.begin(); it != allies.end(); ) {
        if (*it != nullptr && (*it)->get_helpth() <= 0) {
            delete *it;
            it = allies.erase(it);
        } else {
            ++it;
        }
    }
}

void Entity_Manager::moveAllEnemies(Field& field, InterfaceGameLogger& logger_consol) {
    int count_enemies_before = int(enemies.size());
    removeDeadEnemies();
    
    for(auto& enemy : enemies) {
        if(enemy) {
            std::random_device rd;
            std::mt19937 gen(rd());
            
            std::uniform_real_distribution<double> dist(0.0, 1.0);
            
            if (dist(gen) < CHANCE_TO_GO_TO_THE_HERO) {
                enemy->move_towards_target(field, logger_consol);
            } else {
                enemy->random_move(field, logger_consol);
            }
        }
    }
    removeDeadEnemies();
    hero->onEnemyKilled(logger_consol, count_enemies_before - int(enemies.size()));
}

void Entity_Manager::moveAllAllies(Field& field, InterfaceGameLogger& logger_consol) {
   int count_before = int(enemies.size());

    removeDeadAllies();

    for (auto& ally : allies) {
        if (ally) {
            ally->move_towards_target(field, logger_consol);
        }
    }

    removeDeadAllies();
    removeDeadEnemies();

    int count_after = int(enemies.size());
    if (count_before != count_after) {
        hero->onEnemyKilled(logger_consol, count_before - count_after);
    }
}

bool Entity_Manager::allEnemiesDead() {
    return enemies.size() == 0 && statue == nullptr && portal == nullptr;
} 

void Entity_Manager::spawn_enemis_by_portal(std::vector<std::vector<Cell*>>& cells, Field& field) {
    if(!isPortalAlive()) return;
    auto* enemy = portal->generate_emply(cells, field);
    if(enemy) {
        enemies.push_back(enemy);
    }
}

void Entity_Manager::destroyPortal() { 
    if(portal) {
        delete portal;
        portal = nullptr;
    }
}




void Entity_Manager::spawnEnemy(const std::string& type, int x, int y, int hp) {
    Enemy* enemy = nullptr;
    if (type == "Slime") enemy = new Slime(x, y, field->get_offset().x, field->get_offset().y);
    else if (type == "Demon") enemy = new Demon(x, y, field->get_offset().x, field->get_offset().y);
    else if (type == "Shadow") enemy = new Shadow(x, y, field->get_offset().x, field->get_offset().y);
    
    if (enemy) {
        enemy->set_helpth(hp);
        enemies.push_back(enemy);
        field->get_cells()[x][y]->Cell_send_use(true, enemy);
    }
}


void Entity_Manager::clearAllEntities(bool flag_delete_hero) {
    auto& cells = field->get_cells();
    
    if (hero && flag_delete_hero) {
        int x = hero->get_location().x;
        int y = hero->get_location().y;
        if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
            cells[x][y]->Cell_send_use(false);
        }
        delete hero;
        hero = nullptr;
    }

    for(auto& enemy : enemies) {
        if(enemy) {
            int x = enemy->get_location().x;
            int y = enemy->get_location().y;
            if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
                cells[x][y]->Cell_send_use(false);
            }
            delete enemy;
            enemy = nullptr;
        }
    }
    enemies.clear();
    
    for(auto& ally : allies) {
        if(ally) {
            int x = ally->get_location().x;
            int y = ally->get_location().y;
            if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
                cells[x][y]->Cell_send_use(false);
            }
            delete ally;
            ally = nullptr;
        }
    }
    allies.clear();
    
    if(portal) {
        int x = portal->get_location().x;
        int y = portal->get_location().y;
        if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
            cells[x][y]->Cell_send_use(false);
        }
        delete portal;
        portal = nullptr;
    }
    
    if(statue) {
        int x = statue->get_location().x;
        int y = statue->get_location().y;
        if (x >= 0 && x < (int)cells.size() && y >= 0 && y < (int)cells[x].size()) {
            cells[x][y]->Cell_send_use(false);
        }
        delete statue;
        statue = nullptr;
    }
}

void Entity_Manager::set_hero(Hero* newHero) {
    if (hero != nullptr) {
        auto& cells = field->get_cells();
        auto loc = hero->get_location();
        cells[loc.x][loc.y]->Cell_send_use(false);
        delete hero;
    }
    hero = newHero;
}




void Entity_Manager::initializeEntities(int level, bool fromSave) {
    auto& cells = field->get_cells();
    sf::Vector2u offsets = field->get_offset();
    
    std::vector<std::pair<int, int>> free_cells;
    for(int x = 0; x < int(cells.size()); x++) {
        for(int y = 0; y < int(cells[x].size()); y++) {
            if(!cells[x][y]->Cell_get_use() && cells[x][y]->get_status_cell() == ordinary) {
                free_cells.push_back({x, y});
            }
        }
    }
    
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(free_cells.begin(), free_cells.end(), g);
    
    if (hero == nullptr) {
        clearAllEntities(true);
    } else {
        clearAllEntities(false);
    }
    
    int index = 0;
    
    if (!fromSave) {
        for(int i = 0; i < NUMBER_OF_ENEMIES && index < int(free_cells.size()); i++) {
            auto [x, y] = free_cells[index++];
            
            switch(i) {
                case 0:
                    enemies.push_back(new Slime(x, y, offsets.x, offsets.y, level));
                    cells[x][y]->Cell_send_use(true, enemies.back());
                    enemies.back()->update_parametrs();
                    break;
                case 1:
                    enemies.push_back(new Demon(x, y, offsets.x, offsets.y, level));
                    cells[x][y]->Cell_send_use(true, enemies.back());
                    enemies.back()->update_parametrs();
                    break;
                case 2:
                    enemies.push_back(new Shadow(x, y, offsets.x, offsets.y, level));
                    cells[x][y]->Cell_send_use(true, enemies.back());
                    enemies.back()->update_parametrs();
                    break;
            }
        }
    }
    
    if (!fromSave && index < int(free_cells.size())) {
        auto [x, y] = free_cells[index++];  
        portal = new Portal(x, y, offsets.x, offsets.y, 5, level);
        portal->update_parametrs();
        cells[x][y]->Cell_send_use(true, portal);
    }
    
    if (!fromSave && index < int(free_cells.size())) {
        auto [x, y] = free_cells[index++];
        if (hero == nullptr) {
            hero = new Hero(x, y, offsets.x, offsets.y, level);
        } else {
            hero->set_location(x, y);
            hero->updateOffsets(offsets.x, offsets.y);
        }
        cells[x][y]->Cell_send_use(true, hero);
    }
    
    if (!fromSave && index < int(free_cells.size())) {
        auto [x, y] = free_cells[index++];
        statue = new MagicStatue(x, y, offsets.x, offsets.y, level);
        statue->update_parametrs();
        cells[x][y]->Cell_send_use(true, statue);
    }
}

void Entity_Manager::updateField(Field* newField, Hero* hero, int level, bool fromSave) {
    this->field = newField;
    this->hero = hero;
    initializeEntities(level, fromSave);
}