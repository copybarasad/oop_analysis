#include "InputReader.h"

SfmlInputReader::SfmlInputReader(sf::Window& window, KeyBindings& bindings)
    : window_(window),
      bindings_(bindings),
      has_pending_command_(false) {}

Command SfmlInputReader::ReadInput() {
    if (has_pending_command_) {
        Command cmd = pending_command_;
        has_pending_command_ = false;
        pending_command_ = Command();
        return cmd;
    }
    return Command();
}

bool SfmlInputReader::HasInput() const {
    return has_pending_command_;
}

void SfmlInputReader::SetSpellTarget(int spell_index, int target_x, int target_y) {
    pending_command_ = Command(PlayerAction::CAST_SPELL, spell_index, target_x, target_y);
    has_pending_command_ = true;
}

void SfmlInputReader::ProcessEvent(const sf::Event& event) {
    if (const auto* key_pressed = event.getIf<sf::Event::KeyPressed>()) {
        PlayerAction action = bindings_.GetAction(key_pressed->code);
        if (action != PlayerAction::NONE) {
            pending_command_ = Command(action);
            has_pending_command_ = true;
        }
    }
}

void SfmlInputReader::ClearPendingCommand() {
    has_pending_command_ = false;
    pending_command_ = Command();
}
