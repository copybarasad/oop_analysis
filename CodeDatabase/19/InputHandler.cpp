#include "InputHandler.h"

#include "core/input/impl/SDLGameInput.h"
#include "core/Game.h"


template<typename InputImpl>
std::map<Action, std::string> InputHandler<InputImpl>::load() const {
    std::map<Action, std::string> loaded_bindings;

    if (!std::filesystem::exists(file_)) {
        std::cout << "Input config " << file_ << " not found" << std::endl;
        return {};
    }

    std::ifstream i(file_);
    if (!i) {
        std::cerr << "Failed to read input config: " << file_ << std::endl;
        return {};
    }

    json j;
    try {
        i >> j;
    } catch (const std::exception &e) {
        std::cerr << "JSON Error in " << file_ << ": " << e.what() << std::endl;
        return {};
    }

    for (auto &[key, value]: j.items()) {
        try {
            Action action = json(key).get<Action>();

            json check_json = action;
            if (check_json.get<std::string>() == key) {
                loaded_bindings[action] = value.get<std::string>();
            } else {
                std::cerr << "Warning: Unknown action key ignored: " << key << std::endl;
            }
        } catch (const std::exception &e) {
            std::cerr << "Error parsing binding for " << key << ": " << e.what() << std::endl;
        }
    }

    return loaded_bindings;
}

template<typename InputImpl>
bool InputHandler<InputImpl>::init() {
    std::map<Action, std::string> defaults = {
        {Action::SwitchWeapon, "space"},
        {Action::CursorAction, "mouse_left"},
        {Action::FocusSpells, "mouse_right"},
        {Action::NextSpell, "mouse_wheel_down"},
        {Action::PreviousSpell, "mouse_wheel_up"}
    };

    std::map<Action, std::string> custom_bindings = load();

    auto action_handler = [&](Action action) {
        if (game_.state() != GameState::PlayerMove) return;
        switch (action) {
            case Action::CursorAction: {
                int x, y;
                get_mouse_pos(x, y);
                game_.logic().handle_player_click(x, y);
                break;
            }
            case Action::SwitchWeapon:
                game_.logic().switch_attack_mode();
                break;
            case Action::NextSpell:
                game_.logic().handle_next_spell();
                break;
            case Action::PreviousSpell:
                game_.logic().handle_prev_spell();
                break;
            case Action::FocusSpells:
                game_.logic().toggle_spell_focus();
                break;
            default:
                break;
        }
    };

    auto stop_handler = [&]() {
        game_.stop();
    };

    auto click_handler = [&]() {
        int x, y;
        get_mouse_pos(x, y);
        game_.hud().handle_click(x, y);
    };

    if (custom_bindings.size() < defaults.size()) custom_bindings = {};

    return input_.init(defaults, custom_bindings, action_handler, click_handler, stop_handler);
}

template class InputHandler<SDLGameInput>;
