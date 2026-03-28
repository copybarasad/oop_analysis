#pragma once
#include "Game.h"
#include "GameCommand.h"
#include <iostream>

template <typename Input>
class GameController {
private:
    Input input;
    Game& game;

public:
    explicit GameController(Game& g) : game(g) {}

    void processTurn() {
        std::cout << "\nВаш ход: ";

        GameCommand cmd = input.readCommand();

        switch (cmd.type) {
            case CommandType::MoveUp:     game.movePlayer('W'); break;
            case CommandType::MoveDown:   game.movePlayer('S'); break;
            case CommandType::MoveLeft:   game.movePlayer('A'); break;
            case CommandType::MoveRight:  game.movePlayer('D'); break;

            case CommandType::AttackEnemy:  game.attackEnemies(); break;
            case CommandType::AttackTower:  game.attackTower(); break;
            case CommandType::SwitchMode:   game.switchPlayerMode(); break;

            case CommandType::UseSpell:
                if (cmd.argument > 0) {
                    game.useSpell(cmd.argument);
                } else {
                    std::cout << "Некорректный номер заклинания!\n";
                }
                break;

            case CommandType::Save:   game.saveGame(); break;
            case CommandType::Load:   game.loadGame(); break;
            case CommandType::Help:   game.showHelp(); break;
            case CommandType::Quit:   game.stop(); break;

            case CommandType::None:
                std::cout << "Неизвестная команда! Нажмите H для помощи.\n";
                break;

            default:
                break;
        }
    }
};