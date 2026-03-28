#include "draw.h"
#include "field.h"
#include "game.h"
#include <iostream>

void Renderer::render_field(const Field& field) const {
    int width  = field.get_width();
    int height = field.get_height();

    cout << "\n  ";
    for (int x = 0; x < width; ++x) {
        cout << (x % 10) << " ";
    }
    cout << "\n";

    for (int y = 0; y < height; ++y) {
        cout << (y % 10) << " ";
        for (int x = 0; x < width; ++x) {
            cout << field.get_tile_symbol(x, y) << ' ';
        }
        cout << "\n";
    }

    if (auto player = field.get_player()) {
        cout << "\nHealth: " << player->get_health();
        cout << " | Score: " << player->get_score();
        cout << " | Mode: " << (player->get_mode() == Mode::CLOSE ? "Close" : "Far");
        cout << " | Damage: " << player->get_current_damage();
        if (player->is_currently_slowed()) {
            cout << " | SLOWED";
        }
        cout << " | Hand: ";

        const auto& spells = player->get_hand().get_spells();
        for (size_t i = 0; i < spells.size(); ++i) {
            cout << i << ":" << spells[i]->get_name();
            if (i + 1 < spells.size()) cout << "  ";
        }
        if (spells.empty()) cout << "(empty)";
        cout << "\n";
    }
}

void Renderer::render_help() const {
    cout << "\n=== GAME CONTROLS ===\n";
    cout << "w/a/s/d - Move up/left/down/right\n";
    cout << "m - Switch combat mode (melee/ranged)\n";
    cout << "f - Attack enemy at coordinates\n";
    cout << "u - Use spell from hand\n";
    cout << " 0: Direct\n";
    cout << " 1: Area \n";
    cout << " 2: Trap \n";
    cout << " 3: Summon \n";
    cout << " 4: Enhance \n";
    cout << "h - Show help\n";
    cout << "q - Quit game\n\n";
    cout << "Legend:\n";
    cout << "P - Player  E - Enemy  B - Building\n";
    cout << "# - Wall  ~ - Slowdown tile  . - Empty\n";
    cout << "T - Trap\n";
    cout << "X - Tower (ready to attack)\n";
    cout << "x - Tower (cooldown)\n";
    cout << "A - Ally\n";
    cout << "Hand: 0: Direct Damage  1: Area Damage ...\n";
    cout << "l/load - Load the game\n";
    cout << "save - Save the game\n";
    cout << "=====================\n";
}

void Renderer::render_game_over(const Field& field) const {
    cout << "\n===== GAME OVER =====\n";
    int score = 0;
    if (field.get_player()) {
        score = field.get_player()->get_score();
    }
    cout << "Final Score: " << score << "\n";
    cout << "=====================\n";
}

void Renderer::render_prompt(const std::string& text) const {
    cout << text;
}