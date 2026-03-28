#pragma once
#include "command.h"
#include "Game.h"
#include <string>
#include <iostream>

class SaveGameCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        try {
            game.saveGame("savefile.txt"); // Теперь этот метод публичный и доступен!
            resultMessage = "Игра сохранена!";
            return true;
        } catch (const std::exception& e) {
            resultMessage = "Ошибка сохранения: " + std::string(e.what());
            return false;
        }
    }
    
    std::string getType() const override { return "SAVE"; }
};

class LoadGameCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        try {
            game.loadGame("savefile.txt"); // И этот тоже!
            resultMessage = "Игра загружена!";
            return true;
        } catch (const std::exception& e) {
            resultMessage = "Ошибка загрузки: " + std::string(e.what());
            return false;
        }
    }
    
    std::string getType() const override { return "LOAD"; }
};

class ShowStatusCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        // Просто формируем строку статуса
        std::string s = "=== СТАТУС ===\n";
        s += "Уровень: " + std::to_string(game.getCurrentLevel()) + "\n";
        s += "Игрок HP: " + std::to_string(game.getPlayer()->getLives());
        resultMessage = s;
        return true;
    }
    std::string getType() const override { return "SHOW_STATUS"; }
};

class QuitCommand : public Command {
public:
    bool execute(Game& game, std::string& resultMessage) override {
        resultMessage = "Выход...";
        return false; // Возвращаем false, чтобы остановить цикл в контроллере
    }
    std::string getType() const override { return "QUIT"; }
};