#include "MainMenuScreen.h"
#include "GameScreen.h"
#include "../Program.h"

// void MainMenuScreen::render()
// {
// Text title(mFont, "MY GAME");
// title.setCharacterSize(60);
// title.setOrigin(Vector2f(0.5, 0.5));
// title.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 3.f));

// Text options(mFont, "1. New game \n2. Load game \n3. Exit");
// options.setCharacterSize(30);
// options.setOrigin(Vector2f(0.5, 0.5));
// options.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 3.f + 60));
// while (window.isOpen())
// {
//     while (const std::optional event = window.pollEvent())
//     {
//         if (event->is<sf::Event::Closed>())
//             window.close();
//         else if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
//         {
//             if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
//                 window.close();
//         }

//         if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
//         {

//             try
//             {
//                 if (keyPressed->code == Keyboard::Key::Num1 || keyPressed->code == Keyboard::Key::Numpad1)
//                 {
//                     mGame->newGame();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//                 if (keyPressed->code == Keyboard::Key::Num2 || keyPressed->code == Keyboard::Key::Numpad2)
//                 {
//                     mGame->loadGame();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//             }
//             catch (const ios_base::failure &e)
//             {
//                 std::cerr << e.what() << '\n';
//             }
//             catch (const exception &e)
//             {
//                 std::cerr << e.what() << '\n';
//             }

//             if (keyPressed->code == Keyboard::Key::Num3 || keyPressed->code == Keyboard::Key::Numpad3)
//             {
//                 window.close();
//             }
//         }
//     }

//     window.clear();
//     window.draw(title);
//     window.draw(options);
//     window.display();
// }
// }