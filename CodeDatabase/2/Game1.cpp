#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameCycle.hpp"
#include "StartMenu.hpp"
#include <fstream>
#include "SaveLoadMenu.hpp"

int main() {
    try {
        std::cout << "=== OOP GAME ===" << std::endl;

        // Создаем окно для меню
        sf::RenderWindow menuWindow(sf::VideoMode({800, 600}), "OOP Game - Main Menu");

        // Меню запуска
        StartMenu startMenu;
        if (!startMenu.initialize()) {
            throw std::runtime_error("Failed to initialize start menu");
        }

        int choice = startMenu.show(menuWindow);
        menuWindow.close();

        if (choice == 3) {
            std::cout << "Game exited from main menu" << std::endl;
            return 0;
        }

        // Создаем игру
        GameCycle game;

        if (choice == 1) {
            // Новая игра - сначала проверяем автосохранение
            bool hasAutosave = fs::exists("autosave.dat");
            if (hasAutosave) {
                std::cout << "Autosave found. Start new game anyway? (y/n): ";
                char response;
                std::cin >> response;
                if (response == 'y' || response == 'Y') {
                    // Удаляем автосохранение и начинаем новую игру
                    fs::remove("autosave.dat");

                    // Начинаем новую игру с вводом размера
                    int length, width;
                    std::cout << "Enter board size (length width): ";
                    std::cin >> length >> width;

                    if (length < 10 || width < 10 || length > 25 || width > 25) {
                        std::cout << "Invalid size! Using default 15x15." << std::endl;
                        length = 15;
                        width = 15;
                    }

                    game.startNewGame(length, width);
                } else {
                    // Загружаем автосохранение
                    try {
                        game.loadGame("autosave.dat");
                        std::cout << "Autosave loaded successfully!" << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "Autosave load failed: " << e.what() << std::endl;
                        fs::remove("autosave.dat");
                        // Продолжаем с новой игрой
                        int length, width;
                        std::cout << "Enter board size (length width): ";
                        std::cin >> length >> width;

                        if (length < 10 || width < 10 || length > 25 || width > 25) {
                            std::cout << "Invalid size! Using default 15x15." << std::endl;
                            length = 15;
                            width = 15;
                        }

                        game.startNewGame(length, width);
                    }
                }
            } else {
                // Новая игра без автосохранения
                int length, width;
                std::cout << "Enter board size (length width): ";
                std::cin >> length >> width;

                if (length < 10 || width < 10 || length > 25 || width > 25) {
                    std::cout << "Invalid size! Using default 15x15." << std::endl;
                    length = 15;
                    width = 15;
                }

                game.startNewGame(length, width);
            }
        }
        else if (choice == 2) {
            // Загрузка игры - сначала пробуем автосохранение, потом обычные слоты
            try {
                if (fs::exists("autosave.dat")) {
                    game.loadGame("autosave.dat");
                    std::cout << "Autosave loaded successfully!" << std::endl;
                } else {
                    // Загрузка из обычного слота (только слот 1)
                    try {
                        if (!fs::exists("savegame_1.dat")) {
                            throw std::runtime_error("Save file not found");
                        }

                        std::ifstream testFile("savegame_1.dat", std::ios::binary | std::ios::ate);
                        if (!testFile.is_open()) {
                            throw std::runtime_error("Cannot open save file");
                        }

                        std::streamsize size = testFile.tellg();
                        testFile.close();

                        if (size < 100) {
                            std::cout << "Save file appears corrupted. Deleting and starting new game..." << std::endl;
                            fs::remove("savegame_1.dat");
                            throw std::runtime_error("Corrupted save file deleted");
                        }

                        game.loadGame("savegame_1.dat");
                        std::cout << "Game loaded successfully from slot 1!" << std::endl;

                    } catch (const std::exception& e) {
                        std::cerr << "Load from slot failed: " << e.what() << std::endl;

                        if (fs::exists("savegame_1.dat")) {
                            try {
                                fs::remove("savegame_1.dat");
                                std::cout << "Corrupted save file deleted." << std::endl;
                            } catch (...) {
                                std::cout << "Could not delete corrupted save file." << std::endl;
                            }
                        }

                        throw; // Перебрасываем исключение дальше
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Load failed: " << e.what() << std::endl;
                std::cout << "Starting new game instead..." << std::endl;

                // Начинаем новую игру
                int length, width;
                std::cout << "Enter board size (length width): ";
                std::cin >> length >> width;

                if (length < 10 || width < 10 || length > 25 || width > 25) {
                    std::cout << "Invalid size! Using default 15x15." << std::endl;
                    length = 15;
                    width = 15;
                }

                game.startNewGame(length, width);
            }
        }

        game.run();

        std::cout << "Thanks for playing!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        std::cerr << "Press Enter to exit..." << std::endl;
        std::cin.get();
        return -1;
    }
}