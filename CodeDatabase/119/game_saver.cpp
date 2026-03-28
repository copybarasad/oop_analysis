#include "game_saver.h"
#include "game.h"
#include "game_state.h"
#include <fstream>
#include <stdexcept>
#include "fileOpenError.h"

void GameSaver::save(const std::string &filename, const Game &game)
{
    // Сначала собираем всё в структуру
    GameStateData state;
    
    state.currentLevel = game.getCurrentLevel();
    state.fieldWidth = game.getField().getSizeX();
    state.fieldHeight = game.getField().getSizeY();
    
    // Сохранение поля
    state.field.resize(state.fieldHeight, std::vector<int>(state.fieldWidth));
    const GameField& field = game.getField();
    for (int y = 0; y < state.fieldHeight; ++y)
    {
        for (int x = 0; x < state.fieldWidth; ++x)
        {
            state.field[y][x] = field.getCell(y, x).getType();
        }
    }

    // Сохранение игрока
    const Player& player = game.getPlayer();
    state.player.posX = player.getPosX();
    state.player.posY = player.getPosY();
    state.player.hp = player.getHp();
    state.player.xp = player.getXp();
    state.player.maxHp = player.getMaxHp();
    state.player.handMaxSize = player.hand.getMaxSize();
    state.player.spellCount = player.hand.getSpellCount();

    for (const auto &spell : player.hand.getSpells())
    {
        state.player.spells.push_back({spell->getTypeId(), spell->getDmg()});
    }

    // Сохранение врагов
    const Enemy& enemy = game.getEnemy();
    state.enemy.posX = enemy.getPosX();
    state.enemy.posY = enemy.getPosY();
    state.enemy.hp = enemy.getHp();
    state.enemy.isAlive = enemy.isAlive();

    const Enemy& enemy2 = game.getEnemy2();
    state.enemy2.posX = enemy2.getPosX();
    state.enemy2.posY = enemy2.getPosY();
    state.enemy2.hp = enemy2.getHp();
    state.enemy2.isAlive = enemy2.isAlive();

    const EnemyTower& tower = game.getEnemyTower();
    state.enemyTower.posX = tower.getPosX();
    state.enemyTower.posY = tower.getPosY();
    state.enemyTower.hp = tower.getHp();
    state.enemyTower.isAlive = tower.isAlive();

    // Теперь записываем всю структуру в файл
    std::ofstream saveFile(filename);
    if (!saveFile.is_open())
    {
        throw FileOpenError(filename);
    }

    saveFile << state.currentLevel << std::endl;

    // Запись поля
    for (int y = 0; y < state.fieldHeight; ++y)
    {
        for (int x = 0; x < state.fieldWidth; ++x)
        {
            saveFile << state.field[y][x] << " ";
        }
        saveFile << std::endl;
    }

    // Запись игрока
    saveFile << state.player.posX << " " << state.player.posY << " "
             << state.player.hp << " " << state.player.xp << " "
             << state.player.maxHp << " " << state.player.handMaxSize << " "
             << state.player.spellCount << std::endl;

    for (const auto &spell : state.player.spells)
    {
        saveFile << spell.first << " " << spell.second << std::endl;
    }

    // Запись врагов
    saveFile << state.enemy.posX << " " << state.enemy.posY << " "
             << state.enemy.hp << " " << state.enemy.isAlive << std::endl;

    saveFile << state.enemy2.posX << " " << state.enemy2.posY << " "
             << state.enemy2.hp << " " << state.enemy2.isAlive << std::endl;

    saveFile << state.enemyTower.posX << " " << state.enemyTower.posY << " "
             << state.enemyTower.hp << " " << state.enemyTower.isAlive << std::endl;

    saveFile.close();
}
