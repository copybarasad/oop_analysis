#include "SaveStrategy.h"
#include "../Game.h"
#include "InputController.h"
#include "../../models/events/EventNotifier.h"
#include "../../models/events/PrintMessageEvent.h"
#include <sstream>

SaveStrategy::SaveStrategy(Game* game, InputController* inputController)
    : game(game), inputController(inputController) {}

bool SaveStrategy::handle() {
    if (game && inputController) {
        try {
            std::string filename = inputController->getInput("Введите имя файла для сохранения (без расширения): ");
            if (filename.empty()) {
                filename = "save";
            }
            game->saveCurrentGame(filename);
        } catch (const std::exception& e) {
            std::ostringstream errorMsg;
            errorMsg << "Ошибка при сохранении игры: " << e.what();
            PrintMessageEvent errorEvent(errorMsg.str());
            EventNotifier::getInstance().notify(errorEvent);
        }
    }
    return true;
}

