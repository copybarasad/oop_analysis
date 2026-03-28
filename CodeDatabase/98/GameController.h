#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "CommandManager.h"
#include "GameView.h"
#include <iostream>

template <typename InputReader, typename Renderer>
class GameController {
private:
    InputReader input;
    GameView<Renderer> view;
    
public:
    GameController() = default;

    template<typename GameT>
    void Run(GameT& game) {
        bool exitRequested = false;
        game.SetLevelActive(true);

        while (!exitRequested && game.PlayerIsAlive() && !game.IsLevelCompleted()) {
            
            // текущее сост 
            view.DrawGame(game);
            
            // последнее действие
            view.DrawMessage(game.GetLastAction());
            game.ClearLastAction();
            
            view.DrawMenu(game);

            // команда 
            Command cmd = input.GetCommand();
            bool skipEnemyTurn = false;

            switch (cmd.type) {
                case CommandType::MoveUp:
                    game.MovePlayerDirection(0, -1);
                    break;
                case CommandType::MoveDown:
                    game.MovePlayerDirection(0, 1);
                    break;
                case CommandType::MoveLeft:
                    game.MovePlayerDirection(-1, 0);
                    break;
                case CommandType::MoveRight:
                    game.MovePlayerDirection(1, 0);
                    break;
                case CommandType::Attack:
                    {
                        AttackResult result = game.PerformAttackNearest();
                        view.DrawAttackResult(result);
                    }
                    break;
                case CommandType::ShowStats:
                    view.DrawStats(game);
                    skipEnemyTurn = true;
                    break;
                case CommandType::UseSpell:
                    if (cmd.param >= 1) {
                        if (game.HasSpellRequiringCoords(cmd.param)) {
                            // запрос коордов лдя атаки той 
                            auto coords = input.ReadCoordinates();
                            if (coords.first != -1 && coords.second != -1) {
                                SpellResult result = game.UseSpellByIndexWithCoords(cmd.param, coords.first,coords.second);
                                view.DrawSpellResult(result);
                            } else {
                                view.DrawMessage("Ошибка ввода координат");
                            }
                        } else {
                            SpellResult result = game.UseSpellByIndex(cmd.param);
                            view.DrawSpellResult(result);
                        }
                    }
                    break;
                case CommandType::SearchEnemies:
                    {
                        auto enemies = game.SearchForEnemies();
                        view.DrawSearchResults(game, enemies);
                    }
                    skipEnemyTurn = true;
                    break;
                case CommandType::SwitchAttackType:
                    game.SwitchPlayerAttackType();
                    view.DrawMessage("Тип атаки изменен");
                    break;
                case CommandType::PrintField:
                    view.DrawGame(game);
                    skipEnemyTurn = true;
                    break;
                case CommandType::Save:
                    {
                        bool saved = game.SaveGameToFile("savegame.txt");
                        if (saved) {
                            view.DrawSaveLoadMessage("Игра сохранена в savegame.txt");
                        } else {
                            view.DrawSaveLoadMessage("Ошибка сохранения игры");
                        }
                    }
                    skipEnemyTurn = true;
                    break;
                case CommandType::Load:
                    {
                        bool loaded = game.LoadGameFromFile("savegame.txt");
                        if (loaded) {
                            view.DrawSaveLoadMessage("Игра загружена из savegame.txt");
                        } else {
                            view.DrawSaveLoadMessage("Ошибка загрузки игры");
                        }
                    }
                    skipEnemyTurn = true;
                    break;
                case CommandType::Quit:
                    exitRequested = true;
                    game.SetLevelActive(false);
                    view.DrawMessage("Выход из игры...");
                    skipEnemyTurn = true;
                    break;
                case CommandType::None:
                default:
                    view.DrawMessage("Неверная команда! Попробуй снова.");
                    skipEnemyTurn = true;
                    break;
            }

            // Ход врагов 
            if (!skipEnemyTurn && !exitRequested && game.PlayerIsAlive() && !game.IsLevelCompleted()) {
                auto enemyActions = game.EnemyTurn();
                view.DrawEnemyAttacks(enemyActions);
            }
        }

        // окончаниек  игры
        if (!game.PlayerIsAlive()) {
            view.DrawGameOver(false);
        } else if (game.IsLevelCompleted()) {
            view.DrawGameOver(true);
            bool upgradeShown = game.NextLevel();
            if (upgradeShown) {
                view.DrawUpgradeMenu();
                int choice;
                std::cin >> choice;
                game.ApplyUpgrade(choice);
            }
        }
    }
};

#endif