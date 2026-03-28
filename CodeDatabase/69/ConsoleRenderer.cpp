#include "ConsoleRenderer.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <iomanip>

void ConsoleRenderer::Render(const Game& game) const {
    const Player* player = game.GetPlayer();
    if (!player) return;

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════╗\n";
    std::cout << "║           GAME FIELD             ║\n";
    std::cout << "╚══════════════════════════════════╝\n\n";

    int width = game.GetWidth();
    int height = game.GetHeight();

    std::cout << "   ";
    for (int x = 0; x < width; ++x) {
        std::cout << ' ' << (x % 10) << ' ';
    }
    std::cout << '\n';

    for (int y = 0; y < height; ++y)
    {
        std::cout << std::setw(2) << (y % 10) << " ";

        for (int x = 0; x < width; ++x)
        {
            Position p{x, y};
            char ch = '.';

            if (player->GetPosition() == p)
                ch = 'P';
            else
            {
                for (auto const& t : game.GetTowers()) {
                    if (t->IsAlive() && t->GetPosition() == p) { ch = 'T'; break; }
                }
                for (auto const& e : game.GetEnemies()) {
                    if (e->IsAlive() && e->GetPosition() == p) { ch = 'E'; break; }
                }
                for (auto const& a : game.GetAllies()) {
                    if (a->IsAlive() && a->GetPosition() == p) { ch = 'A'; break; }
                }
                for (auto const& tr : game.GetTraps()) {
                    if (tr->active && tr->pos == p) { ch = '^'; break; }
                }
            }

            std::cout << ' ' << ch << ' ';
        }
        std::cout << '\n';
    }

    std::cout << "\n══════════════════════════════════════\n";
    std::cout << " PLAYER STATUS\n";
    std::cout << " - HP:    " << player->GetHp() << "\n";
    std::cout << " - Coins: " << player->GetCoins() << "\n";

    std::cout << "\n  ENEMIES:\n";
    for (auto const& e : game.GetEnemies()) {
        if (e->IsAlive()) {
            std::cout << " - " << e->GetId() 
                      << " at (" << e->GetPosition().x << "," << e->GetPosition().y << ")" 
                      << " HP=" << e->GetHp() << "\n";
        }
    }

    std::cout << "\n TOWERS:\n";
    for (auto const& t : game.GetTowers()) {
        if (t->IsAlive()) {
            std::cout << " - " << t->GetId() 
                      << " at (" << t->GetPosition().x << "," << t->GetPosition().y << ")" 
                      << " HP=" << t->GetHp() 
                      << " R=" << t->GetRange() << "\n";
        }
    }

    const auto& traps = game.GetTraps();
    if (!traps.empty()) {
        std::cout << "\n TRAPS:\n";
        for (auto const& tr : traps) {
            if (tr->active) {
                std::cout << " - " << tr->name 
                          << " at (" << tr->pos.x << "," << tr->pos.y << ") dmg=" << tr->damage << "\n";
            }
        }
    }

    std::cout << "══════════════════════════════════════\n\n";
    
    auto names = player->GetHand().GetSpellNames();
    std::cout << "Hand (" << player->GetHand().Size() << "/" << player->GetHand().Capacity() << "):\n";
    for (size_t i=0;i<names.size();++i)
    {
        std::cout << "  " << (i+1) << ") " << names[i] << "\n";
    }
}

void ConsoleRenderer::ShowGameOverPrompt() const {
    std::cout << "You died.\nRestart from level 1? (y/n): ";
}

void ConsoleRenderer::ShowLevelClearMessage(int level) const {
    std::cout << "Level " << level << " cleared!\n";
}

void ConsoleRenderer::ShowVictoryMessage() const {
    std::cout << "You beat the game! Congrats.\n";
}

void ConsoleRenderer::ShowUpgradeMenu() const {
    std::cout << "Choose upgrade:\n";
    std::cout << " 1) Heal +10 HP\n";
    std::cout << " 2) +10 coins\n";
    std::cout << " 3) Add random spell to hand\n";
}

void ConsoleRenderer::ShowExitMessage() const {
    std::cout << "Exiting.\n";
}

void ConsoleRenderer::ShowMainMenu() const {
    std::cout << "=== MAIN MENU ===\n";
    std::cout << "1) New Game\n2) Load Game\n3) Exit\nChoose: ";
}

void ConsoleRenderer::ShowEnterFilenamePrompt() const {
    std::cout << "Enter save filename: ";
}

void ConsoleRenderer::ShowLoadSuccessMessage() const {
    std::cout << "Loaded.\n";
}

void ConsoleRenderer::ShowLoadErrorMessage(const std::string& error) const {
    std::cout << "Load error: " << error << "\n";
}

void ConsoleRenderer::ShowInvalidChoiceMessage() const {
    std::cout << "Invalid choice.\n";
}

void ConsoleRenderer::ShowActionMessage(const std::string& msg) const {
    if (!msg.empty()) {
        std::cout << msg << "\n";
    }
}

