#include "play_process_state.hpp"
#include "game_controller.hpp"
#include "event_dispatcher.hpp"
#include "events.hpp"
#include "entity_snapshot_factory.hpp"
#include "consolerenderer.hpp"

void PlayProcessState::OnEnter() {
    EventDispatcher::instance().notify(
        UiMessageEvent("===== GAME STARTED:")
    );
    EventDispatcher::instance().notify(
        UiMessageEvent("Enter command (w/a/s/d - move, t - toggle, z - spell, p - save, l - load, q - quit):")
    );
}

void PlayProcessState::Update(IController& controller) {
    Game& game = controller.getGame();
    EntitySnapshotFactory factory;
    if (!game.getPlayer().isAlive()) {
        auto snap = factory.MakePlayerSnapshot(game);
        EventDispatcher::instance().notify(DeadEvent(snap));
        EventDispatcher::instance().notify(UiMessageEvent("💀 The Mad Scientist has perished!"));
        controller.SetState(GameStateType::Menu);
        return;
    }

    if (game.AllEnemiesDefeated()) {
        EventDispatcher::instance().notify(WinEvent(game.getLevel()));
        EventDispatcher::instance().notify(UiMessageEvent("🏆 All aliens defeated!"));
        controller.SetState(GameStateType::LevelComplete);
        return;
    }
}