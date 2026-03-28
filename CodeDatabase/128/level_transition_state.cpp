#include "level_transition_state.hpp"
#include "game_controller.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"

void LevelTransitionState::OnEnter() {
    EventDispatcher::instance().notify(
        UiMessageEvent("\n===== TRANSITION TO NEXT LEVEL:")
    );
}

void LevelTransitionState::Update(IController& controller) {
    Game& game = controller.getGame();
    game.IncreaseLevel();
    game.GenerateNextLevel(game.getLevel());
    game.getPlayer().getHand().RemoveHalfCards();
    EventDispatcher::instance().notify(UiMessageEvent("Level " + std::to_string(game.getLevel()) + " ready!"));
    controller.SetState(GameStateType::PlayProcess);
}
