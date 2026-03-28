#pragma once
#include "consoleui.h"
#include "game.h"
#include <iostream>

namespace game {
    class ConsoleRenderer {
        private:
        
        ConsoleUI ui_;

        public:

        void clear() const { ui_.clearScreen(); }
        
        void draw(const Game& g) {
            ui_.clearScreen();

            switch (g.state()) {
                case GameState::MainMenu:
                    ui_.printMainMenu();
                    return;
                
                case GameState::Help:
                    ui_.printHelpMessage();
                    return;
                
                case GameState::SizeMenu:
                    ui_.printFieldSizeHeader();
                    std::cout << Color::Text << "Width, Height:\n" << Color::Slime;
                    std::cout.flush();

                    return;
                
                case GameState::RunningLevel:
                    if (g.hasField()) {
                        ui_.printLevelBanner(g.currentLevel(), g.field().getW());
                        ui_.drawField(g.field());
                        ui_.drawHUD(g.field());

                        std::cout << "\n\n\tYour move: ";
                    }
                    return;
                
                case GameState::RestartPrompt:
                    ui_.printLoseMessage(g.currentLevel());
                    ui_.printRestartMessage();
                    return;
                
                case GameState::LevelUp:
                    ui_.printWinMessage(g.currentLevel());
                    ui_.printLevelUpMenu(g.cfg());
                    return;
                
                case GameState::Popup: {
                    switch (g.popupType()) {
                        case PopupType::SaveOk: {
                            ui_.printSaveSuccess();
                            break;
                        }
                        case PopupType::SaveError: {
                            ui_.printSaveError(g.popupText());
                            break;
                        }
                        case PopupType::LoadOk: {
                            ui_.printLoadSuccess();
                            break;
                        }
                        case PopupType::LoadError: {
                            ui_.printLoadError(g.popupText());
                            break;
                        }
                        case PopupType::SpellUpgraded: {
                            std::cout << '\n' << Color::Accent  << "[INFO]"
                                      << '\n' << Color::Default << g.popupText()
                                      << '\n';
                            break;
                        }
                        default:
                            break;
                    }
                    ui_.printSkipMessage();
                    return;
                }

                case GameState::Exit:
                    ui_.clearScreen();
                    return;
            }
        }

        ConsoleUI& ui() { return ui_; }
    };
}