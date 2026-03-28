#include "upgrade_character_state.hpp"
#include "game_controller.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"

#include <iostream> 

using namespace configs;

void UpgradeCharacterState::OnEnter() 
{
    EventDispatcher::instance().notify(UiMessageEvent("\n===== CHARACTER UPGRADE:"));
    std::string msg =
        "\nChoose upgrade:\n"
        "0 - Return to Main Menu\n"
        "1 - Increase Max Health (Cost = 30 score)\n"
        "2 - Increase Damage (Cost = 20 score)\n"
        "3 - Load Game\n"
        "4 - Save Game\n"
        "5 - Continue to Next Level\n"
        "> ";
    EventDispatcher::instance().notify(UiMessageEvent(msg));
}

void UpgradeCharacterState::Update(IController& controller) 
{
    Game& game = controller.getGame();
    Player& player = game.getPlayer();
    EventDispatcher::instance().notify(UiMessageEvent("Player has " + std::to_string(player.getScore()) + " score"));
    int choice;
    if (!(std::cin >> choice)) {
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        EventDispatcher::instance().notify(
            UiMessageEvent("Invalid input. Try again.")
        );
        OnEnter();  
        return;
    }

    switch (choice) 
    {
        case 0:
            controller.SetState(GameStateType::Menu);
            return;
        case 1:
            if (!player.SpendScore(kCostMaxHealthPlayer)) {
                EventDispatcher::instance().notify(UiMessageEvent("❌ Not enough score points!"));
                return;
            }
            player.addMaxHealth(kOnceUpgradeHealth);
            EventDispatcher::instance().notify(UiMessageEvent("❤️  Max Health increased by " + std::to_string(kOnceUpgradeHealth)));
            return;
        case 2:
            if (!player.SpendScore(kCostDamagePlayer)) {
                EventDispatcher::instance().notify(UiMessageEvent("❌ Not enough score points!"));
                return;
            }
            player.addDamage(kOnceUpgradeDamage);
            EventDispatcher::instance().notify(UiMessageEvent("⚔️  Damage increased by " + std::to_string(kOnceUpgradeDamage)));
            return;
        case 3: 
        {
            EventDispatcher::instance().notify(UiMessageEvent("Enter save file name:"));
            std::string filename;
            std::cin >> filename;
            if (!controller.LoadGame(filename)) {
                EventDispatcher::instance().notify(UiMessageEvent("❌ Failed to load game."));
                return;
            }
            if (controller.getGame().AllEnemiesDefeated()) {
                controller.SetState(GameStateType::LevelComplete);
            } else {
                controller.SetState(GameStateType::PlayProcess);
            }
            return;
        }
        case 4:
        {
            EventDispatcher::instance().notify(UiMessageEvent("Enter file name to save:"));
            std::string filename;
            std::cin >> filename;
            controller.SaveGame(filename);
            return;
        }
        case 5:
            controller.SetState(GameStateType::LevelTransition);
            return;

        default:
            EventDispatcher::instance().notify(
                UiMessageEvent("Invalid option.")
            );
            return;
    }
    OnEnter();
}
