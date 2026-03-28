#include "general_manager.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"

GeneralManager::GeneralManager(int fieldWidth, int fieldHeight, int enemyCount, int towerCount, PlayerManager* updatePlayerMan) {
    field = new Field(fieldWidth, fieldHeight);
    playerMan = new PlayerManager();
    towerMan = new TowerManager();
    enemyMan = new EnemyManager({});
    for (int i = 0; i < towerCount; i++) {
        Tower* tower = new Tower(60 + towerCount*2,1 + towerCount / 2, getRandomEmptyPosition(),5 + towerCount);
        towerMan->addTower(tower);
    }
    if (!updatePlayerMan){
        Player* player = new Player();
        auto playerPos = getRandomEmptyPosition();
        playerMan->setPlayer(player, playerPos.first, playerPos.second);
    }else{
        playerMan = updatePlayerMan;
    }
    
    for (int i = 0; i < enemyCount; i++) {
        spawnEnemyRandom(30 + enemyCount, 5 + enemyCount);   
    }
}

GeneralManager::GeneralManager() {
    field = new Field(10, 10);
    Player* player = new Player(1, 1);
    playerMan = new PlayerManager(player, {0,0});
    towerMan = new TowerManager();
    enemyMan = new EnemyManager({});
}

PlayerManager& GeneralManager::getPlayerManager() { return *playerMan; }
EnemyManager& GeneralManager::getEnemyManager() { return *enemyMan; }
Field& GeneralManager::getField() { return *field; }
TowerManager& GeneralManager::getTowerManager() { return *towerMan;}

void GeneralManager::playerTurn(Direction direction){
    if (!playerMan || !enemyMan || !field) return;

    auto curPos = playerMan->getPlayerPosition();
    auto newPos = getNear(curPos, direction);
    auto rangePos = getNear(newPos, direction);

    if (!field->canMoveTo(newPos.first, newPos.second)) return;

    bool hasEnemyAtPos = enemyMan->hasEnemyAt(newPos.first, newPos.second);
    bool hasEnemyAtRangePos = enemyMan->hasEnemyAt(rangePos.first, rangePos.second);

    if (!playerMan->getPlayerFightType() && 
        field->canMoveTo(rangePos.first, rangePos.second) &&
        hasEnemyAtRangePos) {
        
        damageEnemyAt(rangePos.first, rangePos.second, playerMan->getPlayerDamage());
        return;
    }

    if (hasEnemyAtPos) {
        damageEnemyAt(newPos.first, newPos.second, playerMan->getPlayerDamage());
        return;
    }

    if (towerMan->hasTowerAt(newPos.first, newPos.second)) {
    towerMan->damageTowerAt(newPos.first, newPos.second, playerMan->getPlayerDamage());
    return;
    }

    playerMan->movePlayer(newPos.first, newPos.second);
}

void GeneralManager::setField(Field* newField) {
    delete field;
    field = newField;
}

void GeneralManager::enemiesTurn() {
    if (!enemyMan || !playerMan || !field) return;

    towerMan->updateTowersCooldown();
    checkTowerAttacks();

    auto playerPos = playerMan->getPlayerPosition();
    auto enemyPositions = enemyMan->getAliveEnemyPositions();
    
    for (const auto& pos : enemyPositions) {
        int fromX = pos.first;
        int fromY = pos.second;
        if (!enemyMan->hasEnemyAt(fromX, fromY) || !enemyMan->getEnemyAt(fromX, fromY)->isAlive()) {
            continue;
        }

        std::pair<int, int> to;
        if (rand() % 100 < 40){
            to = moveEnemyRandom(fromX, fromY);
        } else {
            to = moveEnemyTowardsPlayer(fromX, fromY, playerPos);
        }
        int toX = to.first;
        int toY = to.second;

        if (!field->canMoveTo(toX, toY) || towerMan->hasTowerAt(toX, toY)) {
            continue;
        }

        if (playerMan->hasPlayerAt(toX, toY)) {
            playerMan->takePlayerDamage(enemyMan->getEnemyDamageAt(fromX, fromY));
        } else if (!enemyMan->hasEnemyAt(toX, toY)) {
            enemyMan->moveEnemy(fromX, fromY, toX, toY);
            const Cell* cell = field->getCell(toX, toY);
            if (cell->getType() == CellType::TRAP) {
                damageEnemyAt(toX, toY, cell->getTrapDamage());
                field->setTrapCell(toX, toY, 0, CellType::EMPTY);
            }
        }
    }
}

std::pair<int, int> GeneralManager::moveEnemyTowardsPlayer(int fromX, int fromY, std::pair<int, int> playerPos) {
    int toX = fromX, toY = fromY;
    
    int dx = playerPos.first - fromX;
    int dy = playerPos.second - fromY;
    
    if (abs(dx) > abs(dy)) {
        if (dx > 0) toX++;
        else if (dx < 0) toX--;
    } else {
        if (dy > 0) toY++;
        else if (dy < 0) toY--;
    }
    
    std::pair<int, int> to;
    to.first = toX;
    to.second = toY;
    return to;
}

void GeneralManager::addTower(Tower* tower) {
    towerMan->addTower(tower);
}

void GeneralManager::checkTowerAttacks() {
    std::pair<int, int> playerPos = playerMan->getPlayerPosition();
    
    const auto& allTowers = towerMan->getAllTowers();
    
    for (const auto& [pos, tower] : allTowers) {
        if (tower && tower->isAlive() && tower->canAttack() && 
            tower->isInRange(playerPos) && playerMan->getPlayer()->isAlive()) {
            
            playerMan->takePlayerDamage(tower->getDamage());
            tower->setCooldown();
        }
    }
}

std::pair<int, int> GeneralManager::moveEnemyRandom(int fromX, int fromY) {
    int toX = fromX, toY = fromY;
    
    switch(rand() % 4) {
        case 0: toY--; break;
        case 1: toY++; break;
        case 2: toX--; break; 
        case 3: toX++; break; 
    }
    
    std::pair<int, int> to;
    to.first = toX;
    to.second = toY;
    return to;
}

std::pair<int,int> GeneralManager::getNear(std::pair<int, int> curPos, Direction dir){
    std::pair<int, int> newPos = curPos;
    switch (dir) {
        case Direction::UP: newPos.second--; break;
        case Direction::DOWN: newPos.second++; break;
        case Direction::LEFT: newPos.first--; break;
        case Direction::RIGHT: newPos.first++; break;
    }
    return newPos;
}

void GeneralManager::spawnPlayer(int x, int y) {
    if (!field || !playerMan) return;
    if (!field->isValidPosition(x, y) || !field->canMoveTo(x, y)) {
        throw std::invalid_argument("Invalid player spawn position");
    }
    
    playerMan->movePlayer(x, y);
}

void GeneralManager::spawnEnemyRandom(int health, int damage) {
    auto pos = getRandomEmptyPosition();
    int x = pos.first;
    int y = pos.second;
    if (!field || !enemyMan) return;
    
    if (!field->isValidPosition(x, y) ||
        !field->canMoveTo(x, y) || enemyMan->hasEnemyAt(x, y)){
            throw std::invalid_argument("Incorrect spawn position");
        }
    
    Enemy* enemy = new Enemy(health, damage);
    enemyMan->addEnemy(enemy, x, y);
}

void GeneralManager::switchPlayerFightType() {
    if (playerMan) {
        playerMan->switchPlayerFightType();
    }
}

std::pair<int, int> GeneralManager::getRandomEmptyPosition() const {
    if (!field) return {0, 0};

    std::vector<std::pair<int, int>> emptyPositions;
    
    for (int y = 0; y < field->getHeight(); ++y) {
        for (int x = 0; x < field->getWidth(); ++x) {
            if (isFreePosition(x, y)) {
                emptyPositions.push_back({x, y});
            }
        }
    }
    
    if (emptyPositions.empty()) {
        throw std::invalid_argument("No empty cell found");
    }
    
    return emptyPositions[rand() % emptyPositions.size()];
}

bool GeneralManager::isFreePosition(int x, int y) const{
    return field->canMoveTo(x,y) && !enemyMan->hasEnemyAt(x,y) && !playerMan->hasPlayerAt(x,y) && !towerMan->hasTowerAt(x,y);
}

void GeneralManager::damageEnemyAt(int x, int y, int dmg) {
    Enemy* e = enemyMan->getEnemyAt(x, y);
    if (e && e->isAlive()) {
        enemyMan->attackEnemyAt(x,y,dmg);
        playerMan->awardPoints(10); 
        if (!e->isAlive()) {
            enemyMan->removeEnemyAt(x, y);
            playerMan->awardPoints(100); 
        }
    }
}

void GeneralManager::castSpell(int index, std::pair<int, int> target) {
    ISpell* spell = playerMan->getSpellHand()->getSpell(index);
    auto playerPos = playerMan->getPlayerPosition();
    int dx = abs(playerPos.first - target.first);
    int dy = abs(playerPos.second - target.second);

    int radius = spell->getRadius();

    if (!spell) return;
    if (dx > radius || dy > radius) {
        std::cout << "Target is out of range. The spell will not be used\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        return;
    }
    if (playerMan->getPlayerPoints() >= spell->getCost()) {
        spell->cast(*this, target);
        playerMan->awardPoints(-(spell->getCost()));
        playerMan->getSpellHand()->removeSpell(index);
    }
}

const std::vector<ISpell*>& GeneralManager::getSpells() const {
    return playerMan->getSpellHand()->getSpells();
}