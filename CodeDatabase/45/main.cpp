#include <thread>
#include <chrono>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <memory>

#include "game_controller.hpp"
#include "save_loader.hpp"

#include "player.hpp"
#include "hand.hpp"
#include "map.hpp"

#include "command.hpp"
#include "keyboard_input.hpp"
#include "visual_terminal.hpp"
#include "input_handler.hpp"
#include "game_drawer.hpp"

void wait_ms(int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main() {
    auto ki = std::make_unique<KeyboardInput>();
    auto vt = std::make_unique<VisualTerminal>();
    auto gd = std::make_unique<GameDrawer>(std::move(vt));
    GameController* gc = new GameController(std::move(gd));
    InputHandler handler(std::move(ki), gc);

    handler.start_game();

    return 0;
}
/*
    setNonCanonicalMode();
    std::atexit(restoreTerminalMode);
    std::cout << "Management: wasd. To switch combat types, press M (melee) or R(ranged).  In ranged mode, you cannot move, the arrow flies in the specified direction to the first obstacle.\n";
    
    GameController gc;
    gc.start();
    while (gc.game_active()) {
        gc.game_cycle();
        gc.end_cycle();
    }
    
    return 0;
}*/