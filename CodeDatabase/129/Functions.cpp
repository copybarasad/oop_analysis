#include "Functions.h"
void HelpDisplay::display_choose_spell(){
    move(MY_WINDOW::HEIGHT + 4, 2*MY_WINDOW::WIDTH);
    printw("CHOOSE SPELL     ");
}

void HelpDisplay::display_spell_was_choosed(char c, Manager manager){
    bool is_there_spell = manager.get_hand_can_use_spell(c);

    move(MY_WINDOW::HEIGHT + 4, 2*MY_WINDOW::WIDTH);
    if(is_there_spell){
        switch(c){
            case 'd':
                printw("SPELL Direct Damage ACTIVATED");
                break;
            case 'a':
                printw("SPELL Area Damage ACTIVATED");
                break;
            case 't':
                printw("SPELL Trap ACTIVATED");
                break;
        }
    } else {
        printw("THERE IS NOT THAT SPELL");
        move(MY_WINDOW::HEIGHT + 5, 2*MY_WINDOW::WIDTH +3);
        printw("TYPE ANY KEY TO CONTINUE");
    }
    
}

void HelpDisplay::display_spell_was_used(bool was){
    if(was){
       move(MY_WINDOW::HEIGHT + 4, 2*MY_WINDOW::WIDTH);
       printw("SPELL WAS USED                       ");
    } else {
       move(MY_WINDOW::HEIGHT + 4, 2*MY_WINDOW::WIDTH);
       printw("SPELL WAS'NT USED                    ");
    }
}


void HelpDisplay::display_save_game(const char* ans){
    move(MY_WINDOW::HEIGHT + 15, 0); clrtoeol();
    printw(ans);
}

void HelpDisplay::display_load_game(const char* ans){
    move(MY_WINDOW::HEIGHT + 15, 0); clrtoeol();
    printw(ans);
}

void HelpDisplay::print_win(){
    std::vector<std::string> win = {
        "__        ___ _   _ ",
        "\\ \\      / (_) \\ | |",
        " \\ \\ /\\ / /| |  \\| |",
        "  \\ V  V / | | |\\  |",
        "   \\_/\\_/  |_|_| \\_|"
    };
    
    int start_y = (LINES - win.size()) / 2;
    
    for (size_t i = 0; i < win.size(); ++i) {
        int start_x = (COLS - win[i].length()) / 2;
        mvprintw(start_y + i, start_x, "%s", win[i].c_str());
    }

    std::string exit_text = "type any key to exit";
    int text_y = start_y + win.size() + 1;
    int text_x = (COLS - exit_text.length()) / 2;
    mvprintw(text_y, text_x, "%s", exit_text.c_str());
}

void HelpDisplay::print_big_game_over() {
    std::vector<std::string> game_over = {
        "   ____    _    __  __ _____      ___  _   _ _____ ____  ",
        "  / ___|  / \\  |  \\/  | ____|    / _ \\| | | | ____|  _ \\ ",
        " | |  _  / _ \\ | |\\/| |  _|     | | | | | | |  _| | |_) |",
        " | |_| |/ ___ \\| |  | | |___    | |_| | |_| | |___|  _ < ",
        "  \\____/_/   \\_\\_|  |_|_____|    \\___/ \\___/|_____|_| \\_\\"
    };
    
    int start_y = (LINES - game_over.size()) / 2;
    
    for (size_t i = 0; i < game_over.size(); ++i) {
        int start_x = (COLS - game_over[i].length()) / 2;
        mvprintw(start_y + i, start_x, "%s", game_over[i].c_str());
    }

    std::string exit_text = "tap any key to exit";
    int text_y = start_y + game_over.size() + 1; // +1 для отступа
    int text_x = (COLS - exit_text.length()) / 2;
    mvprintw(text_y, text_x, "%s", exit_text.c_str());
}





