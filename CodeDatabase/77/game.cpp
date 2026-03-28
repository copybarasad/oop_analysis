#include "game.h"
#include <fstream>
#include <stdexcept> 

void Game::saveGame(const std::string &filename)
{
    std::ofstream saveFile(filename);
    if (!saveFile.is_open())
    {
        throw std::runtime_error("Ошибка: не удалось открыть файл для сохранения.");
    }

    saveFile << currentLevel << std::endl;

    for (int y = 0; y < field.getSizeY(); ++y)
    {
        for (int x = 0; x < field.getSizeX(); ++x)
        {

            saveFile << field.getCell(y, x).getType() << " ";
        }
        saveFile << std::endl;
    }

    saveFile << player.getPosX() << " " << player.getPosY() << " "
             << player.getHp() << " " << player.getXp() << " "
             << player.getMaxHp() << " " << player.hand.getMaxSize() << " "
             << player.hand.getSpellCount() << std::endl;

    for (const auto &spell : player.hand.getSpells())
    {
        saveFile << spell->getTypeId() << " " << spell->getDmg() << std::endl;
    }

    saveFile << enemy.getPosX() << " " << enemy.getPosY() << " "
             << enemy.getHp() << " " << enemy.isAlive() << std::endl;

    saveFile << enemy2.getPosX() << " " << enemy2.getPosY() << " "
             << enemy2.getHp() << " " << enemy2.isAlive() << std::endl;

    saveFile << enemy_tower.getPosX() << " " << enemy_tower.getPosY() << " "
             << enemy_tower.getHp() << " " << enemy_tower.isAlive() << std::endl;

    saveFile.close();
}

void Game::loadGame(const std::string &filename)
{
    std::random_device rd;
    gen = std::mt19937(rd()); 

    std::ifstream loadFile(filename);
    if (!loadFile.is_open())
    {
        throw std::runtime_error("Ошибка: файл сохранения не найден или не может быть открыт.");
    }
    if (!(loadFile >> currentLevel))
    {
        throw std::runtime_error("Ошибка: не удалось прочитать номер уровня из файла.");
    }

    int newWidth = 20 + (currentLevel - 1) * 2;
    int newHeight = 10 + (currentLevel - 1);
    field.reinitialize(newWidth, newHeight);

    Entity::currentField = &field;

    int cellType;
    for (int y = 0; y < field.getSizeY(); ++y)
    {
        for (int x = 0; x < field.getSizeX(); ++x)
        {
            if (!(loadFile >> cellType))
            {
                throw std::runtime_error("Ошибка: некорректные или неполные данные поля в файле сохранения.");
            }

            field.setCellType(y, x, static_cast<CellType>(cellType));
        }
    }
    int x, y, hp, xp, maxHp, handMaxSize, spellCount;
    if (!(loadFile >> x >> y >> hp >> xp >> maxHp >> handMaxSize >> spellCount))
    {
        throw std::runtime_error("Ошибка: некорректные данные игрока в файле.");
    }
    player.setPos(x, y);
    player.setHp(hp);
    player.setXp(xp);
    player.setMaxHp(maxHp);
    player.setEntityOnField(field);

    player.hand.setMaxSize(handMaxSize);
    player.hand.clearSpells();
    for (int i = 0; i < spellCount; ++i)
    {
        int typeId, dmg;
        loadFile >> typeId >> dmg;

        std::unique_ptr<ISpell> newSpell;

        switch (static_cast<SpellType>(typeId))
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
            newSpell->setDmg(dmg); 
            player.hand.addSpell(std::move(newSpell));
        }
    }

    bool isAlive;
    if (!(loadFile >> x >> y >> hp >> isAlive))
    {
        throw std::runtime_error("Ошибка: некорректные данные в файле сохранения (враг 1).");
    }
    enemy.setAlive(isAlive); 
    if (isAlive)
    {
        enemy.setPos(x, y);
        enemy.setHp(hp);
        enemy.setEntityOnField(field);
    }

    if (!(loadFile >> x >> y >> hp >> isAlive))
    {
        throw std::runtime_error("Ошибка: некорректные данные в файле сохранения (враг 2).");
    }
    enemy2.setAlive(isAlive);
    if (isAlive)
    {
        enemy2.setPos(x, y);
        enemy2.setHp(hp);
        enemy2.setEntityOnField(field);
    }

    if (!(loadFile >> x >> y >> hp >> isAlive))
    {
        throw std::runtime_error("Ошибка: некорректные данные в файле сохранения (башня).");
    }
    enemy_tower.setAlive(isAlive);
    if (isAlive)
    {
        enemy_tower.setPos(x, y);
        enemy_tower.setHp(hp);
        enemy_tower.setEntityOnField(field);
    }

    loadFile.close();
}

Game::Game() : field(20, 10), currentLevel(1) {}

void Game::startNewGame()
{
    currentLevel = 1;
    player.setHp(100); 
    player.setXp(0);   

    player.hand.addSpell(createRandomSpell());

    generateLevel(); 
}

void Game::generateLevel()
{
    Prints::print_level_generation(currentLevel);

    int newWidth = 20 + (currentLevel - 1) * 2;
    int newHeight = 10 + (currentLevel - 1);
    field.reinitialize(newWidth, newHeight); 

    Entity::currentField = &field; 

    player.setPos(newWidth / 2, newHeight / 2);
    player.setEntityOnField(field);

    int enemyBaseHp = 40 + currentLevel * 10;
    int towerBaseHp = 50 + currentLevel * 25;

    enemy = Enemy(0, 0, enemyBaseHp);
    enemy2 = Enemy(0, 0, enemyBaseHp);
    enemy_tower = EnemyTower(3, 3, 8, towerBaseHp);

    std::uniform_int_distribution<> distX(1, newWidth - 1);
    std::uniform_int_distribution<> distY(1, newHeight - 1);

    enemy.setPos(distX(gen), distY(gen));
    enemy2.setPos(distX(gen), distY(gen));
    enemy_tower.setPos(distX(gen), distY(gen));

    enemy.setEntityOnField(field);
    enemy2.setEntityOnField(field);
    enemy_tower.setEntityOnField(field);
}

void Game::startUpgrades() {
    system("clear");
    Prints::print_upgrade_screen_header();

    while (true)
    {

        Prints::print_upgrade_options(player.getMaxHp(), player.hand.getMaxSize());

        std::string choice;
        std::cin >> choice;

        if (choice == "1")
        {
            player.increaseMaxHp(20);
            break;
	}
        else if (choice == "2")
        {
            player.hand.increaseMaxSize();
            break;
        }
        else if (choice == "3")
        {
            if (player.hand.getSpellCount() == 0)
            {
                Prints::print_hand_is_empty_string();
                continue;
            }

            while (true)
            {
                Prints::print_spell_upgrade_string();
                player.hand.display();
                Prints::print_prompt_for_upgrade();
                std::string spell_choice;
                std::cin >> spell_choice;

                if (spell_choice == "back")
                {
                    break;
                }

                try
                {
                    int index = std::stoi(spell_choice);
                    if (index < player.hand.getSpellCount())
                    {
                        player.hand.upgradeSpell(index);
                        return;
                    }
                    else
                    {
                        Prints::print_incorrect_spell_string();
                    }
                }
                catch (const std::invalid_argument &)
                {
                    Prints::print_incorrect_input_string();
                }
            }
        }
        else
        {
            Prints::print_incorrect_input_string();
        }
    }
}

void Game::nextLevel()
{
    Prints::print_level_completed(currentLevel);
    startUpgrades();
    Prints::print_getting_ready_string();
    // system("pause"); 

    currentLevel++;

    player.setHp(player.getMaxHp()); 

    int spellsToRemove = player.hand.getSpellCount() / 2;

    for (int i = 0; i < spellsToRemove; ++i)
    {
        if (player.hand.getSpellCount() > 0)
        {
            player.hand.removeSpell();
        }
    }
    Prints::print_spell_removed(spellsToRemove);
    generateLevel();
}

void Game::processTowerTurn(){
    enemy_tower.update();
    enemy_tower.tryAttack(enemy_tower.getPosX(), enemy_tower.getPosY(), &enemy_tower);
}

void Game::switchPlayerMode()
{
    player.switchMode();
}

void Game::playerCastSpell(int spellIndex)
{
    player.castSpell(spellIndex);
}

void Game::playerBuySpell()
{    
    const int spellCost = 5; 

    if (player.getXp() >= spellCost) 
    {
        player.hand.addSpell(createRandomSpell());
        player.setXp(player.getXp() - spellCost);
    } 
    else 
    {
        Prints::print_no_xp_string();
    }
}

void Game::movePlayer(int dx, int dy) {
    player.move(dx, dy, field);
    Prints::print_player_movement(player.getPosX(), player.getPosY());
}

void Game::playerAttack() {
    if (player.mode == RANGED){
        player.rangedAttack(&enemy);
    } else {
        Prints::print_incorrect_mode_string(); 
    }
}

void Game::updateWorld() {
    // Логика врагов перенесена сюда
    if (enemy.isAlive()) {
        auto enemy_move = enemy.calculate_new_move(player, field);
        enemy.move(enemy_move[0], enemy_move[1], field);
    }
    if (enemy2.isAlive()) {
        auto enemy_move = enemy2.calculate_new_move(player, field);
        enemy2.move(enemy_move[0], enemy_move[1], field);
    }
    if (enemy_tower.isAlive()) processTowerTurn();

    if (!enemy.isAlive() && !enemy2.isAlive() && !enemy_tower.isAlive()) {
        nextLevel();
    }
}

void Game::renderGameStatus() const {

    Prints::print_game_status(player.isAlive() ? player.getHp() : 0, player.getXp(), \
                                    (enemy.isAlive() ? enemy.getHp() : 0),
                                    (enemy2.isAlive() ? enemy2.getHp() : 0), \
                                    (enemy_tower.isAlive() ? enemy_tower.getHp() : 0));
    player.hand.display();
    field.showField();
}

bool Game::isGameOver() const {
    return !player.isAlive();
}

void Game::update() {
    if (enemy.isAlive()) {
        auto enemy_move = enemy.calculate_new_move(player, field);
        enemy.move(enemy_move[0], enemy_move[1], field);
    }
    if (enemy2.isAlive()) {
        auto enemy_move = enemy2.calculate_new_move(player, field);
        enemy2.move(enemy_move[0], enemy_move[1], field);
    }

    if (enemy_tower.isAlive()) {
        processTowerTurn();
    }

    if (!enemy.isAlive() && !enemy2.isAlive() && !enemy_tower.isAlive()) {
        nextLevel();
    }
}
