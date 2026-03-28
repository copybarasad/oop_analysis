#include "Command.h"
#include "Game.h"
#include "EventSystem.h"
#include <iostream>

void MoveCommand::execute(Game& game) {
    game.movePlayer(dx_, dy_);
}

void SwitchAttackModeCommand::execute(Game& game) {
    game.switchAttackMode();
}

void UseSpellCommand::execute(Game& game) {
    std::cout << "Введите номер заклинания (начиная с 1): ";
    std::cin >> spellIndex_;
    std::cout << "Введите координаты атаки (x y): ";
    std::cin >> targetX_ >> targetY_;
    game.useSpell(spellIndex_ - 1, targetX_, targetY_); 
}

void EndTurnCommand::execute(Game& game) {
    game.endTurn();
}

void SaveCommand::execute(Game& game) {
    if (game.saveGame(filename_)) {
        std::cout << "Игра сохранена в " << filename_ << std::endl;
        std::cout << "Выход из игры..." << std::endl;
        exit(0);
    } else {
        std::cout << "Не удалось сохранить игру!" << std::endl;
    }
}

void LoadCommand::execute(Game& game) {
    try {
        if (game.loadGame(filename_)) {
            std::cout << "Игра загружена из " << filename_ << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Ошибка загрузки: " << e.what() << std::endl;
    }
}

void ExitCommand::execute(Game& game) {
    std::cout << "Выйти без сохранения? (y/n): ";
    char choice;
    std::cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        std::cout << "Выход из игры..." << std::endl;
        exit(0);
    }
}