#include "NC_Graphic.h"

void NC_Graphic::draw(GField gfield){
    gfield.draw(); 
    refresh();
}

void NC_Graphic::display_lifes(Manager manager){
    int lifes = manager.get_player_lifes();
    move(MY_WINDOW::HEIGHT + 4, 0);
    printw("L I F E S");

    move(MY_WINDOW::HEIGHT + 5, 0);
    clrtoeol();
    int i = 0;
    for(int j = 0; j < lifes; j++){
        mvaddch(MY_WINDOW::HEIGHT + 5, i, '<');
        mvaddch(MY_WINDOW::HEIGHT + 5, i + 1, '3');
        i += 3;
    }
    refresh();
}

void NC_Graphic::display_combat(Manager manager){
    bool close_combat = manager.get_player_is_close_combat();
    move(3, 3*MY_WINDOW::WIDTH + 5);
    clrtoeol();
    if(close_combat){
        printw("COMBAT: CLOSE");
        move(4, 3*MY_WINDOW::WIDTH + 5); clrtoeol();
        printw("DMG: 4");
    } else {
        printw("COMBAT: LONG-RANGE");
        move(4, 3*MY_WINDOW::WIDTH + 5); clrtoeol();
        printw("DMG: 2");
    }
}

void NC_Graphic::display_moves(Manager manager){
    move(1, 3*MY_WINDOW::WIDTH + 5);
    clrtoeol();
    printw("MOVE #%d", manager.get_moves());
}

void NC_Graphic::display_controls(){
    move(1, 5*MY_WINDOW::WIDTH);
    printw("C O N T R O L S");
    move(3, 5*MY_WINDOW::WIDTH); printw("%c: exit", CONTROL::QUIT);
    move(5, 5*MY_WINDOW::WIDTH); printw("%c: shift left", CONTROL::SHIFT_LEFT);
    move(6, 5*MY_WINDOW::WIDTH); printw("%c: shift down", CONTROL::SHIFT_DOWN);
    move(7, 5*MY_WINDOW::WIDTH); printw("%c: shift up", CONTROL::SHIFT_UP);
    move(8, 5*MY_WINDOW::WIDTH); printw("%c: shift right", CONTROL::SHIFT_RIGHT);
    move(10, 5*MY_WINDOW::WIDTH); printw("%c: hit left", CONTROL::HIT_LEFT);
    move(11, 5*MY_WINDOW::WIDTH); printw("%c: hit down", CONTROL::HIT_DOWN);
    move(12, 5*MY_WINDOW::WIDTH); printw("%c: hit up", CONTROL::HIT_UP);
    move(13, 5*MY_WINDOW::WIDTH); printw("%c: hit right", CONTROL::HIT_RIGHT);
    move(15, 5*MY_WINDOW::WIDTH); printw("%c: switch combat", CONTROL::COMBAT);
}


void NC_Graphic::display_spells_controls(){
    move(17, 5*MY_WINDOW::WIDTH); printw("%c: choose spell", CONTROL::CHOOSE_SPELL);
    move(18, 5*MY_WINDOW::WIDTH); printw("%c: activate Direct Damage spell", CONTROL::ACT_DD_SPELL);
    move(19, 5*MY_WINDOW::WIDTH); printw("%c: activate Area spell", CONTROL::ACT_AR_SPELL);
    move(20, 5*MY_WINDOW::WIDTH); printw("%c: activate Trap spell", CONTROL::ACT_TR_SPELL);

    move(22, 5*MY_WINDOW::WIDTH); printw("%c: SAVE GAME", CONTROL::SAVE_GAME);
    move(23, 5*MY_WINDOW::WIDTH); printw("%c: LOAD LAST SAVING", CONTROL::LOAD_GAME);

    move(6, 4*MY_WINDOW::WIDTH); printw("DD  : dmg=4; range=4");
    move(7, 4*MY_WINDOW::WIDTH); printw("Area: dmg=4; range=2x2");
    move(8, 4*MY_WINDOW::WIDTH); printw("Trap: dmg=4; range=0");

    

    move(25, 5*MY_WINDOW::WIDTH); printw("tap %c/%c/%c/%c after", CONTROL::DIR_SPELL_UP, CONTROL::DIR_SPELL_DOWN, CONTROL::DIR_SPELL_LEFT, CONTROL::DIR_SPELL_RIGHT);
    move(26, 5*MY_WINDOW::WIDTH); printw("spell activation");
    move(27, 5*MY_WINDOW::WIDTH); printw("to choose");
    move(28, 5*MY_WINDOW::WIDTH); printw("direction");

}

void NC_Graphic::display_my_spells(Manager manager){
    move(10, 3*MY_WINDOW::WIDTH + 5);
    printw("MY S P E L L S:");

    //!!
    for(int i = 11; i < 11 + manager.get_hand_max_count(); i++){
        move(i, 3*MY_WINDOW::WIDTH + 5); mvhline(getcury(stdscr), 3*MY_WINDOW::WIDTH + 5, ' ', 5*MY_WINDOW::WIDTH);
    }

    //!!
    std::vector<std::string> my_spells = manager.hand_get_my_spells();
    
    for(int i = 11; i < 11 + my_spells.size(); i++){
        move(i, 3*MY_WINDOW::WIDTH + 5);
        printw("%s", (my_spells[i - 11].c_str()));
    }
}


void NC_Graphic::display_enemy_tower(Manager manager){
    move(1, 4*MY_WINDOW::WIDTH); printw("I N F O R M A T I O N");
    move(3, 4*MY_WINDOW::WIDTH);
    printw("ENEMY TOWER");
    move(4, 4*MY_WINDOW::WIDTH);
    if(manager.get_enemy_tower_time_to_kill()){
        mvhline(getcury(stdscr), 4*MY_WINDOW::WIDTH, ' ', 5*MY_WINDOW::WIDTH);
        printw("IS READY TO KILL");
    } else {
        mvhline(getcury(stdscr), 4*MY_WINDOW::WIDTH, ' ', 5*MY_WINDOW::WIDTH);
        printw("is not ready to kill");
    }
}

void NC_Graphic::display_killed(Manager manager){
    std::map<char, int> killed = manager.get_killed();
    move(MY_WINDOW::HEIGHT + 7, 0); clrtoeol(); printw("close combat = %d", killed['c']);
    move(MY_WINDOW::HEIGHT + 8, 0); clrtoeol(); printw("range combat = %d", killed['r']);
    move(MY_WINDOW::HEIGHT + 9, 0); clrtoeol(); printw("dd spell = %d", killed['d']);
    move(MY_WINDOW::HEIGHT + 10, 0); clrtoeol(); printw("area spell = %d", killed['a']);
    move(MY_WINDOW::HEIGHT + 11, 0); clrtoeol(); printw("set traps = %d", killed['t']);
}


void NC_Graphic::display_need_to_do(Manager manager){
    move(10, 4*MY_WINDOW::WIDTH); printw("L E V E L: %d", manager.get_level());
    move(12, 4*MY_WINDOW::WIDTH); printw("LEVEL'S  G O A L");
    switch(manager.get_level()){
        case 1:
            move(13, 4*MY_WINDOW::WIDTH); printw("close combat = 5");
            break;
        case 2:
            move(13, 4*MY_WINDOW::WIDTH); printw("range combat = 5");
            break;
        case 3:
            move(13, 4*MY_WINDOW::WIDTH); printw("all spells = 10  ");
            break;
    }

    move(15, 4*MY_WINDOW::WIDTH); printw("ENEMIES'S HEALTH: %d", manager.get_level() + 1);
}

void NC_Graphic::display(Manager manager){
    display_lifes(manager);
    display_combat(manager);
    display_moves(manager);
    display_controls();
    display_spells_controls();
    display_my_spells(manager);
    display_enemy_tower(manager);
    display_killed(manager);
    display_need_to_do(manager);
}

