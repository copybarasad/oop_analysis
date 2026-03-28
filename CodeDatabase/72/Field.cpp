//
// Created by bob on 10/5/25.
//

#include "Field.h"

#include <iostream>
#include <stdexcept>
#include <vector>

#define ERROR_INVALID_SIZE "Invalid size filed."
#define DEFAULT_STEPS 0
#define PERCENT_OBSTACLE 0.04
#define PERCENT_SLOW 0.02
#define DEFAULT_POINTS 5
#define DEFAULT_SIZE 15

Field::Field(const int &height, const int &width, std::shared_ptr<Player> player, std::list<EnemyBuild> enemy_builds, std::list<EnemyTower> enemy_towers)
    : height(height), width(width), player(player), steps(DEFAULT_STEPS), enemy_builds(enemy_builds),
    towers(enemy_towers){

    if (height < MIN_SIZE || height > MAX_SIZE)
        this->height = DEFAULT_SIZE;

    if (width < MIN_SIZE || width > MAX_SIZE)
        this->width = DEFAULT_SIZE;


    map = new Cell*[height];
    for (int i = 0; i < height; i++) {
        map[i] = new Cell[width];
    }

    generateObjects(PERCENT_OBSTACLE, PERCENT_SLOW);
    generatePlayerPosition();
    generatePositionEnemiesBuild();

}

Field::Field() {
    height = 10;
    width = 10;
}

Field::Field(const int &level, std::shared_ptr<Player> player) {

    this->width = MIN_SIZE + (level - 1) * 2;
    this->height = MIN_SIZE + (level - 1) * 2;

    if (height > MAX_SIZE)
        this->height = MAX_SIZE;

    if (width > MAX_SIZE)
        this->width = MAX_SIZE;

    map = new Cell*[height];
    for (int i = 0; i < height; i++) {
        map[i] = new Cell[width];
    }

    int enemyBuildCount = static_cast<int>(level * 1.4f);
    int enemyTowerCount = static_cast<int>(level * 1.3f);

    for (int i = 0; i < enemyTowerCount; i++) {
        EnemyTower tower(Position{0, 0}, level * 100, level * 15, static_cast<int>(level * 0.2 + 5) , level);
        towers.push_back(tower);
    }

    for (int i = 0; i < enemyBuildCount; i++) {
        EnemyBuild build(100 + level * 10,   20 + level * 2, 200 - level * 5);
        enemy_builds.push_back(build);
    }

    this->player = player;

    generateObjects(PERCENT_OBSTACLE, PERCENT_SLOW);
    generatePlayerPosition();
    generatePositionEnemiesBuild();
}


Field::Field(const Field& other)
    : height(other.height),
      width(other.width),
      player(new Player(*other.player)),
      steps(other.steps),
      enemies(other.enemies),
      enemy_builds(other.enemy_builds),
      traps(other.traps),
      towers(other.towers)
{
    map = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new Cell[width];
        for (int j = 0; j < width; ++j) {
            map[i][j] = other.map[i][j];
        }
    }
}


Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;

    for (int i = 0; i < height; ++i) delete[] map[i];
    delete[] map;

    height = other.height;
    width = other.width;
    steps = other.steps;
    enemies = other.enemies;
    enemy_builds = other.enemy_builds;
    traps = other.traps;
    towers = other.towers;

    player = std::shared_ptr<Player>(new Player(*other.player));

    map = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        map[i] = new Cell[width];
        for (int j = 0; j < width; ++j) {
            map[i][j] = other.map[i][j];
        }
    }

    return *this;
}

Field::Field(Field&& other)
    : height(other.height),
      width(other.width),
      map(other.map),
      player(other.player),
      enemies(std::move(other.enemies)),
      enemy_builds(std::move(other.enemy_builds)),
      traps(std::move(other.traps)),
      towers(std::move(other.towers)),
      steps(other.steps)
{
    other.map = nullptr;
    other.player = nullptr;
}

Field& Field::operator=(Field&& other) {
    if (this == &other) return *this;

    for (int i = 0; i < height; ++i) delete[] map[i];
    delete[] map;

    height = other.height;
    width = other.width;
    map = other.map;
    player = other.player;
    enemies = std::move(other.enemies);
    enemy_builds = std::move(other.enemy_builds);
    traps = std::move(other.traps);
    towers = std::move(other.towers);
    steps = other.steps;

    other.map = nullptr;
    other.player = nullptr;

    return *this;
}


Field::~Field() {
    if (map != nullptr) {
        for (int i = 0; i < height; ++i) delete[] map[i];
        delete[] map;
    }

    enemies.clear();
    enemy_builds.clear();
    traps.clear();
    towers.clear();
}


bool Field::isFree(const Position &position) const { return this->map[position.getY()][position.getX()].getState() == State::Free; }

bool Field::isSlow(const Position &position) const { return this->map[position.getY()][position.getX()].getState() == State::Slow; }

bool Field::inField(const Position &position) const {
    return position.getX() >= 0 && position.getY() >= 0 &&
           position.getX() < width && position.getY() < height;
}

bool Field::isObstacle(const Position &position) const {
    return map[position.getY()][position.getX()].getState() == State::Obstacle;
}

bool Field::isEnemy(const Position &position, const bool & isTeammate) {
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (position == it->getPosition()) {
            if (!isTeammate) {
                it->applyDamage(player->getDamage());
                if (!it->isAlive()) {
                    enemies.erase(it);
                    player->addPoints(DEFAULT_POINTS);
                }
            }
            return true;
        }
    }
    return false;
}

bool Field::isPlayer(const Position &position, const int &damage) {
    if (position == player->getPosition()) {
        player->applyDamage(damage);
        return true;
    }
    return false;
}

bool Field::isEnemyBuild(const Position & position, const bool &isTeammate) {
    for (auto it = enemy_builds.begin(); it != enemy_builds.end(); ++it) {
        if (position == it->getPosition()) {
            if (!isTeammate) {
                it->applyDamage(player->getDamage());
                if (!it->isAlive()) {
                    enemy_builds.erase(it);
                }
            }
            return true;
        }
    }
    return false;
}

void Field::generateObjects(const double & obstacleChance, const double & slowChance) const {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double percent = (double)rand() / RAND_MAX;
            if (percent < obstacleChance) {
                map[y][x].setState(State::Obstacle);
            } else if (percent < obstacleChance + slowChance) {
                map[y][x].setState(State::Slow);
            } else {
                map[y][x].setState(State::Free);
            }
        }
    }
}

void Field::generatePlayerPosition() const {
    Position position;
    do {
        position = {rand() % width, rand() % height};
    } while (isObstacle(position));
    player->setPosition(position);
}

void Field::generatePositionEnemiesBuild() {
    for (auto &enemy_build : enemy_builds) {
        Position pos;
        bool placed = false;


        for (int attempt = 0; attempt < 100 && !placed; ++attempt) {
            pos = {rand() % width, rand() % height};
            if (map[pos.getY()][pos.getX()].getState() == State::Free &&
                pos != player->getPosition()) {
                enemy_build.setPosition(pos);
                placed = true;
            }
        }

        if (!placed)
            exit(0);
    }
}

void Field::generateEnemyTower() {
    Position towerPos;
    bool placed = false;

    for (int attempt = 0; attempt < 100 && !placed; ++attempt) {
        towerPos = {rand() % width, rand() % height};
        if (map[towerPos.getY()][towerPos.getX()].getState() == State::Free &&
            towerPos != player->getPosition()) {
            EnemyTower enemy_tower(towerPos);
            towers.push_back(enemy_tower);
            placed = true;
            }
    }

    if (!placed)
        exit(0);
}

Position Field::generatePositionEnemy(const Position &position){
    std::vector<Position> freePositions;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;
            Position pos = position + Position{dx, dy};
            if (inField(pos) && isFree(pos)) {
                freePositions.push_back(pos);
            }
        }
    }

    return freePositions[rand() % freePositions.size()];
}

void Field::checkTrap(const Position &position,std::list<Enemy>::iterator & enemy) {

    for (auto it = traps.begin(); it != traps.end(); ++it) {
        if (it->getPosition() == position && it->isActive()) {
            enemy->applyDamage(it->getDamage());
            if (!enemy->isAlive()) {
                enemy = enemies.erase(enemy);
            }
            it->deactivate();
        }
        if (!it->isActive()) {
            traps.erase(it);
            return;
        }
    }
}

bool Field::isTower(const Position &position, bool isTeammate) {
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if (it->getPosition() == position) {
            if (!isTeammate) {
                it->applyDamage(player->getDamage());
                if (!it->isAlive()) {
                    towers.erase(it);
                }
            }
            return true;
        }
    }

    return false;
}

void Field::addEnemy(const Enemy &enemy) { this->enemies.push_back(enemy); }

int Field::getWidth() const { return this->width; }

int Field::getHeight() const { return this->height; }

std::shared_ptr<Player> Field::getPlayer() const { return this->player; }

std::list<Enemy> & Field::getEnemiesMutable() { return this->enemies; }

const std::list<Enemy> & Field::getEnemies() const {return this->enemies; }

const std::list<EnemyBuild> &Field::getEnemiesBuild() const { return this->enemy_builds; }

std::list<EnemyBuild> & Field::getEnemiesBuildMutable() {
    return this->enemy_builds;
}

const Cell Field::getCell(const Position & position) const { return this->map[position.getY()][position.getX()]; }

void Field::incrementSteps() { this->steps++; }

int Field::getSteps() const { return this->steps; }

void Field::addTrap(const Trap &trap) {
    this->traps.push_back(trap);
}

const std::list<Trap> &Field::getTraps() const {
    return this->traps;
}

std::list<Trap> &Field::getTrapsMutable() {
    return this->traps;
}

std::list<EnemyTower> &Field::getEnemyTowersMutable() {
    return this->towers;
}

const std::list<EnemyTower> &Field::getEnemyTowers() const {
    return this->towers;
}

void Field::setPlayer(std::shared_ptr<Player> player) {
    this->player = player;
}

void Field::load(Reader &reader) {
    reader.read(steps);
    reader.read(height);
    reader.read(width);

    map = new Cell *[height];

    for (int y = 0; y < height; y++) {
        map[y] = new Cell[width];
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map[y][x].load(reader);
        }
    }

    int count;

    reader.read(count);
    enemy_builds.clear();
    for (int i = 0; i < count; i++) {
        EnemyBuild b;
        b.load(reader);
        enemy_builds.push_back(b);
    }

    reader.read(count);
    enemies.clear();
    for (int i = 0; i < count; i++) {
        Enemy e;
        e.load(reader);
        enemies.push_back(e);
    }

    reader.read(count);
    traps.clear();
    for (int i = 0; i < count; i++) {
        Trap t;
        t.load(reader);
        traps.push_back(t);
    }

    reader.read(count);
    towers.clear();
    for (int i = 0; i < count; i++) {
        EnemyTower t;
        t.load(reader);
        towers.push_back(t);
    }

}


void Field::save(Saver &saver) const {
    saver.save(steps);
    saver.save(height);
    saver.save(width);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            map[y][x].save(saver);
        }
    }

    saver.save((int)enemy_builds.size());
    for (auto &build : enemy_builds) build.save(saver);

    saver.save((int)enemies.size());
    for (auto &enemy : enemies) enemy.save(saver);

    saver.save((int)traps.size());
    for (auto &trap : traps) trap.save(saver);

    saver.save((int)towers.size());
    for (auto &tower : towers) tower.save(saver);

}









