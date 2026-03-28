#include "game_loader.h"
#include "game.h"
#include "game_state.h"
#include <fstream>
#include <stdexcept>
#include "fileOpenError.h"
#include "saveDataError.h"

void GameLoader::load(const std::string &filename, Game &game)
{
    std::ifstream loadFile(filename);
    if (!loadFile.is_open())
    {
        throw FileOpenError(filename);
    }

    GameStateData tempState;
    
    if (!(loadFile >> tempState.currentLevel))
    {
        throw SaveDataError("Ошибка: не удалось прочитать номер уровня из файла сохранения.");
    }

    tempState.fieldWidth = 20 + (tempState.currentLevel - 1) * 2;
    tempState.fieldHeight = 10 + (tempState.currentLevel - 1);
    tempState.field.resize(tempState.fieldHeight, std::vector<int>(tempState.fieldWidth));

    int cellType;
    for (int y = 0; y < tempState.fieldHeight; ++y)
    {
        for (int x = 0; x < tempState.fieldWidth; ++x)
        {
            if (!(loadFile >> cellType))
            {
                throw SaveDataError("Ошибка: некорректные или неполные данные поля в файле сохранения.");
            }
            tempState.field[y][x] = cellType;
        }
    }

    int x, y, hp, xp, maxHp, handMaxSize, spellCount;
    if (!(loadFile >> x >> y >> hp >> xp >> maxHp >> handMaxSize >> spellCount))
    {
        throw SaveDataError("Ошибка: некорректные данные игрока в файле сохранения.");
    }
    
    tempState.player.posX = x;
    tempState.player.posY = y;
    tempState.player.hp = hp;
    tempState.player.xp = xp;
    tempState.player.maxHp = maxHp;
    tempState.player.handMaxSize = handMaxSize;
    tempState.player.spellCount = spellCount;

    for (int i = 0; i < spellCount; ++i)
    {
        int typeId, dmg;
        if (!(loadFile >> typeId >> dmg))
        {
            throw SaveDataError("Ошибка: некорректные данные заклинания в файле сохранения.");
        }
        tempState.player.spells.push_back({typeId, dmg});
    }

    bool isAlive;
    if (!(loadFile >> x >> y >> hp >> isAlive))
    {
        throw SaveDataError("Ошибка: некорректные данные в файле сохранения (враг 1).");
    }
    tempState.enemy.posX = x;
    tempState.enemy.posY = y;
    tempState.enemy.hp = hp;
    tempState.enemy.isAlive = isAlive;

    if (!(loadFile >> x >> y >> hp >> isAlive))
    {
        throw SaveDataError("Ошибка: некорректные данные в файле сохранения (враг 2).");
    }
    tempState.enemy2.posX = x;
    tempState.enemy2.posY = y;
    tempState.enemy2.hp = hp;
    tempState.enemy2.isAlive = isAlive;

    if (!(loadFile >> x >> y >> hp >> isAlive))
    {
        throw SaveDataError("Ошибка: некорректные данные в файле сохранения (башня).");
    }
    tempState.enemyTower.posX = x;
    tempState.enemyTower.posY = y;
    tempState.enemyTower.hp = hp;
    tempState.enemyTower.isAlive = isAlive;

    loadFile.close();

    // Если всё прочиталось успешно, то применяем состояние к игре
    game.setCurrentLevel(tempState.currentLevel);
    
    GameField& field = game.getField();
    field.reinitialize(tempState.fieldWidth, tempState.fieldHeight);
    Entity::currentField = &field;

    for (int y = 0; y < tempState.fieldHeight; ++y)
    {
        for (int x = 0; x < tempState.fieldWidth; ++x)
        {
            field.setCellType(y, x, static_cast<CellType>(tempState.field[y][x]));
        }
    }

    Player& player = game.getPlayer();
    player.setPos(tempState.player.posX, tempState.player.posY);
    player.setHp(tempState.player.hp);
    player.setXp(tempState.player.xp);
    player.setMaxHp(tempState.player.maxHp);
    player.setEntityOnField(field);

    player.hand.setMaxSize(tempState.player.handMaxSize);
    player.hand.clearSpells();
    for (const auto &spellData : tempState.player.spells)
    {
        std::unique_ptr<ISpell> newSpell;

        switch (static_cast<SpellType>(spellData.first))
        {
        case SINGLE:
            newSpell = std::make_unique<SingleTarget>();
            break;
        case AREA:
            newSpell = std::make_unique<AreaDamage>();
            break;
        case TRAP:
            newSpell = std::make_unique<Traps>();
            break;
        }

        if (newSpell)
        {
            newSpell->setDmg(spellData.second);
            player.hand.addSpell(std::move(newSpell));
        }
    }

    Enemy& enemy = game.getEnemy();
    enemy.setAlive(tempState.enemy.isAlive);
    if (tempState.enemy.isAlive)
    {
        enemy.setPos(tempState.enemy.posX, tempState.enemy.posY);
        enemy.setHp(tempState.enemy.hp);
        enemy.setEntityOnField(field);
    }

    Enemy& enemy2 = game.getEnemy2();
    enemy2.setAlive(tempState.enemy2.isAlive);
    if (tempState.enemy2.isAlive)
    {
        enemy2.setPos(tempState.enemy2.posX, tempState.enemy2.posY);
        enemy2.setHp(tempState.enemy2.hp);
        enemy2.setEntityOnField(field);
    }

    EnemyTower& tower = game.getEnemyTower();
    tower.setAlive(tempState.enemyTower.isAlive);
    if (tempState.enemyTower.isAlive)
    {
        tower.setPos(tempState.enemyTower.posX, tempState.enemyTower.posY);
        tower.setHp(tempState.enemyTower.hp);
        tower.setEntityOnField(field);
    }
}
