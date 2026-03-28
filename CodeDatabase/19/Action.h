#pragma once
#include <nlohmann/json.hpp>

enum class Action {
    SwitchWeapon,
    FocusSpells,
    NextSpell,
    PreviousSpell,
    CursorAction,
};


NLOHMANN_JSON_SERIALIZE_ENUM(Action, {
                             {Action::SwitchWeapon, "SwitchWeapon"},
                             {Action::FocusSpells, "FocusSpells"},
                             {Action::NextSpell, "NextSpell"},
                             {Action::PreviousSpell, "PreviousSpell"},
                             {Action::CursorAction, "CursorAction"}
                             });
