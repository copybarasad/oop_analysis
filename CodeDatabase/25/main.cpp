#include "Game.hpp"
#include <cctype>
#include <clocale>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <string>

// main.cpp: ‘'گ?‘طگَگّ گ?‘:گ?گ?گّ, گ?গ?গ?গ? গَগ?গ?গّগ?গ?, গْগّগُ‘?‘?গَ/‘?গç‘?‘'গّ‘?‘'/গْগّগ?‘?‘?গْগَगّ গٌگ?‘?‘<, ‘?গُ‘?গّগ?গَগّ.

using namespace std;

void show_help() {
    cout << R"(
=== Controls (enter a command and press Enter) ===
Move:   W A S D
Attack: I J K L  (I=up, J=left, K=down, L=right)
Cast:   1-9 [x y] (slot number; optional target coordinates)
Mode:   T        (switch melee/ranged)
Help:   H or R   (show this help)
Save:   SAVE <file>
Load:   LOAD <file>
New:    NEW      (restart level from scratch)
Quit:   Q

Legend: P -- player, E -- enemy, N -- nest, # -- wall, . -- empty, ~ -- slow
Win:    Destroy all enemies and nests.
Coords: X on top (tens row, ones row), Y on the left (two digits)

)";
}

string lower_token(string s) {
    for (char& ch : s) {
        ch = static_cast<char>(tolower(static_cast<unsigned char>(ch)));
    }
    return s;
}

Game create_game(const GameParams& params) {
    return Game(30, 15, params);
}

bool handle_save(const Game& game, const string& path) {
    try {
        game.save_to_file(path);
        cout << "Game saved to '" << path << "'.\n";
        return true;
    } catch (const SaveLoadError& e) {
        cout << "Save failed: " << e.what() << "\n";
    } catch (const exception& e) {
        cout << "Save failed: " << e.what() << "\n";
    }
    return false;
}

bool handle_load(Game& game, const GameParams& params, const string& path) {
    try {
        game = create_game(params);
        game.load_from_file(path);
        cout << "Save loaded from '" << path << "'.\n";
        return true;
    } catch (const SaveLoadError& e) {
        cout << "Load failed: " << e.what() << "\n";
    } catch (const exception& e) {
        cout << "Load failed: " << e.what() << "\n";
    }
    return false;
}

void prompt_start(Game& game, const GameParams& params) {
    cout << "Load save on startup (enter path or press Enter for new game): ";
    string line;
    if (!getline(cin, line)) {
        game = create_game(params);
        return;
    }
    if (line.empty()) {
        game = create_game(params);
        return;
    }
    if (!handle_load(game, params, line)) {
        cout << "Starting a new game instead.\n";
        game = create_game(params);
    }
}

int main() {
cout << "\n";
cout << "\n";

    cout << "~~Welcome!!!~~\n";
    cout << "to start the game, select:" << "\n";
    cout << "\n";
    cout << "-- if you want to start a SAVED GAME, simply enter the file name\n";
    cout << "\n";
    cout << "--if you want to start a NEW GAME, press 'Enter' \n";
    setlocale(LC_ALL, "Russian");
    ios::sync_with_stdio(false);

    cin.tie(nullptr);

    GameParams params;
    cout << "=== Game parameters (defaults) ===\n";
    cout << "Hero HP (>=0): "            << params.hero_hp         << "\n";
    cout << "Hero melee damage (>=0): "  << params.hero_melee_dmg  << "\n";
    cout << "Hero ranged damage (>=0): " << params.hero_ranged_dmg << "\n";
    cout << "Enemy HP (>=0): "           << params.enemy_hp        << "\n";
    cout << "Enemy damage (>=0): "       << params.enemy_atk       << "\n";
    cout << "Number of enemies (>=0): "  << params.enemy_count     << "\n";
    cout << "Number of enemy nests (>=0): " << params.nest_count   << "\n\n";

    show_help();

    Game game = create_game(params);
    prompt_start(game, params);

    string line;
    bool running = true;
    while (running) {
        game.print();
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        istringstream cmd(line);
        string token;
        cmd >> token;
        const string key = lower_token(token);

        if (key == "h") {
            show_help();
            cout.flush();
            std::fflush(stdout);
            continue;
        }
        if (key == "r") {
            show_help();
            cout.flush();
            std::fflush(stdout);
            continue;
        }
        if (key == "save") {
            string path;
            cmd >> path;
            if (path.empty()) {
                cout << "Please specify a filename to save.\n";
            } else {
                handle_save(game, path);
            }
            continue;
        }
        if (key == "load") {
            string path;
            cmd >> path;
            if (path.empty()) {
                cout << "Please specify a filename to load.\n";
            } else {
                handle_load(game, params, path);
            }
            continue;
        }
        if (key == "new" || key == "restart") {
            game = create_game(params);
            cout << "New game started.\n";
            continue;
        }
        if (key == "q" && token.size() == 1) {
            break;
        }

        bool alive = false;
        try {
            alive = game.step(line);
        } catch (const exception& e) {
            cout << "Error during turn: " << e.what() << "\n";
            continue;
        }

        if (!alive) {
            game.print();
            cout << "\n=== GAME OVER ===\n";
            cout << "Enter R to restart, L <file> to load, or Q to quit: ";
            if (!getline(cin, line)) break;
            istringstream retry(line);
            string retryCmd;
            retry >> retryCmd;
            string retryKey = lower_token(retryCmd);
            if (retryKey == "r" || retryKey == "restart") {
                game = create_game(params);
                continue;
            }
            if (retryKey == "l" || retryKey == "load") {
                string path;
                retry >> path;
                if (!path.empty() && handle_load(game, params, path)) {
                    continue;
                }
                cout << "No save loaded. Exiting.\n";
            }
            break;
        }

        if (game.level_finished()) {
            game.print();
            cout << "\n=== LEVEL CLEARED ===\n";
            cout << "Start a new game? (y/n): ";
            if (!getline(cin, line)) break;
            char ans = line.empty() ? 'n' : static_cast<char>(tolower(static_cast<unsigned char>(line[0])));
            if (ans == 'y' || ans == 'r') {
                game = create_game(params);
                continue;
            }
            break;
        }
    }
    return 0;
}
