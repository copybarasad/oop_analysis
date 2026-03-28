#include <iostream> 
#include "level_complete_state.hpp"
#include "game_controller.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"

void LevelCompleteState::OnEnter() {
    std::string msg =
        "\n===== LEVEL COMPLETE! =====\n"
        "Choose action:\n"
        "0 — Main Menu\n"
        "1 — Upgrade character\n"
        "2 — Next Level\n"
        "3 — Load game\n"
        "4 — Save game\n"
        "> ";
    EventDispatcher::instance().notify(UiMessageEvent(msg));
}

void LevelCompleteState::Update(IController& controller) {
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        EventDispatcher::instance().notify(UiMessageEvent("Invalid input. Try again."));
        OnEnter();   
        return;
    }

    switch (choice) {
        case 0:
            controller.SetState(GameStateType::Menu);
            return;

        case 1:
            controller.SetState(GameStateType::Upgrade);
            return;

        case 2:
            controller.SetState(GameStateType::LevelTransition);
            return;

        case 3: {
            EventDispatcher::instance().notify(
                UiMessageEvent("Enter file name to load:")
            );
            std::string filename;
            std::cin >> filename;
            bool ok = controller.LoadGame(filename);
            if (!ok) {
                EventDispatcher::instance().notify(
                    UiMessageEvent("Failed to load game.")
                );
                return;
            }
            if (controller.getGame().AllEnemiesDefeated()) {
                controller.SetState(GameStateType::LevelComplete);
            } else {
                controller.SetState(GameStateType::PlayProcess);
            }
            return;
        }

        case 4: {
            EventDispatcher::instance().notify(
                UiMessageEvent("Enter file name to save:")
            );
            std::string filename;
            std::cin >> filename;
            controller.SaveGame(filename);
            return;
        }

        default:
            EventDispatcher::instance().notify(UiMessageEvent("Invalid option."));
            OnEnter();
            return;
    }
}
