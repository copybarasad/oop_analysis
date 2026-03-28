#include "commands.h"
#include "game.h"

Move_command::Move_command(Movement::Direction dir) : direction(dir){}
int Move_command::execute(Game& game){
    return game.move(direction);
}

int Attack_command::execute(Game& game){
    return game.attack();
}

int Use_spell_command::execute(Game& game){
    return game.use_spell();
}

int Buy_spell_command::execute(Game& game){
    return game.buy_spell();
}

int Change_weapon_command::execute(Game& game){
    return game.change_weapon();
}

int Save_game_command::execute(Game& game){
    return game.save_game();
}

int Exit_game_command::execute(Game& game){
    game.exit_game();
    return 99;
}

Main_menu_commands::Main_menu_commands(char _choice) : choice(_choice){}
int Main_menu_commands::execute(Game& game){
    game.main_choice(choice);
    return 1;
}

Upgrade_commands::Upgrade_commands(char _choice) : choice(_choice){}
int Upgrade_commands::execute(Game& game){
    game.upgrade(choice);
    return 1;
}
