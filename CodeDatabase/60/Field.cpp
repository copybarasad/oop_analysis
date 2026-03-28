#include <Field.hpp>
#include <PlayerTrap.hpp> 
#include <ncurses.h>
#include <Constants.hpp>
#include <Wall.hpp>
#include <Floor.hpp>
#include <Player.hpp>
#include <Enemy.hpp>
#include <EnemyBase.hpp>
#include <SpikedTrap.hpp>
#include <ArcherTower.hpp>
#include <DecoyAlly.hpp>
#include <Utils.hpp>
#include <algorithm>
#include <random>

Field::Field() : screen(nullptr), width(FIELD_WIDTH), height(FIELD_HEIGHT), player_ptr(nullptr), turnCounter(0), currentLevel(1) {
    ceils.resize(height);
    for (int i = 0; i < height; ++i) {
        ceils[i].resize(width);
    }
}

Field::Field(const Field& other) : 
    ceils(other.ceils), 
    entities(other.entities), 
    player_ptr(other.player_ptr), 
    screen(other.screen), 
    height(other.height), 
    width(other.width), 
    turnCounter(other.turnCounter),
    currentLevel(other.currentLevel) 
{}

Field::Field(Field&& other) : 
    ceils(std::move(other.ceils)), 
    entities(std::move(other.entities)), 
    player_ptr(std::move(other.player_ptr)), 
    screen(other.screen), 
    height(other.height), 
    width(other.width), 
    turnCounter(other.turnCounter),
    currentLevel(other.currentLevel) 
{
    other.height = 0;
    other.width = 0;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        ceils = other.ceils;
        entities = other.entities;
        player_ptr = other.player_ptr;
        screen = other.screen;
        height = other.height;
        width = other.width;
        turnCounter = other.turnCounter;
        currentLevel = other.currentLevel; 
    }
    return *this;
}

Field& Field::operator=(Field&& other) {
    if (this != &other) {
        ceils = std::move(other.ceils);
        entities = std::move(other.entities);
        player_ptr = std::move(other.player_ptr);
        screen = other.screen;
        height = other.height;
        width = other.width;
        turnCounter = other.turnCounter;
        currentLevel = other.currentLevel; 
        other.height = 0;
        other.width = 0;
    }
    return *this;
}

void Field::draw() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ceils[i][j].draw();
        }
    }

    Utils::printMultiline(screen->yMax - 5, 0, Utils::readFileToString(TURN_BAR));
    Utils::printNumbers(screen->yMax - 5, 27, turnCounter);
    Utils::printNumbers(screen->yMax - 5, screen->xMax/2, currentLevel);

    if (player_ptr) {
        player_ptr->drawUI(); 
        if (player_ptr->getInTrap()) {
            mvprintw(screen->yMax - 10, screen->xMax / 2 - 19, "You are trapped. Press Space to escape");
            refresh();
        }
    }
} 

void Field::setScreen(const ScreenSize* screen) {
    this->screen = screen;
}

std::shared_ptr<Player> Field::getPlayer() const {
    return player_ptr;
}

bool Field::updatePlayer(int ch) {
    if (!player_ptr || player_ptr->getHealth() <= 0) {
        return false;
    }

    player_ptr->resetAttackFlag();
    bool playerMadeAction = false;
    if (ch != ERR && !player_ptr->getInTrap()) {
        playerMadeAction = player_ptr->handleInput(ceils, entities, ch, screen, height, width);
    } else if (ch == ' ') {
        playerMadeAction = true;
        player_ptr->unSetInTrap();
    }

    if (playerMadeAction) {
        player_ptr->giveMp(1);
        turnCounter++;
    }

    return playerMadeAction;
}

void Field::updateAllies() {
    for (auto& entity : entities) {
        if (entity->getType() == Entity::Type::DECOY_ALLY && entity->getHealth() > 0) {
            std::static_pointer_cast<DecoyAlly>(entity)->update(ceils, screen, height, width);
        }
    }
}

void Field::updateEnemies() {
    if (!player_ptr || player_ptr->getHealth() <= 0) return;

    for (auto& entity : entities) {
        if (entity->getType() == Entity::Type::ENEMY && entity->getHealth() > 0) {
            std::static_pointer_cast<Enemy>(entity)->update(ceils, screen, entities, height, width);
            if (player_ptr->getHealth() <= 0) break;
        }
    }
}

void Field::updateStructures() {
    if (!player_ptr || player_ptr->getHealth() <= 0) return;

    bool baseAlreadySpawned = false;

    for (size_t i = 0; i < entities.size(); ++i) {
        auto& entity = entities[i];

        if (entity->getHealth() <= 0) continue;

        if (entity->getType() == Entity::Type::ENEMY_BASE) {
            if (!baseAlreadySpawned && (turnCounter % SPAWN_COOLDOWN == 0)) {
                auto base = std::static_pointer_cast<EnemyBase>(entity);
                base->spawnEnemy(ceils, entities, screen, height, width);
                baseAlreadySpawned = true;
            }
        } else if (entity->getType() == Entity::Type::ARCHER_TOWER) {
            auto tower = std::static_pointer_cast<ArcherTower>(entity);
            tower->update(player_ptr, ceils, screen, height, width);
            if (player_ptr->getHealth() <= 0) break; 
        }
    }
}

void Field::cleanupDeadEntities() {
    if (!player_ptr) return;

    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [&](const std::shared_ptr<LivingEntity>& entity) {
            if (entity->getHealth() <= 0 && entity->getType() != Entity::Type::PLAYER) {
                player_ptr->giveExp(entity->getExp());

                int worldY = entity->getY() - screen->yMax / 2 + height / 2;
                int worldX = entity->getX() - screen->xMax / 2 + width / 2;

                if (worldY >= 0 && worldY < height && worldX >= 0 && worldX < width) {

                    if (ceils[worldY][worldX].getEntity() == entity) {
                        ceils[worldY][worldX].setEntity(std::make_shared<Floor>(entity->getX(), entity->getY()));
                    }
                }

                return true; 
            }
            return false;
        }), entities.end());
}

bool Field::screenToWorld(int screenX, int screenY, int& worldX, int& worldY) const {
    worldY = screenY - screen->yMax / 2 + height / 2;
    worldX = screenX - screen->xMax / 2 + width / 2;
    return (worldY >= 0 && worldY < height && worldX >= 0 && worldX < width);
}

size_t Field::getTurnCounter() const {
    return turnCounter;
}

const std::vector<std::shared_ptr<LivingEntity>>& Field::getEntities() const {
    return entities;
}

void Field::setTurnCounter(size_t count) {
    this->turnCounter = count;
}

void Field::clearAndCreateBaseLayout() {

    entities.clear();
    player_ptr = nullptr;
    turnCounter = 0;
}

void Field::addPlayer(std::shared_ptr<Player> player) {
    if (!player) return;
    this->player_ptr = player;
    addEntity(player);
}

void Field::addEntity(std::shared_ptr<LivingEntity> entity) {
    if (!entity) return;
    this->entities.push_back(entity);

    int worldY, worldX;
    if (this->screenToWorld(entity->getX(), entity->getY(), worldX, worldY)) {
        ceils[worldY][worldX].setEntity(entity);
    }
}

void Field::createPlayerTrapAt(int x, int y, int damage) {
    auto trap = std::make_shared<PlayerTrap>(x, y);
    trap->setDamage(damage);

    int worldY, worldX;
    if (this->screenToWorld(x, y, worldX, worldY)) {
        ceils[worldY][worldX].setEntity(trap);
    }
}

void Field::createSpikedTrapAt(int x, int y) {
    auto trap = std::make_shared<SpikedTrap>(x, y);
    int worldY, worldX;
    if (this->screenToWorld(x, y, worldX, worldY)) {
        ceils[worldY][worldX].setEntity(trap);
    }
}

std::shared_ptr<Entity> Field::getEntityAt(int worldY, int worldX) const {
    if (worldY >= 0 && worldY < height && worldX >= 0 && worldX < width) {
        return ceils[worldY][worldX].getEntity();
    }
    return nullptr; 
}

int Field::getH() const {
    return height;
}

int Field::getW() const {
    return width;
}

int Field::getCurrentLevel() const {
    return currentLevel;
}

void Field::init() {
    initLevel();
}

void Field::initLevel(bool spawnNewEntities) {

    width = FIELD_WIDTH + (currentLevel - 1) * 4;  
    height = FIELD_HEIGHT + (currentLevel - 1) * 2; 

    ceils.assign(height, std::vector<Ceil>(width));

    int xStart = screen->xMax / 2 - width / 2;
    int yStart = screen->yMax / 2 - height / 2;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                ceils[i][j].setEntity(std::make_shared<Wall>(xStart + j, yStart + i));
            } else {
                ceils[i][j].setEntity(std::make_shared<Floor>(xStart + j, yStart + i));
            }
        }
    }

    if (spawnNewEntities) {

        auto preserved_player = player_ptr; 
        entities.clear(); 
        player_ptr = preserved_player; 

        std::vector<std::pair<int, int>> possiblePositions;
        for (int i = 1; i < height - 1; ++i) {
            for (int j = 1; j < width - 1; ++j) {
                possiblePositions.push_back({i, j});
            }
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(possiblePositions.begin(), possiblePositions.end(), gen);

        std::pair<int, int> pos = possiblePositions.back();
        possiblePositions.pop_back();

        if (player_ptr) { 

            player_ptr->setX(xStart + pos.second);
            player_ptr->setY(yStart + pos.first);
            addEntity(player_ptr); 
        } else { 

            this->player_ptr = std::make_shared<Player>(xStart + pos.second, yStart + pos.first);
            addEntity(player_ptr);
        }

        if (!possiblePositions.empty()) {
            std::pair<int, int> pos = possiblePositions.back();
            possiblePositions.pop_back();
            auto enemy_base_ptr = std::make_shared<EnemyBase>(xStart + pos.second, yStart + pos.first, currentLevel); 
            addEntity(enemy_base_ptr);
        }

        if (!possiblePositions.empty()) {
            std::pair<int, int> pos = possiblePositions.back();
            possiblePositions.pop_back();
            auto tower = std::make_shared<ArcherTower>(xStart + pos.second, yStart + pos.first); 
            addEntity(tower);
        }

        const int numberOfTraps = 8 + (currentLevel - 1) * 2; 
        for (int k = 0; k < numberOfTraps; ++k) {
            if (possiblePositions.empty()) break;
            std::pair<int, int> pos = possiblePositions.back();
            possiblePositions.pop_back();
            createSpikedTrapAt(xStart + pos.second, yStart + pos.first);
        }
    }
}

void Field::advanceToNextLevel() { 
    if (currentLevel < MAX_LEVEL) {
        currentLevel++;
        entities.clear(); 
        initLevel(); 
    }
}

bool Field::isLevelClear() const { 
    for (const auto& entity : entities) {
        if (entity->getType() == Entity::Type::ENEMY || entity->getType() == Entity::Type::ENEMY_BASE) {
            return false; 
        }
    }
    return true;
}

void Field::setCurrentLevel(int level) {
    this->currentLevel = level;
}

const ScreenSize* Field::getScreen() const {
    return screen;
}