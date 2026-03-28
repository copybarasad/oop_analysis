#include "console_input_processor.h"
#include <iostream>

#include "commands/wasd_cmd.h"
#include "commands/e_cmd.h"
#include "commands/f_cmd.h"
#include "commands/load_cmd.h"
#include "commands/save_cmd.h"
#include "commands/cast_spell_cmd.h"
#include "commands/quit_cmd.h"


std::unique_ptr<Command> ConsoleInputProcessor::process_input() {
    char choice;
     std::cin >> choice;

     switch (std::tolower(choice)) {
         case 'w':
             return std::make_unique<WasdCommand>('w');
         case 'a':
             return std::make_unique<WasdCommand>('a');
         case 's':
             return std::make_unique<WasdCommand>('s');
         case 'd':
             return std::make_unique<WasdCommand>('d');
         case 'f':
             return std::make_unique<NewCombatModeCommand>();
         case 'e':
             return std::make_unique<KickCommand>();

         // NEW BUTTONS (3_d lab)
         case 'l': // LOAD
            return std::make_unique<LoadCommand>();
         case 'v': // SAVE
             return std::make_unique<SaveCommand>();
         // обработка ввода заклинания
         case '0':
         case '1':
         case '2':
         case '3':
         case '4': {
             int spell_index = choice - '0';
             return std::make_unique<CastSpellCommand>(spell_index);
         }
         case 'q':
             return std::make_unique<QuitCommand>();

         default:
             std::cout << "Неверная команда! Пропускаем ход.\n";
             return nullptr;
     }
}


