#include "GameField.h"

#include "Player.h"
#include "Saloon.h"
#include "Barracks.h"
#include "CowboyTower.h"
#include "Enemy.h"

#include <iostream>

using namespace GameConstants; 

void GameField::clearGrid() {
    if (grid != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
}

void GameField::copyGrid(const GameField& other) {
    width = other.width;
    height = other.height;
    
    grid = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new Cell[width];
        for (int j = 0; j < width; ++j) {
            grid[i][j] = other.grid[i][j];
        }
    }
}

GameField::GameField(int w, int h) : width(w), height(h) {
    if (w < MIN_FIELD_SIZE) width = MIN_FIELD_SIZE;
    if (h < MIN_FIELD_SIZE) height = MIN_FIELD_SIZE;
    if (w > MAX_FIELD_SIZE) width = MAX_FIELD_SIZE;
    if (h > MAX_FIELD_SIZE) height = MAX_FIELD_SIZE;
    
    
    grid = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new Cell[width];
    }
    //initializeField();
    max_cnt_enemy = MAX_CNT_ENEMY;
    max_cnt_pit = MAX_CNT_PIT;
    max_cnt_barracks = MAX_CNT_BARRACKS;
    max_cnt_saloon = MAX_CNT_SALOON;
    max_cnt_cowboy_tower = MAX_CNT_COWBOYTOWER;
    max_cnt_traps = MAX_CNT_TRAPS;

}

GameField::GameField(const GameField& other) : grid(nullptr) {
    width = other.width;
    height = other.height;
    copyGrid(other);

    for (const auto& enemy : other.arr_enemies) 
        arr_enemies.push_back(std::make_unique<Enemy>(*enemy));
    for (const auto& pit : other.arr_pit) 
        arr_pit.push_back(std::make_unique<Pit>(*pit));
    for (const auto& barracks : other.arr_barracks) 
        arr_barracks.push_back(std::make_unique<Barracks>(*barracks));
    for (const auto& saloon : other.arr_saloon) 
        arr_saloon.push_back(std::make_unique<Saloon>(*saloon));
    for (const auto& tower : other.arr_cowboy_towers)
        arr_cowboy_towers.push_back(std::make_unique<CowboyTower>(*tower));
    for (const auto& trap : other.arr_traps)
        arr_traps.push_back(std::make_unique<Trap>(*trap));
    for (const auto& teamate : other.arr_teamates)
        arr_teamates.push_back(std::make_unique<Teamate>(*teamate)); 
}

GameField::GameField(GameField&& other) 
    : width(other.width), height(other.height), grid(other.grid),
      arr_enemies(std::move(other.arr_enemies)),
      arr_pit(std::move(other.arr_pit)),
      arr_barracks(std::move(other.arr_barracks)),
      arr_saloon(std::move(other.arr_saloon)), 
      arr_traps(std::move(other.arr_traps)),
      arr_teamates(std::move(other.arr_teamates))  {
    other.width = 0;
    other.height = 0;
    other.grid = nullptr;
}

GameField& GameField::operator=(const GameField& other) {
    if (this == &other) return *this;
    
    clearGrid();
    
    arr_enemies.clear();
    arr_pit.clear();
    arr_barracks.clear();
    arr_saloon.clear();
    arr_cowboy_towers.clear();
    arr_traps.clear();
    arr_traps.clear();
    arr_teamates.clear();
    
    width = other.width;
    height = other.height;
    copyGrid(other);
    
    for (const auto& enemy : other.arr_enemies) 
        arr_enemies.push_back(std::make_unique<Enemy>(*enemy));
    for (const auto& pit : other.arr_pit) 
        arr_pit.push_back(std::make_unique<Pit>(*pit));
    for (const auto& barracks : other.arr_barracks) 
        arr_barracks.push_back(std::make_unique<Barracks>(*barracks));
    for (const auto& saloon : other.arr_saloon) 
        arr_saloon.push_back(std::make_unique<Saloon>(*saloon));
    for (const auto& tower : other.arr_cowboy_towers)
        arr_cowboy_towers.push_back(std::make_unique<CowboyTower>(*tower));
    for (const auto& trap : other.arr_traps)
        arr_traps.push_back(std::make_unique<Trap>(*trap));
    for (const auto& teamate : other.arr_teamates)
        arr_teamates.push_back(std::make_unique<Teamate>(*teamate)); 

    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this == &other) {
        return *this;
    }
    
    clearGrid();
    
    arr_enemies.clear();
    arr_pit.clear();
    arr_barracks.clear();
    arr_saloon.clear();
    arr_cowboy_towers.clear();
    arr_traps.clear();
    arr_traps.clear();
    arr_teamates.clear();

    width = other.width;
    height = other.height;
    grid = other.grid;
    
    arr_enemies = std::move(other.arr_enemies);
    arr_pit = std::move(other.arr_pit);
    arr_barracks = std::move(other.arr_barracks);
    arr_saloon = std::move(other.arr_saloon);
    arr_cowboy_towers = std::move(other.arr_cowboy_towers);
    arr_traps = std::move(other.arr_traps);
    arr_teamates = std::move(other.arr_teamates);


    other.width = 0;
    other.height = 0;
    other.grid = nullptr;
    
    return *this;
}

GameField::~GameField() {
    clearGrid();
}

void GameField::activateTrap(int x, int y, Character& character, Player& player) {
    for (auto& trap : arr_traps) {
        if (trap->isActive() && trap->getX() == x && trap->getY() == y) {
            trap->activate(character);
            break;
        }
    }
}

Enemy& GameField::findEnemy(int x, int y) {
    for (auto& enemy : arr_enemies) {
        if (enemy->getX() == x && enemy->getY() == y) {
            return *enemy;
        }
    }
    throw std::runtime_error("Враг не найден");
}

Saloon& GameField::findSaloon(int x, int y) {
    for (auto& saloon : arr_saloon) {
        if (saloon->getX() == x && saloon->getY() == y) {
            return *saloon;
        }
    }
    throw std::runtime_error("SALOON не найден");
}

Barracks& GameField::findBarracks(int x, int y) {
    for (auto& barracks : arr_barracks) {
        if (barracks->getX() == x && barracks->getY() == y) {
            return *barracks;
        }
    }
    throw std::runtime_error("BARRACKS не найден");
}

CowboyTower& GameField::findCowboyTower(int x, int y){
    for (auto& tower : arr_cowboy_towers) {
        if (tower->getX() == x && tower->getY() == y) {
            return *tower;
        }
    }
    throw std::runtime_error("COWBOYTOWER не найден");
}

Teamate& GameField::findTeamate(int x, int y){
    for (auto& teamate : arr_teamates) {
        if (teamate->getX() == x && teamate->getY() == y) {
            return *teamate;
        }
    }
    throw std::runtime_error("COWBOYTOWER не найден");
}

void GameField::removeDeadObjects() {
    for (auto it = arr_enemies.begin(); it != arr_enemies.end(); ) {
        if (!(*it)->isAlive()) {
            clearCell((*it)->getX(), (*it)->getY());
            if ((*it)->getInsaloon()) {
                setCellType((*it)->getX(), (*it)->getY(), SALOON);
            }
            it = arr_enemies.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = arr_barracks.begin(); it != arr_barracks.end(); ) {
        if (!(*it)->IsNotDestroyed()) {
            clearCell((*it)->getX(), (*it)->getY());
            it = arr_barracks.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = arr_cowboy_towers.begin(); it != arr_cowboy_towers.end(); ) {
        if (!(*it)->IsNotDestroyed()) {
            clearCell((*it)->getX(), (*it)->getY());
            it = arr_cowboy_towers.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = arr_traps.begin(); it != arr_traps.end(); ) {
        if (!(*it)->isActive()) {
            clearCell((*it)->getX(), (*it)->getY());
            it = arr_traps.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = arr_teamates.begin(); it != arr_teamates.end(); ) {
        if (!(*it)->isAlive()) {
            std::cout << "Союзник " << (*it)->getName() << " погиб в бою!" << std::endl;
            clearCell((*it)->getX(), (*it)->getY());
            it = arr_teamates.erase(it);
        } else {
            ++it;
        }
    }
}

void GameField::initializeField() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = Cell(EMPTY, true);    
        }
    }
}

Cell& GameField::getCell(int x, int y) const{
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    throw std::out_of_range("Координаты выходят за пределы поля");
}


bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::canMoveTo(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].isWalkable();
}

void GameField::setCellType(int x, int y, Type_cell type) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(type);
        if (type == PIT or type == ENEMY or type == PLAYER or type == BARRACK or type == COWBOYTOWER or type == JHONNYGHOST) {
            grid[y][x].setWalkable(false);
        } else {
            grid[y][x].setWalkable(true);
        }
    }
}

void GameField::spawn(int count, Type_cell type) {
    for (int i = 0; i < count; i++) {
        int x, y;
        int attempts = 0;
        
        do {
            x = 1 + std::rand() % (width - 2);
            y = 1 + std::rand() % (height - 2);
            attempts++;
        } while (!canMoveTo(x, y) && attempts < SPAWN_ATTEMPTS_LIMIT);
        
        if (attempts >= SPAWN_ATTEMPTS_LIMIT) continue;
        
        setCellType(x, y, type);
        
        switch(type) {
            case ENEMY: {
                if (arr_enemies.size() < max_cnt_enemy) {
                    std::string name = CowboyNames::names_arr[std::rand() % CowboyNames::NAME_COUNT];
                    arr_enemies.push_back(std::make_unique<Enemy>(name, DEFAULT_ENEMY_HEALTH, DEFAULT_ENEMY_DAMAGE, x, y));
                }
                break;
            }
            case PIT: {
                if (arr_pit.size() < max_cnt_pit) {
                    arr_pit.push_back(std::make_unique<Pit>(x, y));
                }
                break;
            }
            case BARRACK: {
                if (arr_barracks.size() < max_cnt_barracks) {
                    arr_barracks.push_back(std::make_unique<Barracks>(x, y, DEFAULT_COOLDOWN));
                }
                break;
            }
            case SALOON: {
                if (arr_saloon.size() < max_cnt_saloon) {
                    arr_saloon.push_back(std::make_unique<Saloon>(x, y));
                }
                break;
            }
            case COWBOYTOWER: {
                if (arr_cowboy_towers.size() < max_cnt_cowboy_tower) {
                    arr_cowboy_towers.push_back(std::make_unique<CowboyTower>(x, y, TOWER_HEALTH, TOWER_ATTACK_COOLDOWN, TOWER_ATTACK_RANGE));
                }
                break;
            }
        }
    }
}

void GameField::clearCell(int x, int y) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(EMPTY);
        grid[y][x].setWalkable(true);
    }
}

void GameField::processAllTurns(Player& player){

    removeDeadObjects();
    for (auto& teamate : arr_teamates) {
        if (teamate && teamate->isAlive()) {
            teamate->update(*this, player);
        }
    }
    removeDeadObjects();
    for (auto& enemy : arr_enemies) {
        if (enemy && enemy->isAlive()) {
            enemy->move(player.getX(), player.getY(), player, *this);
        }
    }

    for (auto& barracks : arr_barracks) {
        if (barracks) {
           barracks->update(*this);
        }
    }

    for (auto& tower : arr_cowboy_towers) {
        if (tower) {
           tower->update(*this, player);
        }
    }
    removeDeadObjects();

}

int GameField::getWidth() const { return width; }
int GameField::getHeight() const { return height; }
int GameField::getEnemyCount() const {
    return arr_enemies.size();
}
int GameField::getMaxCntEnemy() const { return max_cnt_enemy; }

void GameField::setMaxPitCnt(int newMaxCnt) { max_cnt_pit = newMaxCnt; }