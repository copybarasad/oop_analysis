#include "../headers/gameController.hpp"
template <typename InputHandlerType>

GameController() : inputHandler(std::make_unique<InputHandlerType>()), game(std::make_unique<Game>()){
    inputHandler->loadKeyBindings("saves/controls.json");

    game->updateKeyBindingsInfo(inputHandler->getKeyBindingsInfo());
}

void run(){
    if (!game)
        return;
    
    auto& window = game->getWindow();
    
    while (window.isOpen() && game->isRunning()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            else
                handleInput(event);
        }
        
        game->update();
        
        window.clear(sf::Color::Black);
        game->render();
        window.display();
    }
}

void handleInput(const sf::Event& event){
    auto command = inputHandler->handleEvent(event, game->getCurrentState().state);
    if (!command || command->getType() == CommandType::UNKNOWN)
        return;
    
    executeCommand(*command);
}

void executeCommand(const Command& command){
    if (!game)
        return;
    
    switch(command.getType()){
        case CommandType::MOVE_UP:
            game->movePlayer(direction::UP);
            break;
        case CommandType::MOVE_DOWN:
            game->movePlayer(direction::DOWN);
            break;
        case CommandType::MOVE_LEFT:
            game->movePlayer(direction::LEFT);
            break;
        case CommandType::MOVE_RIGHT:
            game->movePlayer(direction::RIGHT);
            break;
        case CommandType::TOGGLE_FIGHT:
            game->toggleFight();
            break;
        case CommandType::SHOW_ATTACK_RADIUS:
            game->showAttackRadius();
            break;
        case CommandType::BUY_SPELL:
            game->enterBuySpellMode();
            break;
        case CommandType::SAVE_GAME:
            game->saveGameCommand();
            break;
        case CommandType::CAST_SPELL_1:
            game->castSpell(0);
            break;
        case CommandType::CAST_SPELL_2:
            game->castSpell(1);
            break;
        case CommandType::CAST_SPELL_3:
            game->castSpell(2);
            break;
        case CommandType::CAST_SPELL_4:
            game->castSpell(3);
            break;
        case CommandType::CAST_SPELL_5:
            game->castSpell(4);
            break;
        case CommandType::CAST_SPELL_6:
            game->castSpell(5);
            break;
        case CommandType::CANCEL:
            game->cancelAction();
            break;
        case CommandType::NEW_GAME:
            game->startNewGameCommand();
            break;
        case CommandType::CONTINUE_GAME:
            game->continueGame();
            break;
        case CommandType::EXIT:
            game->exitGame();
            break;
        case CommandType::CONTINUE_NEXT_LEVEL:
            game->continueToNextLevel();
            break;
        case CommandType::MAIN_MENU:
            game->returnToMainMenu();
            break;
        case CommandType::UPGRADE_1:
            game->applyUpgrade(0);
            break;
        case CommandType::UPGRADE_2:
            game->applyUpgrade(1);
            break;
        case CommandType::UPGRADE_3:
            game->applyUpgrade(2);
            break;
        case CommandType::UPGRADE_4:
            game->applyUpgrade(3);
            break;
        case CommandType::BUY_SPELL_1:
            game->buySpell(0);
            break;
        case CommandType::BUY_SPELL_2:
            game->buySpell(1);
            break;
        case CommandType::BUY_SPELL_3:
            game->buySpell(2);
            break;
        case CommandType::BUY_SPELL_4:
            game->buySpell(3);
            break;
        case CommandType::BUY_SPELL_5:
            game->buySpell(4);
            break;
        case CommandType::MOUSE_CLICK:
            {const auto& mouseCmd = dynamic_cast<const MouseCommand&>(command);
            game->handleMouseClick(mouseCmd.getPosition()); }
            break;
        default:
            break;
    }
}