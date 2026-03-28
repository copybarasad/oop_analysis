#include <Game.hpp>
#include <Utils.hpp> 
#include <Player.hpp>
#include <Enemy.hpp>
#include <EnemyBase.hpp>
#include <ArcherTower.hpp>
#include <DecoyAlly.hpp>
#include <PlayerTrap.hpp>
#include <SpikedTrap.hpp>
#include <Constants.hpp>
#include <Wall.hpp>
#include <Floor.hpp>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include <iostream>

Game::Game() : field(), menu(0, 0) {}

void Game::initScreen() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    set_escdelay(25);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, 35, COLOR_BLACK); 
    }

    getmaxyx(stdscr, screen.yMax, screen.xMax);
    field.setScreen(&screen);
    menu = Menu(screen.yMax, screen.xMax);
}

void Game::cleanup() {
    clear();
    endwin();
}

bool Game::help() {
    std::cout << "\n";
    std::cout << "=============================================\n";
    std::cout << "            ДОБРО ПОЖАЛОВАТЬ В ИГРУ!\n";
    std::cout << "=============================================\n\n";

    std::cout << "Перед запуском убедитесь, что:\n";
    std::cout << "  • Размер шрифта терминала — не более 12pt\n";
    std::cout << "  • Окно терминала развернуто на весь экран\n\n";

    std::cout << "УПРАВЛЕНИЕ:\n";
    std::cout << "  Стрелки — движение по локации\n";
    std::cout << "  TAB     — сменить оружие (ближний/дальний бой)\n";
    std::cout << "  Пробел  — пропустить ход / выбраться из ловушки\n";
    std::cout << "  ESC     — выйти из игры (в меню паузы)\n";
    std::cout << "  E       — заблокировать удар во время боя\n\n";

    std::cout << "Чтобы начать игру, введите:  start\n";
    std::cout << "Чтобы выйти, введите:        exit\n";
    std::cout << "\n";

    std::string command;
    while (true) {
        std::cout << ">>> ";
        std::getline(std::cin, command);

        if (command == "start") {
            return true; 
        } else if (command == "exit") {
            std::cout << "Выход из игры.\n";
            return false; 
        } else {
            std::cout << "\nНеизвестная команда. Введите 'start' или 'exit'.\n";
        }
    }
}

void Game::start() {
    initScreen();

    int menuChoice = menu.init(field);

    bool shouldStartGame = false;

    if (menuChoice == 1) { 
        field.init(); 
        shouldStartGame = true;
    } else if (menuChoice == 2) { 

        shouldStartGame = true;
    }

    if (shouldStartGame) {
        bool playerIsAlive = true;
        bool gameWon = false; 

        clear();
        field.draw();
        refresh();

        while (playerIsAlive && !gameWon) { 
            int ch = getch();

            if (ch == ESC) {
                if (!menu.openPauseMenu(field)) {
                    break;
                }
                clear();
                field.draw();
                refresh();
                continue; 
            }

            field.getPlayer()->setTargeted(false);

            if (!field.updatePlayer(ch)) {
                continue;
            }

            field.updateEnemies(); 
            field.updateStructures();

            field.cleanupDeadEntities();

            if (!field.getPlayer() || field.getPlayer()->getHealth() <= 0) {
                playerIsAlive = false;
                continue;
            }

            field.updateAllies();
            field.cleanupDeadEntities();

            if (field.isLevelClear()) {
                if (field.getCurrentLevel() >= MAX_LEVEL) {
                    gameWon = true;
                } else {
                    clear();
                    mvprintw(screen.yMax / 2, screen.xMax / 2 - 10, "LEVEL %d COMPLETE!", field.getCurrentLevel());
                    refresh();
                    napms(2500); 

                    field.getPlayer()->prepareForNextLevel(field.getCurrentLevel() + 1);
                    field.advanceToNextLevel();
                }
            }

            clear();
            field.draw();
            refresh();
        }

        clear();
        refresh();
        if (gameWon) {
            mvprintw(screen.yMax / 2, screen.yMax / 2 - 5, "YOU WON!");
            refresh();
            getch();
        } else if (!playerIsAlive) {
            menu.gameOver();
        }
    }

    cleanup();
}
