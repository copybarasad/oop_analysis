#include "gamemap.hpp"

GameMap::GameMap(int width, int height) 
    : width(width), 
      height(height) {
    initializeGrid();
}

GameMap::GameMap(std::string name){
    saveFile f(name);
    
    json j = json::parse(f.get());
    loadMap(j);
}

GameMap::GameMap(const GameMap& other) 
    : width(other.width), height(other.height) {
    
    grid.resize(height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i].push_back(other.grid[i][j]);
        }
    }
}

GameMap::GameMap(GameMap&& other) noexcept 
    : width(other.width), height(other.height), 
      grid(std::move(other.grid)) {
    
    other.width = 0;
    other.height = 0;
}

GameMap& GameMap::operator=(const GameMap& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

GameMap& GameMap::operator=(GameMap&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameMap::initializeGrid() {
    grid.resize(height);
    std::srand(std::time(nullptr));
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char icon;
            if (std::rand() % 100 < 10) {
                icon = '#'; 
            } else if (std::rand() % 100 < 5) {
                icon = '~'; 
            }else if(std::rand() % 100 < 2) {
                icon = 'G';
            } else {
                icon = '*';
            }
            grid[i].emplace_back(icon);
        }
    }
}
void GameMap::saveMap(json& j) {
    json jmap = json::array();

    for (int y = 0; y < height; ++y) {
        json row = json::array();
        for (int x = 0; x < width; ++x) {
            char icon = grid[y][x].getCharacter() == nullptr  ? grid[y][x].getType() : '*'; 
            row.push_back(std::string(1, icon));
            // row.push_back(std::string(1, grid[y][x].getType()));
        }
        jmap.push_back(row);
    }

    j["map"] = jmap;
}

void GameMap::loadMap(json& j) {
    json& jmap = j.at("map");
    height = j["game_state"]["height"];
    width = j["game_state"]["width"];
    grid.resize(height);

    for (int y = 0; y < height; ++y) {
        grid[y].resize(width);
        for (int x = 0; x < width; ++x) {
            grid[y][x] = Plot(jmap[y][x].get<std::string>()[0]); 
        }
    }
}


bool GameMap::isValidPosition(int x, int y){
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameMap::isPositionEmpty(int x, int y){
    return isValidPosition(x, y) && grid[y][x].isEmpty();
}

bool GameMap::isPositionPassable(int x, int y) {
    return isValidPosition(x, y) && !grid[y][x].isNotPassable();
}

Plot& GameMap::getPlot(int x, int y) {
    return grid[y][x];
}


void GameMap::placeCharacter(Character* character, int x, int y) {
    if (character == nullptr) {
        return;
    }
    if (isValidPosition(x, y)) {
        Plot& tile = grid[y][x]; 
        if(tile.isTrap()){
            std::cout << character->getIcon() << ": Stepped into a trap! " << trapDamage << " damage received!\n";
            character->takeDamage(trapDamage);
        }
        tile.placeObj(character);
    }
}

void GameMap::removeCharacter(int x, int y) {
    if (isValidPosition(x, y)) {
        grid[y][x].removeObj();
    }
}

Character* GameMap::getCharacterAt(int x, int y){
    if (isValidPosition(x, y)) {
        return grid[y][x].getCharacter();
    }
    return nullptr;
}

void GameMap::display() const {
    std::cout << "\n";
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << grid[i][j].getType() << " ";
        }
        std::cout << "\n";
    }
}

void GameMap::setTrap(int x, int y){
    Plot& plot = getPlot(x, y);
    if(plot.isEmpty() && isValidPosition(x, y) && isPositionPassable(x, y)) plot.setTrap();
    else std::cout << "The place is not empty!\n";
}

void GameMap::findSpawnPositionNearEntity(int X, int Y, int& emptyX, int& emptyY) {
    int directions[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},          {0, 1},
        {1, -1},  {1, 0}, {1, 1}
    };
    
    for (int i = 0; i < 8; ++i) {
        int newX = X + directions[i][0];
        int newY = Y + directions[i][1];
        
        if (isPositionEmpty(newX, newY)) {
            emptyX = newX;
            emptyY = newY;
            return;
        }
    }
    
    emptyX = -1;
    emptyY = -1;
}