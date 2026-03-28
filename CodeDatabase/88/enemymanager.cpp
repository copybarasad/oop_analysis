#include "enemymanager.hpp"
#include <ctime>
#include <iostream>

EnemyManager::EnemyManager(GameMap& map, std::vector<std::unique_ptr<Character>>& characters) 
    : gameMap(map), allCharacters(characters), magichand(map) {
    std::srand(std::time(nullptr));
}

void EnemyManager::LoadEnemies(json& j){
    for(auto& c: j){
        char icon = c["icon"].get<char>();
        if(c["health"] <= 0) continue;
        if(icon == 'T' || icon == 'M'){
            auto tower = addTower(c["x"], c["y"], c["tot_cooldown"], icon);
            tower->setHealth(c["health"]);
            if(c["burning"]) tower->setAblaze();
            tower->setBurnCooldown(c["burn_cooldown"]);
            MageTower* m = dynamic_cast<MageTower*>(tower);
            if(m){
                m->setAttackRadius(c["radius"]);
            }
        } else if(icon == 'E'){
            auto enemy = std::make_unique<Enemy>(c["health"], c["damage"], c["x"], c["y"]);
            enemy->setBurnCooldown(c["burn_cooldown"]);
            if(c["burning"]) enemy->setAblaze();
            gameMap.placeCharacter(enemy.get(), c["x"], c["y"]);
            allCharacters.push_back(std::move(enemy));
        }
    }
}

Tower* EnemyManager::addTower(int x, int y, int cooldown, char type) {
    std::unique_ptr<Tower> tower;
    if (type == 'T') {
        tower = std::make_unique<Tower>(20, 0, x, y, cooldown);
    } else if (type == 'M') {
        tower = std::make_unique<MageTower>(20, 20, x, y, cooldown, 4);
    } 
    Tower* ptr = tower.get();
    gameMap.placeCharacter(ptr, x, y);
    allCharacters.push_back(std::move(tower));
    return ptr;
}


void EnemyManager::spawnInitialEnemies(int count) {
    for (int i = 0; i < count; i++) {
        int x, y;
        if (findEmptyPosition(x, y)) {
            auto enemy = std::make_unique<Enemy>(40, 10, x, y);
            gameMap.placeCharacter(enemy.get(), x, y);
            allCharacters.push_back(std::move(enemy));
        }
    }

}

void EnemyManager::spawnTowers(int mageCount, int spawnerCount){
    int towerX, towerY;
    for(int i = 0; i < mageCount + spawnerCount; i++){
        do {
            towerX = std::rand() % gameMap.getWidth();
            towerY = std::rand() % gameMap.getHeight();
        } while (!gameMap.isPositionEmpty(towerX, towerY) || (towerX == 0 && towerY == 0));
        
        addTower(towerX, towerY, 20, i < mageCount ? 'M' : 'T');
    }
}
void EnemyManager::updateMageTowers(Player* player){
    for (auto& character : allCharacters) {
        bool inRange = false;
        MageTower* m = dynamic_cast<MageTower*>(character.get());
        if (m && m->isAlive()) {
            int dx = player->getX() - m->getX();
            int dy = player->getY() - m->getY();
            int dist = std::abs(dx + dy);
            if(dx == 0 || dy == 0) inRange = dist <= m->getAttackRadius();
            if (inRange && m->isReady()) {
                magichand.thunderbolt(dx, dy, dist, m, player);
                m->resetCooldown();
            } else {
                m->tickCooldown();
            }
        }
    }
}


void EnemyManager::spawnEnemyFromTower(Tower* tower) {
    if (tower == nullptr) {
        return;
    }
    int spawnX, spawnY;
    gameMap.findSpawnPositionNearEntity(tower->getX(), tower->getY(), spawnX, spawnY);
    
    if (spawnX != -1 && spawnY != -1) {
        auto enemy = std::make_unique<Enemy>(50, 10, spawnX, spawnY);
        gameMap.placeCharacter(enemy.get(), spawnX, spawnY);
        allCharacters.push_back(std::move(enemy));
        std::cout << "Tower spawned new enemy at (" << spawnX << ", " << spawnY << ")\n";
    } else {
        std::cout << "Tower could not find spawn position\n";
    }
}

void EnemyManager::updateTowers() {
    std::vector<Tower*> towers;
    for (auto& character : allCharacters) {
        Tower* tower = dynamic_cast<Tower*>(character.get());
        if (tower && tower->isAlive()) {
            towers.push_back(tower);
        }
    }
    
    for (Tower* tower : towers) {
        if (tower->isTimerUp()) {
            if(tower->getIcon() == 'T') spawnEnemyFromTower(tower);
            else std::cout << tower->getIcon() << "\n";
        }
    }
}

bool EnemyManager::findEmptyPosition(int& x, int& y) const {
    for (int attempt = 0; attempt < 100; attempt++) {
        x = std::rand() % gameMap.getWidth();
        y = std::rand() % gameMap.getHeight();
        
        if (gameMap.isPositionEmpty(x, y) && (x != 0 || y != 0)) {
            return true;
        }
    }
    return false;
}