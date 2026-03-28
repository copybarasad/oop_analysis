#include "ConsoleRenderer.h"
#include "../game_objects/EnemyTower.h"
#include <iostream>

// Отрисовка поля
void ConsoleRenderer::drawField(const GameField& field, const std::vector<Character*>& characters, 
                        const std::vector<EnemyBuilding*>& buildings, const std::vector<EnemyTower*>& towers, const std::vector<Ally*>& allies) {
    int width = field.getWidth();
    int height = field.getHeight();

    // Cимволы рамки
    const char corner = '+';
    const char horizontal = '-';
    const char vertical = '|';
    
    std::cout << Colors::BROWN << corner;
    for (int i = 0; i < width * 2; ++i)
        std::cout << horizontal;
    std::cout << corner << std::endl;

    for (int y = 0; y < height; ++y) {
        std::cout << vertical << Colors::RESET;
        for (int x = 0; x < width; ++x) {

            // Отрисовка персонажей
            bool characterHere = false;
            for (const auto& character : characters) {
                if (character->isAlive() && x == character->getX() && y == character->getY()) {
                    char symbol = character->getSymbol();
                    if (symbol == 'P') {
                        std::cout << Colors::PLAYER << symbol << ' ' << Colors::RESET;
                    } else if (symbol == 'E') {
                        std::cout << Colors::ENEMY << symbol << ' ' << Colors::RESET;
                    } 

                    characterHere = true;
                    break;
                }
            }

            // Иначе если не игрок -> проверяем здания
            if (!characterHere) {

                // Отрисовка здания
                bool buildingHere = false;
                for (const auto& building : buildings) {
                    if (building->isAlive() && x == building->getX() && y == building->getY()) {
                        std::cout << Colors::ORANGE << building->getSymbol() << ' ' << Colors::RESET;
                        buildingHere = true;
                        break;
                    }
                }
                
                // Иначе если не здание -> проверяем башни
                if (!buildingHere) {
                    bool towerHere = false;
                    for (const auto& tower : towers) {
                        if (tower->isAlive() && x == tower->getX() && y == tower->getY()) {
                            std::cout << Colors::ORANGE << tower->getSymbol() << ' ' << Colors::RESET;
                            towerHere = true;
                            break;
                        }
                    }
                    
                    // Иначе если не башня -> проверяем союзников
                    if (!towerHere) {
                        bool allyHere = false;
                        for (const auto& ally : allies) {
                            if (ally->isAlive() && x == ally->getX() && y == ally->getY()) {
                                std::cout << Colors::PLAYER << ally->getSymbol() << ' ' << Colors::RESET;
                                allyHere = true;
                                break;
                            }
                        }
                        
                        // Обычная клетка
                        if (!allyHere) {
                            const Cell& cell =  field.getCell(x, y);
                            char symbol = '.';
                            std::string color = Colors::EMPTY;

                            if (!cell.getIsPassable()) {
                                symbol = '#'; // стена (непроходимая клетка)
                                color = Colors::WALL;
                            } else if (cell.getIsSlowing()) {
                                symbol = '~'; // замедляющая клетка
                                color = Colors::SLOW;
                            } else if (cell.getHasTrap()) {
                                symbol = 'T';
                                color = Colors::DARK_RED;
                            }
                            std::cout << color << symbol << ' ' << Colors::RESET;
                        }
                    }
                }
            }
        }
        std::cout << Colors::BROWN << vertical << std::endl;
    } 
    std::cout << corner;
    for (int i = 0; i < width * 2; ++i)
        std::cout << horizontal;
    std::cout << corner <<  Colors::RESET << std::endl;
}

// Отрисовка информации об игроке
void ConsoleRenderer::drawPlayerInfo(const Player& player, const PlayerHand& hand) {
    std::cout << Colors::CYAN << "Player Info: " << Colors::RESET 
              << "HP: " << player.getHealth() 
              << " | Damage: " << player.getCurrentDamage()
              << " | CombatMode: " << (player.getCombatMode() == CombatMode::Melee ? "Melee" : "Ranged")
              << " | Score: " << player.getScore() << std::endl;

    std::cout << Colors::CYAN << "Player Hand: " << Colors::RESET;
    for (std::size_t i = 0; i < hand.getCurrentSize(); ++i) {
        auto spell = hand.getSpell(i);
        if (spell) {
            std::cout << "[" << i + 1 << "] " << spell->getName() << " ";
        } else {
            std::cout << "[" << i + 1 << "] (empty) ";
        }
    }
    if (hand.getCurrentSize() == 0) {
        std::cout << "No spells";
    }
    std::cout << std::endl;
}

// Вывод событий в игре
void ConsoleRenderer::drawMessages(const std::vector<std::string>& messages) {
    if (!messages.empty()) {
        std::cout << Colors::CYAN << "\n---------- Round info: ----------\n" << Colors::RESET;
        
        for (const auto& msg : messages) {
            std::cout << msg << std::endl;
        }
        std::cout << '\n';
    }
}

// Очистка консоли
void ConsoleRenderer::clearScreen() {
    #ifdef _WIN32
        system("cls");     // Windows
    #else
        system("clear");   // Linux/Mac
    #endif
}