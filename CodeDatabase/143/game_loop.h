#pragma once

#include "game_controller.h"
#include "console_input_reader.h"
#include "console_render.h"
#include "game_view.h"

template<typename InputReader = ConsoleInputReader>
class GameLoop {
public:
    explicit GameLoop(GameController& ctrl)
        : controller(ctrl),
        inputReader(ctrl),
        view(ctrl) {
    }

    void run() {
        ConsoleRenderer renderer;
        renderer.renderMainMenu();

        char choice;
        std::cin >> choice;
        inputReader.clearBuffer();

        if (choice == 'N' || choice == 'n') {
            controller.startNewGame();
        }
        else if (choice == 'L' || choice == 'l') {
            if (!controller.loadGame()) {
                std::cout << "No save found. Starting new game.\n";
                controller.startNewGame();
            }
        }
        else {
            return;
        }

        bool quitRequested = false;
        while (controller.isGameRunning() && !quitRequested) {
            view.update(); // отрисовка

            MoveDirection dir = inputReader.getMoveInput(quitRequested);
            if (quitRequested) {
                renderer.renderQuitPrompt();
                char confirm;
                std::cin >> confirm;
                if (confirm == 'y' || confirm == 'Y' || confirm == 's' || confirm == 'S') {
                    if (confirm == 's' || confirm == 'S') controller.saveGame();
                    break;
                }
                quitRequested = false;
                continue;
            }

            switch (dir) {
            case MoveDirection::UP:
            case MoveDirection::DOWN:
            case MoveDirection::LEFT:
            case MoveDirection::RIGHT:
                controller.processPlayerMove(dir);
                break;
            case MoveDirection::CAST_SPELL:
                handleCastSpell();
                break;
            case MoveDirection::OPEN_SHOP:
                handleShop();
                break;
            }

            controller.updateGame();

            if (!controller.isGameRunning()) {
                view.update();
                renderer.renderGameOver(controller);
                char ch;
                std::cin >> ch;
                if (ch == 'r') controller.resetGame();
                else if (ch == 'l') controller.loadGame();
                else break;
            }
        }
    }

private:
    void handleCastSpell() {
        const auto& hand = controller.getPlayer().getHand();
        if (hand.isEmpty()) {
            std::cout << "No spells available!\n";
            return;
        }

        ConsoleRenderer renderer;
        renderer.renderSpells(controller);
        int index = inputReader.chooseSpellIndex(static_cast<int>(hand.getSize()));
        if (index == -1) {
            std::cout << "Cancelled.\n";
            return;
        }

        SpellCard* spell = hand.getSpell(index);
        if (spell->isUsed()) {
            std::cout << "Spell already used!\n";
            return;
        }

        std::vector<Position> targets;
        if (auto* direct = dynamic_cast<DirectDamageSpell*>(spell)) {
            targets = direct->getValidTargets();
        }
        else if (auto* area = dynamic_cast<AreaDamageSpell*>(spell)) {
            targets = area->getValidTargets();
        }

        if (targets.empty()) {
            std::cout << "No valid targets!\n";
            return;
        }

        renderer.renderTargetHelp(controller, targets, dynamic_cast<AreaDamageSpell*>(spell) != nullptr);
        Position target = inputReader.chooseTarget();

        if (controller.castSpell(index, target)) {
            std::cout << spell->getName() << " cast successfully!\n";
        }
        else {
            std::cout << "Spell failed!\n";
        }
    }

    void handleShop() {
        ConsoleRenderer renderer;
        renderer.renderShop(controller);
        int index = inputReader.chooseShopItem(static_cast<int>(controller.getSpellShop().getAvailableSpells().size()));
        if (index == -1) {
            std::cout << "Shop closed.\n";
            return;
        }

        if (controller.buySpell(index)) {
            std::cout << "Spell purchased!\n";
        }
        else {
            std::cout << "Cannot buy!\n";
        }
    }

    GameController& controller;
    InputReader inputReader;
    GameView<ConsoleRenderer> view;
};
