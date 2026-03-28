#include "level.hpp"
#include "game.hpp"
#include "ise_ball.hpp"
#include <random>
#include "game_framework.hpp"

#define ENEMY_BASE_COUNT 3

Level::Level(Game* parentGame,int number, int width, int height) 
    : game(parentGame), levelNumber(number), field(width, height), player(100, 15), spellHand(3), completed(false), failed(false) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disX(1, width - 1);
    std::uniform_int_distribution<> disY(1, height - 1);
    int enemyCount = ENEMY_BASE_COUNT + levelNumber;

    //усиление
    int enemyHP = 25 + levelNumber * 5;//30 35 40
    int enemyDamage = 10 + levelNumber * 5;//15 20 25

    for (int i = 0; i < enemyCount; ++i) {
        int x, y;
        do {
            x = disX(gen);
            y = disY(gen);
        } while (!field.getField()[x][y].get_isPass() ||
                 (x == player.get_x() && y == player.get_y()));

        //создание услиленных
        Enemy enemy(x, y);
        enemy.setHP(enemyHP);
        enemy.setDamage(enemyDamage);
        enemies.push_back(enemy);
    }
    auto startingSpell = SpellFactory::createRandomSpell();
    spellHand.addSpell(std::move(startingSpell));
}

bool Level::run() {
    //новое
    ConsoleInputReader input;
    GameVisualizer<ConsoleRenderer> visualizer;

    visualizer.renderLevelHeader();

    while (player.is_alive() && !completed) {
        visualizer.renderStatus(player);
        visualizer.renderField(field, player, enemies);
        visualizer.renderEnemyInfo(enemies);
        spellHand.display_hand();

        if (player.getIsRangedCombat()) {
            std::cout << "Ranged attack: xw(up), xs(down), xa(left), xd(right)" << std::endl;
        }

        if (allEnemiesDead()) {
            completed = true;
            std::cout << "LEVEL " << levelNumber << " COMPLETED!" << std::endl;
            return true;
        }
        std::cout << "Command: ";
        LevelCommand command = input.readLevelCommand();

        auto applyDirection = [](Direction dir, int &dx, int &dy) {
            dx = 0;
            dy = 0;
            switch (dir) {
                case Direction::Up:    dy = -1; break;
                case Direction::Down:  dy = 1;  break;
                case Direction::Left:  dx = -1; break;
                case Direction::Right: dx = 1;  break;
                case Direction::None: default: break;
            }
        };

        bool attacked = false;
        switch (command.type) {
            case LevelCommandType::Move: {
                int dx = 0, dy = 0;
                applyDirection(command.direction, dx, dy);
                player.move(dx, dy, field.getField(), enemies);
                break;
            }
            case LevelCommandType::RangedAttack: {
                if (player.getIsRangedCombat()) {
                    int dx = 0, dy = 0;
                    applyDirection(command.direction, dx, dy);
                    player.rangedAttack(dx, dy, enemies);
                    attacked = true;
                }
                break;
            }
            case LevelCommandType::SwitchStyle: {
                player.switch_style();
                std::cout << "Combat style: " << (player.getIsRangedCombat() ? "Ranged" : "Melee") << std::endl;
                break;
            }
            case LevelCommandType::QuitLevel: {
                std::cout << "Returning to main menu..." << std::endl;
                completed = false; // не завершен
                failed = false;    // не проигран
                return false;      // просто выход
            }
            case LevelCommandType::BuySpell: {
                offerSpellPurchase();
                continue;
            }
            case LevelCommandType::SaveGame: {
                if (game) {
                    game->saveGame();
                }
                continue;
            }
            case LevelCommandType::ShowSpells: {
                spellHand.display_hand();
                continue;
            }
            case LevelCommandType::UseSpell: {
                int spellIndex = command.spellIndex;
                if (spellIndex >= 0 && spellIndex < spellHand.get_spellCount()) {
                    std::string spellType = spellHand.getSpell(spellIndex)->getType();
                    if (spellType == "DIRECT_DAMAGE") {
                        std::cout << "Direction (w/s/a/d): ";
                        std::string direction;
                        std::cin >> direction;
                        int dx = 0, dy = 0;
                        if (direction == "w") dy = -1;
                        else if (direction == "s") dy = 1;
                        else if (direction == "a") dx = -1;
                        else if (direction == "d") dx = 1;
                        else {
                            std::cout << "Invalid direction!" << std::endl;
                            continue;
                        }
                        spellHand.useSpell(spellIndex, player, dx, dy, enemies, field);
                    } else {
                        spellHand.useSpell(spellIndex, player, 0, 0, enemies, field);
                    }
                    processEnemyTurns();
                }
                continue;
            }
            case LevelCommandType::None:
            default:
                // Неверная или пустая команда
                continue;
        }

        if (command.type != LevelCommandType::SwitchStyle || attacked) {
            processEnemyTurns();
        }
        if (!player.is_alive()) {
            failed = true;
            std::cout << "GAME OVER! Your score: " << player.get_score() << std::endl;
            break;
        }
        std::cout << "---" << std::endl;
    }
    return completed;
}

void Level::processEnemyTurns() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-1, 1);

    for (auto &enemy : enemies) {
        if (enemy.is_alive()) {
            int dy = dis(gen);
            int dx = dis(gen);
            enemy.move(dx, dy, field.getField(), player, enemies);
        }
    }
}

void Level::displayStatus() const {
    std::cout << "Health: " << player.get_hp() << " | ";
    std::cout << "Score: " << player.get_score() << " | ";
    std::cout << "Style: " << (player.getIsRangedCombat() ? "Ranged" : "Melee") << " | ";
    std::cout << "Damage: " << player.get_damage() << std::endl;
}

void Level::displayEnemyInfo() const {
    std::cout << "Enemies: ";
    bool anyAlive = false;
    for (size_t i = 0; i < enemies.size(); ++i) {
        if (enemies[i].is_alive()) {
            std::cout << "E" << i+1 << "(" << enemies[i].get_hp() << "HP) ";
            anyAlive = true;
        }
    }
    if (!anyAlive) {
        std::cout << "none";
    }
    std::cout << std::endl;
}

bool Level::allEnemiesDead() const {
    for (const auto& enemy : enemies) {
        if (enemy.is_alive()) {
            return false;
        }
    }
    return true;
}

void Level::offerSpellPurchase() {
    if (player.get_score() < 10) {
        std::cout << "Need 10 points (you have " << player.get_score() << ")" << std::endl;
        return;
    }
    
    if (spellHand.isFull()) {
        std::cout << "Hand is full!" << std::endl;
        return;
    }

    bool hasIceBall = false;
    for (int i = 0; i < spellHand.get_spellCount(); i++) {
        const Spell* handSpell = spellHand.getSpell(i);
        if (handSpell->getType() == "DIRECT_DAMAGE") {
            hasIceBall = true;
            break;
        }
    }

    if (hasIceBall) {
        std::cout << "You already have Ice Ball!" << std::endl;
        return;
    }

    std::cout << "Buy Ice Ball for 10 points? (y/n): ";
    char choice;
    std::cin >> choice;
    
    if (choice == 'y') {
        if (player.spendScore(10)) {
            auto newSpell = std::make_unique<IseBall>();
            spellHand.addSpell(std::move(newSpell));
            std::cout << "Ice Ball purchased! Points: " << player.get_score() << std::endl;
        }
    } else {
        std::cout << "Purchase cancelled." << std::endl;
    }
}

void Level::reset() {
    completed = false;
    failed = false;
}