#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "input_reader.h"
#include "command.h"
#include <memory>

class Game;

template<typename InputReaderType>
class GameController {
private:
    Game& game;
    std::unique_ptr<InputReaderType> inputReader;

public:
    explicit GameController(Game& g, const std::string& configFile = "controls.cfg")
        : game(g), inputReader(std::make_unique<InputReaderType>(configFile)) {}

    virtual ~GameController() = default;

    // Получить следующую команду от считывателя ввода
    std::unique_ptr<Command> getNextCommand() {
        GameCommand gameCmd = inputReader->getNextGameCommand();
        return convertGameCommandToCommand(gameCmd);
    }

    // Выполнить команду
    void executeCommand(const std::unique_ptr<Command>& command) {
        if (command) {
            command->execute(game);
        }
    }

    // Получить целевые координаты для заклинания
    std::pair<int, int> getSpellTarget() {
        return inputReader->getSpellTarget();
    }

    // Получить ответ да/нет
    bool getYesNoChoice(const std::string& prompt) {
        return inputReader->getYesNoChoice(prompt);
    }

    // Получить имя файла
    std::string getFilename(const std::string& prompt) {
        return inputReader->getFilename(prompt);
    }

    Game& getGame() { return game; }
    const Game& getGame() const { return game; }

    InputReaderType& getInputReader() { return *inputReader; }

private:
    // Преобразовать GameCommand в Command объект
    std::unique_ptr<Command> convertGameCommandToCommand(const GameCommand& cmd);
};

#endif