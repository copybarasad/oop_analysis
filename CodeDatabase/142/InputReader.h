#pragma once

#include <iostream>
#include <cctype>
#include <unordered_map>
#include "Command.h"
#include "Game.h"

class ConsoleCommandReader {
public:
    Command readCommand(const Game& game) const {
        Command cmd;

        const auto controls = game.getControlsForTest();
        auto showKey = [](char key) {
            return static_cast<char>(std::toupper(static_cast<unsigned char>(key)));
        };

        std::cout << "Команды: "
                  << showKey(controls.at("MOVE_UP")) << "/" << showKey(controls.at("MOVE_LEFT")) << "/"
                  << showKey(controls.at("MOVE_DOWN")) << "/" << showKey(controls.at("MOVE_RIGHT"))
                  << " - движение, " << showKey(controls.at("SWITCH_MODE")) << " - переключить режим, "
                  << showKey(controls.at("CAST_SPELL")) << " - заклинание, "
                  << showKey(controls.at("SAVE_GAME")) << " - сохранить, "
                  << showKey(controls.at("QUIT")) << " - выход\n";
        std::cout << "Ваш ход: ";

        char inputChar;
        std::cin >> inputChar;
        std::cin.ignore();

        auto matches = [&](const std::string& keyName) {
            return std::tolower(static_cast<unsigned char>(inputChar)) ==
                   std::tolower(static_cast<unsigned char>(controls.at(keyName)));
        };

        if (matches("MOVE_UP")) {
            cmd.type = CommandType::Move;
            cmd.dy = -1;
        } else if (matches("MOVE_DOWN")) {
            cmd.type = CommandType::Move;
            cmd.dy = 1;
        } else if (matches("MOVE_LEFT")) {
            cmd.type = CommandType::Move;
            cmd.dx = -1;
        } else if (matches("MOVE_RIGHT")) {
            cmd.type = CommandType::Move;
            cmd.dx = 1;
        } else if (matches("SWITCH_MODE")) {
            cmd.type = CommandType::SwitchMode;
        } else if (matches("CAST_SPELL")) {
            cmd.type = CommandType::CastSpell;
            std::cout << "Введите индекс заклинания: ";
            std::cin >> cmd.spellIndex;
            std::cout << "Введите координату X цели: ";
            std::cin >> cmd.targetX;
            std::cout << "Введите координату Y цели: ";
            std::cin >> cmd.targetY;
            std::cin.ignore();
        } else if (matches("SAVE_GAME")) {
            cmd.type = CommandType::SaveGame;
        } else if (matches("QUIT")) {
            cmd.type = CommandType::QuitToMenu;
        } else {
            cmd.type = CommandType::Invalid;
        }

        return cmd;
    }
};

