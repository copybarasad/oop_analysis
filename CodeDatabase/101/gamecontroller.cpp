#include "gamecontroller.h"
#include "gameview.h"


template<typename TInputHandler, typename TRenderer>
GameController<TInputHandler, TRenderer>::GameController(GameModel& model, Visualization<TRenderer>* visualizer)
    : timerId(0), gameModel(&model), visitor(new Visitor(&model)),
    isRunning(false), visualizer(visualizer) {

    gameModel->set_current_option(mainMenu);

    if (visualizer) {
        visualizer->setGameModel(gameModel);
    }
}


template<typename TInputHandler, typename TRenderer>
GameController<TInputHandler, TRenderer>::~GameController() {
    delete visitor;
    if (timerId != 0) {
        killTimer(timerId);
    }
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::startNewGame() {
    if (timerId != 0) {
        killTimer(timerId);
        timerId = 0;
    }

    gameModel->set_current_level(1);
    gameModel->set_score(-gameModel->get_score());
    gameModel->get_player().recovery_health();


    int currentMoney = gameModel->get_player().get_money();
    if (currentMoney > 100) {
        gameModel->get_player().spend_money(currentMoney - 100);
    } else if (currentMoney < 100) {
        gameModel->get_player().add_money(100 - currentMoney);
    }


    auto* hand = gameModel->get_player().get_hand();
    if (hand) {
        hand->first_random_card(*visitor);
    }


    gameModel->get_enemy().set_alive(true);
    gameModel->get_enemy().recovery_health();
    gameModel->get_tower().set_alive(true);
    gameModel->get_tower().recovery_health();


    auto& leons = gameModel->get_leons();
    for (Leon* leon : leons) {
        delete leon;
    }
    leons.clear();

    gameModel->change_flagAtack(0);
    gameModel->change_inGame(true);
    gameModel->set_current_option(processGame);

    delete visitor;
    visitor = new Visitor(gameModel);
    gameModel->get_player().get_hand()->first_random_card(*visitor);


    timerId = startTimer(DELAY);
    isRunning = true;
    gameModel->notify();
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::timerEvent(QTimerEvent*) {
    if(gameModel->get_current_option() != processGame) return;

    bool enemyDead = !gameModel->get_enemy().get_alive();
    bool towerDead = !gameModel->get_tower().get_alive();

    if(enemyDead && towerDead) {
        if (!gameModel->is_level_completed()) {
            gameModel->complete_level();
            gameModel->set_current_option(upgradeMenu);
            gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
        }
        return;
    }

    static int counter = 0;
    counter++;

    if(counter % 10 == 0) gameModel->enemy_journey();
    if(counter % 15 == 0) gameModel->enemy_attack();
    if(counter % 20 == 0) gameModel->tower_attack();
    if(counter % 25 == 0 && !gameModel->get_leons().empty()) gameModel->leon_journey();

    gameModel->notify();
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::processCommands() {
    while (inputHandler.hasCommands()) {
        GameCommand cmd = inputHandler.getNextCommand();
        executeCommand(cmd);
    }
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::saveGame() {
    std::ofstream file("SaveGame.dat", std::ios::binary);
    if (!file) {
        qDebug() << "ERROR: Cannot create SaveGame.dat";
        return;
    }

    try {
        gameModel->serialize(file);
        file.close();
    } catch(const std::exception& e) {
        qDebug() << "Serialize error:" << e.what();
        file.close();
    }
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::loadGame() {
    std::ifstream file("SaveGame.dat", std::ios::binary);
    if (!file) {
        return;
    }

    try {
        if (timerId != 0) {
            killTimer(timerId);
            timerId = 0;
        }

        gameModel->deserialize(file);
        file.close();

        timerId = startTimer(DELAY);
        isRunning = true;

        gameModel->set_current_option(processGame);
        gameModel->get_buttons().updateVisibility(gameModel->get_current_option());

    } catch(const std::exception& e) {
        file.close();
        timerId = 0;
        startNewGame();
    }
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::executeCommand(const GameCommand& cmd) {
    OptionGame currentOption = gameModel->get_current_option();

    // // Добавляем обработку START_DRAG для отладки
    // if (cmd.type == GameCommand::START_DRAG) {
    //     qDebug() << "Drag started for spell:" << QString::fromStdString(cmd.data);
    //     // Можно ничего не делать, просто логируем
    //     return;
    // }

    // if (cmd.type == GameCommand::MOUSE_DROP) {
    //     qDebug() << "Spell dropped:" << QString::fromStdString(cmd.data)
    //     << "at position:" << cmd.x << "," << cmd.y;
    // }


    if (gameModel->is_level_completed() || currentOption == upgradeMenu) {

        switch(cmd.type) {
        case GameCommand::NEW_GAME:
            if (gameModel->get_skill_points() > 0) {
                gameModel->upgrade_player_health();
                gameModel->spend_skill_point();
            }
            break;

        case GameCommand::LOAD_GAME:
            if (gameModel->get_skill_points() > 0) {
                gameModel->upgrade_player_damage();
                gameModel->spend_skill_point();
            }
            break;

        case GameCommand::QUIT_GAME:
            if (gameModel->get_skill_points() > 0) {
                gameModel->upgrade_spell_power();
                gameModel->spend_skill_point();
            }
            break;

        case GameCommand::CONTINUE_LEVEL:
            if (gameModel->get_skill_points() == 0) {
                gameModel->reset_level_completed();
                gameModel->reset_for_new_level();

                if (timerId != 0) {
                    killTimer(timerId);
                }
                timerId = startTimer(DELAY);

                gameModel->set_current_option(processGame);
                gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
            }
            break;

        case GameCommand::MOUSE_DROP:
            break;
        }

        gameModel->notify();
        return;
    }


    if (currentOption == mainMenu || currentOption == pauseMenu) {
        switch(cmd.type) {
        case GameCommand::NEW_GAME:
            if (currentOption == mainMenu) {
                startNewGame();
            } else if (currentOption == pauseMenu) {
                gameModel->set_current_option(processGame);
            }
            gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
            break;

        case GameCommand::LOAD_GAME:
            if (currentOption == pauseMenu) {
                saveGame();
            } else if (currentOption == mainMenu) {
                loadGame();
            }
            gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
            break;

        case GameCommand::QUIT_GAME:
            if (currentOption == pauseMenu) {
                gameModel->set_current_option(mainMenu);
                gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
            } else if (currentOption == mainMenu) {
                QApplication::quit();
            }
            break;

        case GameCommand::PAUSE_GAME:
            if (currentOption == pauseMenu) {
                gameModel->set_current_option(mainMenu);
                gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
            }
            break;

        case GameCommand::MOUSE_DROP:
            break;
        }

        gameModel->notify();
        return;
    }

    if (currentOption == processGame) {
        int oldX = gameModel->get_player().get_x();
        int oldY = gameModel->get_player().get_y();
        switch(cmd.type) {
        case GameCommand::PAUSE_GAME:
            gameModel->set_current_option(pauseMenu);
            gameModel->get_buttons().updateVisibility(gameModel->get_current_option());
            break;

        case GameCommand::MOVE_LEFT: {
            int count_for_slow = gameModel->get_player().get_count_for_slow();
            std::vector<int> coord_info = gameModel->check_coords_map(-1, 0, count_for_slow, 0);
            gameModel->get_player().player_journey(coord_info[0], coord_info[1]);
            gameModel->get_player().change_direction_attack(left);
            int newX = gameModel->get_player().get_x();
            int newY = gameModel->get_player().get_y();
            gameModel->notifyPlayerMoved(oldX, oldY, newX, newY);
            break;
        }

        case GameCommand::MOVE_RIGHT: {
            int count_for_slow = gameModel->get_player().get_count_for_slow();
            std::vector<int> coord_info = gameModel->check_coords_map(1, 0, count_for_slow, 0);
            gameModel->get_player().player_journey(coord_info[0], coord_info[1]);
            gameModel->get_player().change_direction_attack(right);
            int newX = gameModel->get_player().get_x();
            int newY = gameModel->get_player().get_y();
            gameModel->notifyPlayerMoved(oldX, oldY, newX, newY);
            break;
        }

        case GameCommand::MOVE_UP: {
            int count_for_slow = gameModel->get_player().get_count_for_slow();
            std::vector<int> coord_info = gameModel->check_coords_map(0, -1, count_for_slow, 0);
            gameModel->get_player().player_journey(coord_info[0], coord_info[1]);
            gameModel->get_player().change_direction_attack(up);
            int newX = gameModel->get_player().get_x();
            int newY = gameModel->get_player().get_y();
            gameModel->notifyPlayerMoved(oldX, oldY, newX, newY);
            break;
        }

        case GameCommand::MOVE_DOWN: {
            int count_for_slow = gameModel->get_player().get_count_for_slow();
            std::vector<int> coord_info = gameModel->check_coords_map(0, 1, count_for_slow, 0);
            gameModel->get_player().player_journey(coord_info[0], coord_info[1]);
            gameModel->get_player().change_direction_attack(down);
            int newX = gameModel->get_player().get_x();
            int newY = gameModel->get_player().get_y();
            gameModel->notifyPlayerMoved(oldX, oldY, newX, newY);
            break;

        }

        case GameCommand::ATTACK_CLOSE:
            if (gameModel->get_player().get_typeBattle() == close_combat) {
                gameModel->player_close_attack();
            } else {
                gameModel->player_ranged_attack();
            }
            break;

        case GameCommand::SWITCH_WEAPON:
            gameModel->get_player().change_typeBattle();
            break;

        case GameCommand::MOUSE_DROP:
            gameModel->get_player().get_hand()->find_and_use(*visitor, cmd.data, cmd.x, cmd.y);
            break;
        }

        gameModel->notify();
    }
}

template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::keyboardHandler(QKeyEvent* e)  {
    if(!e) return;
    inputHandler.handleKeyPressEvent(e);
    processCommands();
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::mouseMoveHandler(QMouseEvent* e) {
    if (!e) return;

    std::vector<QPushButton*> buttons = gameModel->get_buttons().get_my_buttons();
    inputHandler.handleMouseMove(e, this, buttons);

    // Обрабатываем команды
    processCommands();
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::mouseClickHandler(QMouseEvent* e)  {
    inputHandler.handleMouseClick(e);
    processCommands();
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::mouseDragHandler(QDragEnterEvent* e)  {
    inputHandler.handleDragEnter(e);
}


template<typename TInputHandler, typename TRenderer>
void GameController<TInputHandler, TRenderer>::mouseDropHandler(QDropEvent* e) {
    if (!e) return;

    std::string str1 = e->mimeData()->text().toStdString();

    if (str1.empty()) {
        e->acceptProposedAction();
        return;
    }

    int x = e->pos().x();
    int y = e->pos().y();

    gameModel->get_player().get_hand()->find_and_use(*visitor, str1, x, y);

    e->acceptProposedAction();
    gameModel->notify();

}



template<typename TInputHandler, typename TRenderer>
HandlerEvent* GameController<TInputHandler, TRenderer>::getEventHandler() {
    return this;
}


template<typename TInputHandler, typename TRenderer>
Visualization<TRenderer>* GameController<TInputHandler, TRenderer>::getVisualizer() { return visualizer; }


template class GameController<InputHandler, GameView>;
