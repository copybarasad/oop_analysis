#include "ConsoleRenderer.h"

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"   
#include "Game.h"
#include "Ally.h"

#include <iostream>


void ConsoleRenderer::RenderField(const GameField& field) {
    std::cout << "\n=== FIELD ===\n";

    for (int y = 0; y < field.Height(); ++y) {
        for (int x = 0; x < field.Width(); ++x) {
            char ch = '.';
            const Cell& c = field.CellAt(x, y);

            if (!c.isWalkable()) {
                ch = '#';
            }
            else if (c.hasEntity()) {
                auto ent = c.getEntity().get();
                if (dynamic_cast<const Player*>(ent)) ch = '@';
                else if (dynamic_cast<const Enemy*>(ent)) ch = 'E';
                else if (dynamic_cast<const Ally*>(ent)) ch = 'A';
            }
            else if (c.hasTrap()) {
                ch = 'T';
            }

            std::cout << ch;
        }
        std::cout << '\n';
    }

    std::cout << "=============\n";
}


static const char* CmdName(Command c) {
    switch (c) {
        case Command::MoveUp: return "Up";
        case Command::MoveDown: return "Down";
        case Command::MoveLeft: return "Left";
        case Command::MoveRight: return "Right";
        case Command::Attack: return "Attack";
        case Command::CastSpell: return "Spell";
        case Command::SwitchMode: return "Mode";
        case Command::Save: return "Save";
        case Command::Quit: return "Quit";
        default: return "";
    }
}

void ConsoleRenderer::RenderHUD(const Game& game) {
    const Player& p = game.GetPlayer();

    std::cout << "\nHP: " << p.Health()
              << " | Mode: "
              << (p.Mode() == AttackMode::Melee ? "MELEE" : "RANGED")
              << "\n";

    if (!bindings_) return;

    std::cout << "Controls: ";
    for (auto& [cmd, key] : bindings_->GetAllBindings()) {
        std::cout << "[" << CmdName(cmd) << ":" << key << "] ";
    }
    std::cout << "\n";
}


void ConsoleRenderer::RenderMessage(const char* msg) {
    std::cout << msg << "\n";
}
