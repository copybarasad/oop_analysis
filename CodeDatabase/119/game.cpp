#include "game.h"
#include "console_input.h"
#include <stdexcept> 

void Game::saveGame(const std::string &filename)
{
    saver.save(filename, *this);
}

void Game::loadGame(const std::string &filename)
{
    loader.load(filename, *this);
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
    system("cls");
    Prints::print_upgrade_screen_header();

    bool upgradesRunning = true;
    while (upgradesRunning)
    {
        Prints::print_upgrade_options(player.getMaxHp(), player.hand.getMaxSize());

        std::string choice;
        std::cin >> choice;
        GameCommand cmd = ConsoleInput().parseCommandString(choice);

        switch (cmd.type)
        {
        case CommandType::UpgradeHp:
            player.increaseMaxHp(20);
            upgradesRunning = false;
            break;
        
        case CommandType::UpgradeHand:
            player.hand.increaseMaxSize();
            upgradesRunning = false;
            break;
        case CommandType::UpgradeSpell:
            {
                if (player.hand.getSpellCount() == 0)
                {
                    Prints::print_hand_is_empty_string();
                    break;
                }

                bool upgradeSpellRunning = true;
                while (upgradeSpellRunning)
                {
                    Prints::print_spell_upgrade_string();
                    player.hand.display();
                    Prints::print_prompt_for_upgrade();
                    std::string spell_choice;
                    std::cin >> spell_choice;
                    GameCommand cmd1 = ConsoleInput().parseCommandString(spell_choice);

                    if (cmd1.type == CommandType::BackMenu)
                    {
                        upgradeSpellRunning = false;
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
                break;
            }

        default:
            Prints::print_incorrect_input_string();
            break;
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
