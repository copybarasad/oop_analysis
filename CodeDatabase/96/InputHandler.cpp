#include "InputHandler.h"




Command ConsoleInput::getCommand(int& a1, int& a2) {
        char cmd = getChar();
    
        if (cmd == 'w') return Command::MoveUp;
        if (cmd == 's') return Command::MoveDown;
        if (cmd == 'a') return Command::MoveLeft;
        if (cmd == 'd') return Command::MoveRight;
        if (cmd == 'g'){
            std::string params;
            std::getline(std::cin, params);
            if (params == "up") return Command::AttackUp;
            if (params == "down") return Command::AttackDown;
            if (params == "left") return Command::AttackLeft;
            if (params == "right") return Command::AttackRight;
        }
        // if (cmd == "attack up") return Command::AttackUp;
        // if (cmd == "attack down") return Command::AttackDown;
        // if (cmd == "attack left") return Command::AttackLeft;
        // if (cmd == "attack right") return Command::AttackRight;

        if (cmd == 'o') return Command::Settings;
        if (cmd == 'e') return Command::Exit;
        if (cmd == 'p') return Command::Save;
        if (cmd == 'l') return Command::Load;
        if (cmd == 'b') return Command::Back;
        if (cmd == 'c'){
            std::cout << "ast ";
            std::string params;
            std::getline(std::cin, params);
            
            std::stringstream ss("cast " + params);
            std::string cmd;
            ss >> cmd >> a1 >> a2;
            return Command::CastSpell;
        }
        if (cmd == 't') {
            std::cout << "select spell ";
            std::string params;
            std::getline(std::cin, params);
            
            std::stringstream ss("select spell " + params);
            std::string cmd;
            ss >> cmd >> a1;
            return Command::SelectSpell;
        }

        if (cmd == 'n') return Command::NewGame;

        return Command::None;
    }