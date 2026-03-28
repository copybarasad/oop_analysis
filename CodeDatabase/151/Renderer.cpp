// Renderer.cpp
#include "Renderer.h"
#include "GameField.h"
#include "Player.h"
#include <iostream>

void ConsoleRenderer::render(const GameField& field, const Player& player) const {
    field.display();
    std::cout << "\nPlayer - Health: " << player.getHealth()
        << " | Score: " << player.getScore()
        << " | Mana: " << player.getMana() << "\n";
    std::cout << "--- YOUR SPELLS ---\n";
    auto& hand = const_cast<Player&>(player).getHand();
    for (int i = 0; i < hand.getSize(); ++i) {
        if (auto* spell = hand.getSpell(i)) {
            std::cout << i + 1 << ". " << spell->getName() << " (Cost: " << spell->getManaCost() << ")\n";
        }
    }
    std::cout << "Mana: " << player.getMana() << "/100\n";
}