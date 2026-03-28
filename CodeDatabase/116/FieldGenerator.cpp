#include "FieldGenerator.h"
#include "../spells/SpellManager.h"

FieldGenerator::FieldGenerator(int enemiesCount, int slowingCellsCount, int wallsCount, int fortressesCount, int lvl):enemiesCount(enemiesCount),wallsCount(wallsCount),slowingCellsCount(slowingCellsCount),fortressesCount(fortressesCount),lvl(lvl){}

FieldGenerator::~FieldGenerator() = default;

std::pair<int, int> FieldGenerator::getRandomEmptyPosition(GameWorld& world) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    auto& field = world.getField();
    int height = field.getHeight();
    int width = field.getWidth();
    std::uniform_int_distribution<> distX(0, width - 1);
    std::uniform_int_distribution<> distY(0, height - 1);
    while(true) {
        int x = distX(gen);
        int y = distY(gen);
        if (field.getCell(x, y).getType() == Cell::CellType::EMPTY and !(x == width/2 and y == height/2)) {
            return {x, y};
            break;
        }
    }
}

void FieldGenerator::spawnStartObjects(GameWorld& world) {
    auto& field = world.getField();
    int height = field.getHeight();
    int width = field.getWidth();
    field.getCell(width/2,height/2).setType(Cell::CellType::PLAYER);

    for (int i = 0; i < enemiesCount; ++i) {
        auto pos = getRandomEmptyPosition(world);
        spawnEnemy(world,pos.first,pos.second,30 + lvl, 8 + lvl);
    }

    for (int i = 0; i < fortressesCount; ++i) {
        auto pos = getRandomEmptyPosition(world);
        spawnFortress(world,pos.first,pos.second,40+lvl*2,2,5+lvl,3);
    }

    for (int i = 0; i < wallsCount; ++i){
        auto pos = getRandomEmptyPosition(world);
        field.getCell(pos.first, pos.second).setType(Cell::CellType::WALL);
    }

    for (int i = 0; i < slowingCellsCount; ++i) {
        auto pos = getRandomEmptyPosition(world);
        field.getCell(pos.first, pos.second).setSlowing();
    }

    auto pos = getRandomEmptyPosition(world);
    spawnBuilding(world,pos.first,pos.second);

    SpellManager spellManager;
    spellManager.addRandomSpell(world.getPlayer());
}

void FieldGenerator::spawnBuilding(GameWorld& world, int x, int y, int Health, int frequency) {
    world.setTower(x, y, Health, frequency);
    world.getField().getCell(x, y).setType(Cell::CellType::BUILDING);
}

void FieldGenerator::spawnEnemy(GameWorld& world,int x, int y, int health, int damage) {
    std::unique_ptr<Enemy> enemy(new Enemy(x,y,health,damage));
    auto& enemies = world.getEnemies();
    enemies.push_back(std::move(enemy));
    world.getField().getCell(x, y).setType(Cell::CellType::ENEMY);
}

void FieldGenerator::spawnAlly(GameWorld& world,int x, int y, int health, int damage) {
    std::unique_ptr<Ally> ally(new Ally(x,y, health, damage));
    auto& allies = world.getAllies();
    allies.push_back(std::move(ally));
    world.getField().getCell(x, y).setType(Cell::CellType::ALLY);
}

void FieldGenerator::spawnTrap(GameWorld& world,int x, int y,int damage) {
    std::unique_ptr<Trap> trap(new Trap(x,y,1,damage));
    auto& traps = world.getTraps();
    traps.push_back(std::move(trap));
    world.getField().getCell(x, y).setType(Cell::CellType::TRAP);
}

void FieldGenerator::spawnFortress(GameWorld& world,int x, int y, int health, int attackRad, int damage, int attackFrequency) {
    std::unique_ptr<Fortress> fortress(new Fortress(x,y,health,attackRad,damage,attackFrequency));
    auto& fortresses = world.getFortresses();
    fortresses.push_back(std::move(fortress));
    world.getField().getCell(x, y).setType(Cell::CellType::FORTRESS);
}