#pragma once

#include "Command.h"
#include "EventBus.h"
#include "SaveSystem.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

template<typename InputReader, typename Visualizer>
class GameController {
public:
    GameController(InputReader &reader, Visualizer &visualizer)
        : inputReader(reader), view(visualizer), running(true), restartNeeded(false) {
    }

    template<typename GameType>
    void run(GameType &game) {
        while (running && game.isPlayerAlive() && game.isGameRunning()) {
            view.render(game);
            Command cmd = inputReader.readCommand();
            handleCommand(game, cmd);
        }
        if (!game.isPlayerAlive()) {
            view.render(game);
            view.promptInput("Вы проиграли. Начать заново? (y/N): ");
            std::string answer;
            if (std::getline(std::cin, answer)) {
                if (!answer.empty()) {
                    char c = static_cast<char>(std::tolower(static_cast<unsigned char>(answer.front())));
                    if (c == 'y') {
                        restartNeeded = true;
                    }
                }
            }
        }
    }

    bool isRestartRequested() const {
        return restartNeeded;
    }

private:
    InputReader &inputReader;
    Visualizer &view;
    bool running;
    bool restartNeeded;

    std::string selectSlot(const std::string &title) {
        // Выбор слота сохранения/загрузки
        auto slots = SaveSystem::listSaveSlots();
        std::cout << "\n" << title << ":\n";
        for (size_t i = 0; i < slots.size(); ++i) {
            std::cout << (i + 1) << ". " << slots[i] << "\n";
        }
        std::cout << "0. Отмена\n";
        std::cout << "Введите номер слота или имя нового: ";
        std::string input;
        std::getline(std::cin >> std::ws, input);
        if (input.empty()) return "";
        bool isNumber = !input.empty() && std::all_of(input.begin(), input.end(), [](char c) {
            return std::isdigit(static_cast<unsigned char>(c));
        });
        if (isNumber) {
            int choice = std::stoi(input);
            if (choice == 0) return "";
            if (choice >= 1 && choice <= static_cast<int>(slots.size())) {
                return slots[choice - 1];
            }
            return "";
        }
        return input;
    }

    template<typename GameType>
    void handleCommand(GameType &game, Command &cmd) {
        switch (cmd.type) {
            case CommandType::MOVE_UP: game.movePlayer(0, -1);
                break;
            case CommandType::MOVE_DOWN: game.movePlayer(0, 1);
                break;
            case CommandType::MOVE_LEFT: game.movePlayer(-1, 0);
                break;
            case CommandType::MOVE_RIGHT: game.movePlayer(1, 0);
                break;
            case CommandType::ATTACK: {
                view.promptInput(inputReader.directionPrompt());
                Direction dir = inputReader.readDirection();
                game.playerAttack(dir.dx, dir.dy);
                break;
            }
            case CommandType::SWITCH_WEAPON:
                game.switchWeapon();
                break;
            case CommandType::CAST_SPELL: {
                view.promptSpellSelection(game);
                int idx = inputReader.readIndex();
                if (idx < 0 || idx >= game.getPlayer().getHand().getSize()) {
                    view.showMessage("Неверный номер заклинания");
                    break;
                }
                Direction dir{0, 0};
                Spell *spell = game.getPlayer().getHand().getSpell(idx);
                if (spell && spell->requiresTarget()) {
                    view.promptInput(inputReader.directionPrompt());
                    dir = inputReader.readDirection();
                }
                game.castSpellAt(idx, dir.dx, dir.dy);
                break;
            }
            case CommandType::SAVE_GAME: {
                std::string slot = selectSlot("Сохранение");
                if (!slot.empty()) {
                    std::string path = SaveSystem::getSaveFilePath(slot);
                    SaveSystem::saveGame(game, path);
                    view.showMessage("Сохранено в слот \"" + slot + "\"");
                }
            }
            break;
            case CommandType::LOAD_GAME: {
                std::string slot = selectSlot("Загрузка");
                if (!slot.empty()) {
                    std::string path = SaveSystem::getSaveFilePath(slot);
                    try {
                        SaveSystem::loadGame(game, path);
                        view.showMessage("Загружен слот \"" + slot + "\"");
                    } catch (const std::exception &e) {
                        view.showMessage(std::string("Ошибка загрузки: ") + e.what());
                    }
                }
            }
            break;
            case CommandType::EXIT:
                running = false;
                game.stop();
                break;
            default:
                break;
        }
        game.afterTurn();
    }
};
