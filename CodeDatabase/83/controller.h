#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "game.h"
#include "visualizer.h" 
#include "renderer.h"   
#include "app_state.h"
#include "input_handler.h"

template <typename TInput>
class Controller {
public:
    Controller(Visualizer<Renderer>& visualizer)
        : input(),
          visualizer(visualizer), 
          state(AppState::MENU),
          cursorX(0),
          cursorY(0),
          spellIndex(0),
          needRender(true)
    {}

    void run(Game& game) {
        while (state != AppState::EXIT) {
            if (needRender) {
                visualizer.updateAndRender(game, state, cursorX, cursorY, spellIndex);
            }
            
            Command cmd = input.processInput();
            handle(cmd, game);
        }
    }

private:
    TInput input;
    Visualizer<Renderer>& visualizer; 
    AppState state;

    int cursorX;
    int cursorY;
    int spellIndex;
    bool needRender;

    void afterPlayerTurn(Game& game) {
        game.enemiesMove();
        game.towersUpdate();
        
        if (game.allEnemiesDead()) {
            state = AppState::WIN;
        }        
        needRender = true;
    }

    void handle(Command cmd, Game& game) {
        if (state == AppState::MENU) {
            switch (cmd) {
                case Command::MENU_NEW_GAME:
                    game.startNewGame();
                    state = AppState::GAME;
                    break;

                case Command::MENU_LOAD_GAME:
                    if (game.loadGame()) {
                        state = AppState::GAME;
                    }
                    break;

                case Command::MENU_EXIT:
                    state = AppState::EXIT;
                    break;

                default:
                    break;
            }
            return;
        }

        if (state == AppState::WIN) {
            state = AppState::MENU;
            needRender = true;
            return;
        }


        if (state == AppState::GAME) {
            switch (cmd) {
                case Command::MOVE_UP:
                    game.movePlayer(Direction::Up);
                    afterPlayerTurn(game);
                    break;

                case Command::MOVE_DOWN:
                    game.movePlayer(Direction::Down);
                    afterPlayerTurn(game);
                    break;

                case Command::MOVE_LEFT:
                    game.movePlayer(Direction::Left);
                    afterPlayerTurn(game);
                    break;

                case Command::MOVE_RIGHT:
                    game.movePlayer(Direction::Right);
                    afterPlayerTurn(game);
                    break;

                case Command::ATTACK:
                    game.castSpell();
                    afterPlayerTurn(game);
                    break;

                case Command::CHANGE_SPELL: {
                    auto spells = game.getAvailableSpells();
                    if (!spells.empty()) {
                        spellIndex = (spellIndex + 1) % spells.size();
                        game.chooseSpellByIndex(spellIndex);
                    }
                    break;
                }

                case Command::SAVE_GAME:
                    game.saveGame();
                    break;

                case Command::EXIT_TO_MENU:
                    game.returnToMenu();
                    state = AppState::MENU;
                    break;

                default:
                    break;
            }
        }
    }
};

#endif