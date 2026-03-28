#include "TransitionToNewLevelScreen.h"
#include "../Program.h"
#include "GameScreen.h"
#include "MainMenuScreen.h"
#include <random>

#define UPGRADE_COST 30

// void TransitionToNewLevelScreen::render()
// {
// std::random_device rseed;
// std::mt19937 rng(rseed());
// if (mGame->getPlayer().get()->getHand()->getSize() < 1)
//     mGame->getPlayer().get()->getHand()->addSpell();
// else
// {
//     vector<int> nums(mGame->getPlayer().get()->getHand()->getSize());
//     for (size_t i = 0; i < nums.size(); i++)
//     {
//         nums[i] = i;
//     }
//     for (size_t i = 0; i < mGame->getPlayer().get()->getHand()->getSize() / 2; i++)
//     {
//         std::uniform_int_distribution<int> range(0, nums.size() - i - 1);
//         nums.erase(nums.begin() + range(rng));
//     }
//     for (size_t i = 0; i < nums.size(); i++)
//     {
//         mGame->getPlayer().get()->getHand()->deleteSpell(nums[i]);
//     }
// }

// Text title(mFont, format("{} --> {} \nCHOOSE UPGRADE", mGame->getLevel(), mGame->getLevel() + 1));
// title.setCharacterSize(60);
// title.setOrigin(Vector2f(0.5, 0.5));
// title.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 3.f));

// string optStr = format("Experience: {} \nCost: {} \n1. Resume without upgrade ", mGame->getPlayer().get()->getExp(), UPGRADE_COST);
// if (mGame->getPlayer().get()->getExp() >= UPGRADE_COST)
// {
//     optStr += format("\n2. Update life {} --> {} \n3. Update melee damage {} --> {} \n4. Update long-range damage {} --> {}\n",
//                      mGame->getPlayer().get()->getMaxLife(), (int)(mGame->getPlayer().get()->getMaxLife() * 1.2) + 1,
//                      mGame->getPlayer().get()->getMeleeDamage(), (int)(mGame->getPlayer().get()->getMeleeDamage() * 1.2) + 1,
//                      mGame->getPlayer().get()->getLongRangeCombatDamage(), (int)(mGame->getPlayer().get()->getLongRangeCombatDamage() * 1.2) + 1);

//     for (size_t i = 0; i < mGame->getPlayer().get()->getHand()->getSize(); i++)
//     {
//         optStr += format("{}. {} (lvl {} --> {})\n", i + 5, mGame->getPlayer().get()->getHand()->getSpell(i)->getName(),
//                          mGame->getPlayer().get()->getHand()->getSpell(i)->getLevel(),
//                          mGame->getPlayer().get()->getHand()->getSpell(i)->getLevel() + 1);
//     }
// }

// Text options(mFont, optStr);
// options.setCharacterSize(30);
// options.setOrigin(Vector2f(0.5, 0.5));
// options.setPosition(Vector2f(window.getSize().x / 2.f, window.getSize().y / 3.f + 130));
// while (window.isOpen())
// {
//     while (const std::optional event = window.pollEvent())
//     {
//         if (event->is<sf::Event::Closed>())
//             window.close();
//         else if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
//         {
//             if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
//             {
//                 mGame->changeScreen(new MainMenuScreen);
//                 return;
//             }
//         }

//         if (const auto *keyPressed = event->getIf<sf::Event::KeyReleased>())
//         {
//             if (keyPressed->code == Keyboard::Key::Num1 || keyPressed->code == Keyboard::Key::Numpad1)
//             {
//                 mGame->setNewLevel();
//                 mGame->changeScreen(new GameScreen);
//                 return;
//             }
//             if (mGame->getPlayer().get()->getExp() >= UPGRADE_COST)
//             {
//                 if (keyPressed->code == Keyboard::Key::Num2 || keyPressed->code == Keyboard::Key::Numpad2)
//                 {
//                     mGame->getPlayer().get()->setMaxLife(mGame->getPlayer().get()->getMaxLife() * 1.2 + 1);
//                     mGame->getPlayer().get()->subExp(UPGRADE_COST);
//                     mGame->setNewLevel();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//                 if (keyPressed->code == Keyboard::Key::Num3 || keyPressed->code == Keyboard::Key::Numpad3)
//                 {
//                     mGame->getPlayer().get()->setMeleeDamage(mGame->getPlayer().get()->getMeleeDamage() * 1.2 + 1);
//                     mGame->getPlayer().get()->subExp(UPGRADE_COST);
//                     mGame->setNewLevel();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//                 if (keyPressed->code == Keyboard::Key::Num4 || keyPressed->code == Keyboard::Key::Numpad4)
//                 {
//                     mGame->getPlayer().get()->setLongRangeCombatDamage(mGame->getPlayer().get()->getLongRangeCombatDamage() * 1.2 + 1);
//                     mGame->getPlayer().get()->subExp(UPGRADE_COST);
//                     mGame->setNewLevel();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//                 if (keyPressed->code > Keyboard::Key::Num4 && (int)keyPressed->code <= (int)Keyboard::Key::Num4 + mGame->getPlayer().get()->getHand()->getSize())
//                 {
//                     mGame->getPlayer().get()->getHand()->getSpell((int)keyPressed->code - 31)->update();
//                     mGame->getPlayer().get()->subExp(UPGRADE_COST);
//                     mGame->setNewLevel();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//                 if (keyPressed->code > Keyboard::Key::Numpad4 && (int)keyPressed->code <= (int)Keyboard::Key::Numpad4 + mGame->getPlayer().get()->getHand()->getSize())
//                 {
//                     mGame->getPlayer().get()->getHand()->getSpell((int)keyPressed->code - 80)->update();
//                     mGame->getPlayer().get()->subExp(UPGRADE_COST);
//                     mGame->setNewLevel();
//                     mGame->changeScreen(new GameScreen);
//                     return;
//                 }
//             }
//         }
//     }

//     window.clear();
//     window.draw(title);
//     window.draw(options);
//     window.display();
// }
// }