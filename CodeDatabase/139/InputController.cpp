//#include "InputControler.h"
//#include "DataTypes/SpellType.h"
//#include "Game.h"
//#include "States/MenuState.h"
//#include "States/PlayState.h"
//#include "States/PauseState.h"
//#include "States/LevelBetweenState.h"
//#include <iostream>
//
//
//InputControler::InputControler(Game* game)
//{
//    this->game = game;
//}
//
//
//void InputControler::handlePlayState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent())
//    {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//
//        if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
//        {
//            if (keyPress->scancode == sf::Keyboard::Scancode::Escape) {
//                std::cout << u8"Меню PauseState\n";
//                game->setState(new PauseState);
//            }
//        }
//
//
//        if (const auto* keyPress = event->getIf<sf::Event::KeyPressed>())
//        {
//            moveEnemies = true;
//            switch (keyPress->scancode)
//            {
//            case sf::Keyboard::Scancode::Num1:
//            case sf::Keyboard::Scancode::Num2:
//            case sf::Keyboard::Scancode::Num3:
//            case sf::Keyboard::Scancode::Num4:
//            case sf::Keyboard::Scancode::Num5:
//            case sf::Keyboard::Scancode::Z:
//            case sf::Keyboard::Scancode::X:
//            case sf::Keyboard::Scancode::C:
//            case sf::Keyboard::Scancode::V:
//            case sf::Keyboard::Scancode::B:
//            case sf::Keyboard::Scancode::M:
//                handleSpellsButton(keyPress->scancode);
//                break;
//            case sf::Keyboard::Scancode::LShift:
//            case sf::Keyboard::Scancode::Left:
//            case sf::Keyboard::Scancode::Right:
//            case sf::Keyboard::Scancode::Up:
//            case sf::Keyboard::Scancode::Down:
//                handleMove(keyPress->scancode);
//                break;
//            case sf::Keyboard::Scancode::L:
//            case sf::Keyboard::Scancode::A:
//            case sf::Keyboard::Scancode::D:
//            case sf::Keyboard::Scancode::W:
//            case sf::Keyboard::Scancode::S:
//                handleAttack(keyPress->scancode);
//                break;
//            default:
//                moveEnemies = false;
//                break;
//            }
//
//            game->getExecuteUseCase()->checkSpawnNewEnemy(moveEnemies);
//        }
//        else
//        {
//            moveEnemies = false;
//        }
//
//        if (const auto* mousePress = event->getIf<sf::Event::MouseButtonPressed>())
//            handleSpellsMouse(window, mousePress->button);
//
//
//        if (moveEnemies) {
//            game->getExecuteUseCase()->executeEntity();
//            game->getExecuteUseCase()->refresh();
//        }
//    }
//}
//
//
//void InputControler::handleMenuState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent()) {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
//                std::cout << u8"Новая игра ->PlayState\n";
//                game->resetLevel(window);
//                game->setState(new PlayState);
//            }
//            if (game->running()) {
//                if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
//                    std::cout << u8"Продолжить игру ->PlayState\n";
//                    game->resizeGameWindow(window, Point(game->fieldWidth(), game->fieldHeight()));
//                    game->setState(new PlayState);
//                }
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {
//                std::cout << u8"Загрузить последнюю игру ->PlayState\n";
//                if (game->loadGame(window))
//                    game->setState(new PlayState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
//                std::cout << u8"Выход\n";
//                game->stop();
//                window.close();
//            }
//        }
//    }
//}
//
//
//void InputControler::handlePauseState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent()) {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
//                std::cout << u8"Возврат в PlayState\n";
//                game->setState(new PlayState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
//                std::cout << u8"Сохранение игры в PauseState\n";
//                game->saveGame();
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
//                std::cout << u8"Загрузка игры в PauseState\n";
//                if(game->loadGame(window))
//                    game->setState(new PlayState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
//                std::cout << u8"Выход в главное меню MenuState\n";
//                game->setState(new MenuState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
//                std::cout << u8"Тема игры лес в PauseState\n";
//                game->getRenderer()->setTheme(Renderer::Theme::DEFAULT);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
//                std::cout << u8"Тема игры зима в PauseState\n";
//                game->getRenderer()->setTheme(Renderer::Theme::WINTER);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num3) {
//                std::cout << u8"Тема игры пустыня в PauseState\n";
//                game->getRenderer()->setTheme(Renderer::Theme::DISERT);
//            }
//        }
//    }
//}
//
//
//void InputControler::handleLevelWinState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent()) {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//            if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
//                std::cout << u8"Выбор улучшений в LevelBetweenState\n";
//                game->setState(new LevelBetweenState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
//                std::cout << u8"Выход в меню NenuState\n";
//                game->setState(new MenuState);
//            }
//        }
//    }
//}
//
//
//void InputControler::handleLevelBetweenState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent()) {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num1) {
//                std::cout << u8"+25 к здоровью ->PlayState\n";
//                game->nextLevel(window, 25, 0);
//                game->setState(new PlayState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Num2) {
//                std::cout << u8"+15 к урону ->PlayState\n";
//                game->nextLevel(window, 0, 15);
//                game->setState(new PlayState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
//                std::cout << u8"Выход в меню NenuState\n";
//                game->setState(new MenuState);
//            }
//        }
//    }
//}
//
//
//void InputControler::handleGameWinState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent()) {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
//                std::cout << u8"Выход в меню NenuState\n";
//                game->stop();
//                game->setState(new MenuState);
//            }
//        }
//    }
//}
//
//
//void InputControler::handleGameOverState(sf::RenderWindow& window)
//{
//    while (const std::optional event = window.pollEvent()) {
//        if (event->is<sf::Event::Closed>())
//            window.close();
//
//        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
//            if (keyPressed->scancode == sf::Keyboard::Scancode::R) {
//                std::cout << u8"Повторить уровень ->PlayState\n";
//                game->getExecuteUseCase()->restartGame();
//                game->setState(new PlayState);
//            }
//            if (keyPressed->scancode == sf::Keyboard::Scancode::Q) {
//                std::cout << u8"Выход в меню NenuState\n";
//                game->setState(new MenuState);
//            }
//        }
//    }
//}
//
//
//void InputControler::handleMove(sf::Keyboard::Scancode keyCode)
//{
//    Point vector;
//    switch (keyCode)
//    {
//    case sf::Keyboard::Scancode::LShift:
//        game->getExecuteUseCase()->changePlayerFastMode();
//        break;
//    case sf::Keyboard::Scancode::Left:
//        vector.setX(-1);
//        break;
//    case sf::Keyboard::Scancode::Right:
//        vector.setX(1);
//        break;
//    case sf::Keyboard::Scancode::Up:
//        vector.setY(-1);
//        break;
//    case sf::Keyboard::Scancode::Down:
//        vector.setY(1);
//        break;
//    }
//
//    
//    if (!game->getExecuteUseCase()->executePlayer(vector))
//        moveEnemies = false;
//    else
//        game->getExecuteUseCase()->increasePlayerSteps();
//}
//
//
//void InputControler::handleAttack(sf::Keyboard::Scancode keyCode)
//{
//    Point vector;
//
//    switch (keyCode)
//    {
//    case sf::Keyboard::Scancode::L:
//        game->getExecuteUseCase()->changePlayerLongFight();
//        break;
//    case  sf::Keyboard::Scancode::A:
//        vector.setX(-1);
//        break;
//    case  sf::Keyboard::Scancode::D:
//        vector.setX(1);
//        break;
//    case  sf::Keyboard::Scancode::W:
//        vector.setY(-1);
//        break;
//    case  sf::Keyboard::Scancode::S:
//        vector.setY(1);
//        break;
//    }
//
//    if (game->getExecuteUseCase()->executePlayerAttack(vector))
//    {
//        moveEnemies = true;
//        game->getExecuteUseCase()->increasePlayerSteps();
//    }
//    else
//        moveEnemies = false;
//}
//
//
//void InputControler::handleSpellsButton(sf::Keyboard::Scancode keyCode)
//{
//    moveEnemies = false;
//    switch (keyCode)
//    {
//    case sf::Keyboard::Scancode::Num1:
//        game->getExecuteUseCase()->takeSpell(0);
//        break;
//    case sf::Keyboard::Scancode::Num2:
//        game->getExecuteUseCase()->takeSpell(1);
//        break;
//    case sf::Keyboard::Scancode::Num3:
//        game->getExecuteUseCase()->takeSpell(2);
//        break;  
//    case sf::Keyboard::Scancode::Num4:
//        game->getExecuteUseCase()->takeSpell(3);
//        break;
//    case sf::Keyboard::Scancode::Num5:
//        game->getExecuteUseCase()->takeSpell(4);
//        break;
//    case sf::Keyboard::Scancode::M:
//        if(game->getExecuteUseCase()->useSpell())
//            moveEnemies = true;
//        break;
//    case sf::Keyboard::Scancode::Z:
//        game->getExecuteUseCase()->addSpell(SpellType::AreaDamageSpell);
//        break;
//    case sf::Keyboard::Scancode::X:
//        game->getExecuteUseCase()->addSpell(SpellType::DirectDamageSpell);
//        break;
//    case sf::Keyboard::Scancode::C:
//        game->getExecuteUseCase()->addSpell(SpellType::TrapSpell);
//        break;
//    case sf::Keyboard::Scancode::V:
//        game->getExecuteUseCase()->addSpell(SpellType::AllySpell);
//        break;
//    case sf::Keyboard::Scancode::B:
//        game->getExecuteUseCase()->addSpell(SpellType::UpgradeSpell);
//        break;
//    }
//
//}
//
//
//void InputControler::handleSpellsMouse(sf::RenderWindow& window, sf::Mouse::Button mouseButton)
//{
//    Point destination;
//    switch (mouseButton)
//    {
//    case sf::Mouse::Button::Left:
//        destination.setX(sf::Mouse::getPosition(window).x);
//        destination.setY(sf::Mouse::getPosition(window).y);
//        if(game->getExecuteUseCase()->useSpell(destination))
//            moveEnemies = true;
//        break;
//    }
//}