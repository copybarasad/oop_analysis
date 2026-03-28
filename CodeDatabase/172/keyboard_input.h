#pragma once
#include "command.h"
#include "key_bindings.h"
#include <cctype>
#include <iostream>
#include <limits>

namespace game {
    enum class GameState;

    class KeyboardInput {
        private:
            KeyBindings binds_;

        public:
        KeyboardInput() : binds_(KeyBindings::load("controls.cfg")) {}

        Command readCommand(GameState state) const {
            Command cmd;
            
            if (state == GameState::SizeMenu) {
                int w = 0, h = 0;

                if ( !(std::cin >> w) ) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    cmd.type = CommandType::SetFieldSize;
                    cmd.w = 10;
                    cmd.h = 10;

                    return cmd;
                }

                if ( !(std::cin >> h) ) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    
                    cmd.type = CommandType::SetFieldSize;
                    cmd.w = 10;
                    cmd.h = 10;

                    return cmd;
                }

                cmd.type = CommandType::SetFieldSize;
                cmd.w = w;
                cmd.h = h;

                return cmd;
            }

            // Read single key
            char c = 0;
            if ( !(std::cin >> c) ) {
                std::cin.clear();
                cmd.type = CommandType::None;
                return cmd;
            }
            c = static_cast<char>(std::tolower( static_cast<unsigned char>(c) ));

            // Popup -- any key to continue
            if (state == GameState::Popup) {
                cmd.type = CommandType::PopupConfirm;
                return cmd;
            }

            // MainMenu options
            if (state == GameState::MainMenu) {
                if (c == 'n') { cmd.type = CommandType::MenuNewGame;  return cmd; }
                if (c == 'l') { cmd.type = CommandType::MenuLoadGame; return cmd; }
                if (c == 'q') { cmd.type = CommandType::MenuQuit;     return cmd; }

                cmd.type = CommandType::None;
                return cmd;
            }

            // Help options
            if (state == GameState::Help) {
                if (c == 'q') { cmd.type = CommandType::Quit; return cmd; }
                if (c == 'm') { cmd.type = CommandType::HelpOpenSizeMenu; return cmd; }

                cmd.type = CommandType::HelpConfirm; // any key -- continue
                return cmd;
            }

            // Level up options
            if (state == GameState::LevelUp) {
                if (c == '1') { cmd.type = CommandType::LevelUp1; return cmd; }
                if (c == '2') { cmd.type = CommandType::LevelUp2; return cmd; }
                if (c == '3') { cmd.type = CommandType::LevelUp3; return cmd; }

                cmd.type = CommandType::None;
                return cmd;
            }

            // Level options
            if (state == GameState::RunningLevel) {
                auto it = binds_.key2command.find(c);
                if (it != binds_.key2command.end()) {
                    return it->second;
                }

                if (c >= '1' && c <= '9') { 
                    cmd.type = CommandType::CastSpell;
                    cmd.index = static_cast<size_t>(c - '1');
                    return cmd;
                }
            }

            cmd.type = CommandType::None;
            return cmd;
        }
    };
}