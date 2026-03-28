#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Command.h"

template<typename InputT>
class GameController {
public:
    GameController(GameModel &model) : model_(model), input_() {}

    bool processOnce() {
        Command cmd = input_.nextCommand();

        if (cmd.action == Action::Quit) return false;

        bool turnSpent = false; // Флаг: был ли потрачен ход

        if (cmd.action == Action::Move) {
            // movePlayerTo возвращает true, если ход успешен
            if (model_.movePlayerTo(cmd.x, cmd.y)) {
                turnSpent = true;
            }
        }
        else if (cmd.action == Action::Attack) {
            if (model_.attackAt(cmd.x, cmd.y)) {
                turnSpent = true;
            }
        }
        else if (cmd.action == Action::Use) {
            if (model_.useAt(cmd.idx, cmd.x, cmd.y)) {
                turnSpent = true;
            }
        }
        else if (cmd.action == Action::Save) {
            model_.saveToFile(cmd.arg);
            // При сохранении ход НЕ тратится
        }
        else if (cmd.action == Action::Load) {
            model_.loadFromFile(cmd.arg);
            // При загрузке ход НЕ тратится
        }
        else if (cmd.action == Action::Next) {
            model_.nextLevel();
            // Переход на уровень обновляет состояние, но технически это не "ход" в старом уровне
        }

        // Вызываем tick() только если игрок совершил действие, требующее времени
        if (turnSpent) {
            model_.stepTurn();
        }

        return true;
    }

private:
    GameModel &model_;
    InputT input_;
};

#endif // GAMECONTROLLER_H