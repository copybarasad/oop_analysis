#include "LoadStrategy.h"
#include "../Game.h"
#include "InputController.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/PrintMessageEvent.h"
#include <iostream>
#include <sstream>

LoadStrategy::LoadStrategy(Game* game, InputController* inputController)
    : game(game), inputController(inputController) {}

bool LoadStrategy::handle() {
    if (game && inputController) {
        try {
            std::string filename = inputController->getInput("Введите имя файла для загрузки (без расширения): ");
            game->loadGameFromFile(filename);
            return false;  // Игра должна быть перезапущена после загрузки
        } catch (const std::exception& e) {
            std::ostringstream errorMsg;
            errorMsg << "Ошибка при загрузке игры: " << e.what();
            PrintMessageEvent errorEvent(errorMsg.str());
            EventNotifier::getInstance().notify(errorEvent);
        }
    }
    return true;
}

