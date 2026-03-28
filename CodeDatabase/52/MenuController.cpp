#include "MenuController.h"
#include <iostream>
#include <limits>


static void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int MenuController::ShowMainMenu() {
    std::cout << "\n=== Главное меню ===\n";
    std::cout << "1. Новая игра\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Выйти\n";
    std::cout << "> ";

    int c;

    if (!(std::cin >> c)) {
        ClearInput();
        std::cout << "Неверный ввод.\n";
        return -1; 
    }

    return c;
}

int MenuController::ShowGameOverMenu() {
    std::cout << "\n=== Игра окончена ===\n";
    std::cout << "1. Начать заново\n";
    std::cout << "2. Выйти в меню\n";
    std::cout << "> ";

    int c;

    if (!(std::cin >> c)) {
        ClearInput();
        std::cout << "Неверный ввод.\n";
        return -1;
    }

    return c;
}