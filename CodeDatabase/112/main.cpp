#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <csignal>

void signalHandler(int signal) {
    std::cout << "\nПолучен сигнал " << signal << ". Завершаем работу..." << std::endl;
    exit(0);
}

int main() {
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    std::cout << "=== ИГРА С ЗАКЛИНАНИЯМИ ===" << std::endl;
    std::cout << "Лабораторные работы 1-3" << std::endl;
    std::cout << "==========================" << std::endl;

    try {
        Game game;
        game.run();
    } catch (const std::exception& e) {
        std::cout << "Критическая ошибка: " << e.what() << std::endl;
        std::cout << "Нажмите Enter для выхода..." << std::endl;
        std::cin.get();
        return 1;
    } catch (...) {
        std::cout << "Неизвестная критическая ошибка!" << std::endl;
        std::cout << "Нажмите Enter для выхода..." << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}
