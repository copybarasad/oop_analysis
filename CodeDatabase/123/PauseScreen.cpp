#include "PauseScreen.h"
#include "GameScreen.h"
#include "MainMenuScreen.h"
#include "../Program.h"

// void PauseScreen::render()
// {
// Text title(mFont, "Pause");
// title.setCharacterSize(60);
// title.setOrigin(Vector2f(0.5, 0.5));
// title.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 3.f));

// Text options(mFont, "1. Resume game \n2. Save game \n3. Load game \n4. Exit to main menu");
// options.setCharacterSize(30);
// options.setOrigin(Vector2f(0.5, 0.5));
// options.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 3.f + 60));
// while (window.isOpen())
// {
//     while (const std::optional event = window.pollEvent())
//     {
//         if (event->is<sf::Event::Closed>())
//             window.close();

//         if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
//         {
//             if (keyPressed->code == Keyboard::Key::Num1 || keyPressed->code == Keyboard::Key::Numpad1 || keyPressed->code == Keyboard::Key::Escape)
//             {
//                 mGame->changeScreen(new GameScreen);
//                 return;
//             }
//             try
//             {
//                 if (keyPressed->code == Keyboard::Key::Num2 || keyPressed->code == Keyboard::Key::Numpad2)
//                 {
//                     mGame->saveGame();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//                 if (keyPressed->code == Keyboard::Key::Num3 || keyPressed->code == Keyboard::Key::Numpad3)
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
//             catch (exception &e)
//             {
//                 std::cerr << e.what() << '\n';
//             }

//             if (keyPressed->code == Keyboard::Key::Num4 || keyPressed->code == Keyboard::Key::Numpad4)
//             {
//                 mGame->changeScreen(new MainMenuScreen);
//                 return;
//             }
//         }
//     }

//     window.clear();
//     window.draw(title);
//     window.draw(options);
//     window.display();
// }
// }