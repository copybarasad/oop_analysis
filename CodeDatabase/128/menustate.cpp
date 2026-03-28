#include "menustate.hpp"
#include "game_controller.hpp"
#include "events.hpp"
#include "event_dispatcher.hpp"
#include "loadgamecommand.hpp"
#include <iostream>

void MenuState::OnEnter() {
    EventDispatcher::instance().notify(
        UiMessageEvent("===== MAIN MENU:")
    );
    std::string msg =
        "0 - Quit\n"
        "1 - New Game\n"
        "2 - Load Game\n"
        "> ";
    EventDispatcher::instance().notify(UiMessageEvent(msg));
}

void MenuState::Update(IController& controller) {
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        EventDispatcher::instance().notify(UiMessageEvent("Invalid input. Try again."));
        return;
    }

    switch (choice) {
        case 0:
            controller.Stop();
            break;

        case 1:
            controller.StartNewGame();
            controller.SetState(GameStateType::PlayProcess);
            break;

        case 2: {
            EventDispatcher::instance().notify(UiMessageEvent("Enter save file name: "));
            std::string filename;
            std::cin >> filename;
            LoadGameCommand cmd(filename);
            if (!cmd.Execute(controller.getGame())) {
                EventDispatcher::instance().notify(UiMessageEvent("Bad save file. Returning to menu."));
                return;
            }

            if (controller.getGame().AllEnemiesDefeated()) {
                controller.SetState(GameStateType::LevelComplete);
            } else {
                controller.SetState(GameStateType::PlayProcess);
            }
            break;
        }

        default:
            EventDispatcher::instance().notify(UiMessageEvent("Invalid input. Try again."));
            break;
    }
}
