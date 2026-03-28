#include "controls.hpp"

namespace Key {
    const char UP = 'w';
    const char DOWN = 's';
    const char LEFT = 'a';
    const char RIGHT = 'd';
    const char CHANGE_COMBAT = 'p';
    const char SHOOT = 'o';

    const char UNKNOWN = '?';

    std::set<char> AVAILABLE = { 
        UP, DOWN, LEFT, RIGHT, CHANGE_COMBAT, SHOOT
    };

    std::set<char> POSITIONAL = { 
        UP, DOWN, LEFT, RIGHT 
    };

    std::map<char, std::pair<int, int>> DESCRIPTION = {
        { UP, {-1, 0} },
        { DOWN, {1, 0} },
        { LEFT, {0, -1} },
        { RIGHT, {0, 1} },
    };
}

namespace Menu {
    const char NEW_GAME = 'n';
    
    const char SAVE_GAME = 'v';
    const char LOAD_GAME = 'l';

    std::set<char> AVAILABLE = { 
        NEW_GAME, SAVE_GAME, LOAD_GAME
    };

    const char HP_UP = '1';
    const char CP_UP = '2';
}

static struct termios stored_settings;

char Control::prev_command = Key::UNKNOWN;

void Control::set_keypress() {
    struct termios new_settings;

    tcgetattr(0, &stored_settings);

    new_settings = stored_settings;

    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;

    tcsetattr(0,TCSANOW,&new_settings);
}

char Control::get_move() {
    char key;
    if (Key::AVAILABLE.find(prev_command) != Key::AVAILABLE.end()) {
        key = prev_command;
        reset_prev_command();
        return key;
    }

    std::cin >> key;

    if (Key::AVAILABLE.find(key) != Key::AVAILABLE.end()) {
        prev_command = key;
        return key;
    } else {
        prev_command = Key::UNKNOWN;
        return Key::UNKNOWN;
    }
}

char Control::get_menu_action_on_start() {
    char key;

    while (true) {
        std::cout << "New game(n) | Load game(l): ";
        std::cin >> key;

        if (key == Menu::NEW_GAME || key == Menu::LOAD_GAME) {
            return key;
        }
    }
}

char Control::get_menu_action_in_game() {   
    char key;

    while (true) {
        std::cin >> key;

        if (Menu::AVAILABLE.find(key) != Menu::AVAILABLE.end()) {
            prev_command = key;
            return key;
        } else if (Key::AVAILABLE.find(key) != Key::AVAILABLE.end()) {
            prev_command = key;
            return key;
        }
    }
}

std::string Control::get_save_name() {
    std::string load_name;
    
    Control::reset_keypress();

    std::cout << "Enter `game name` to load: ";
    std::cin >> load_name;

    Control::set_keypress();

    return load_name;
}

char Control::get_ench() {
    char key;
    while (true) {
        std::cout << "Choose hp+(1) or cp+(2): ";
        std::cin >> key;

        if (key == Menu::HP_UP || key == Menu::CP_UP) {
            return key;
        }
    }
}

char Control::get_prev_command() {
    return prev_command;
}

void Control::reset_prev_command() {
    prev_command = Key::UNKNOWN;
}

void Control::reset_keypress() {
	tcsetattr(0,TCSANOW,&stored_settings);
	return;
}