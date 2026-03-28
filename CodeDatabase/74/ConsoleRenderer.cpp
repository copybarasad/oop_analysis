#include "ConsoleRenderer.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

ConsoleRenderer::ConsoleRenderer() {}

void ConsoleRenderer::clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void ConsoleRenderer::render(const Game& game) {
    const Field& field = game.getField();
    const Player& p = game.getPlayer();
    const std::vector<Enemy>& enemies = game.getEnemies();
    const std::string& message = game.getCurrentMessage();
    bool aimActive = game.isAimActive();
    Coords aimPos = game.getAimPos();
    bool splashAim = game.isSplashAim();
    int radius = game.getRadius();
    bool trapAim = game.isTrapAim();
    const Hand* hand = game.getShowHand() ? &game.getHand() : nullptr;

    clearConsole();
    int width = field.getWidth();
    int height = field.getHeight();

    // === STATUS BAR ===
    int aliveEnemies = 0;
    for (const auto& e : enemies) if (e.isAlive()) ++aliveEnemies;

    std::cout << "HP: " << p.getHealth()
        << " | Attack: " << (p.getAttackMode() == AttackMode::Melee ? "Melee" : "Ranged")
        << " | Enemies: " << aliveEnemies << "\n";

    if (!message.empty())
        std::cout << message << "\n\n";
    else
        std::cout << "\n";

    // === TOP BORDER ===
    for (int i = 0; i < width + 2; ++i) std::cout << '#';
    std::cout << "\n";

    // === FIELD ===
    for (int y = 0; y < height; ++y) {
        std::cout << '#';
        for (int x = 0; x < width; ++x) {

            // Player
            if (p.getCoords().x == x && p.getCoords().y == y)
                std::cout << 'P';

            // Splash aim
            else if (aimActive && splashAim &&
                std::abs(x - aimPos.x) <= radius && std::abs(y - aimPos.y) <= radius)
                std::cout << '^';

            // Trap aim cursor
            else if (aimActive && trapAim && aimPos.x == x && aimPos.y == y)
                std::cout << '@';

            // Ranged aim cursor
            else if (aimActive && !splashAim && !trapAim && aimPos.x == x && aimPos.y == y)
                std::cout << '+';

            // Enemy
            else {
                bool printed = false;
                for (const auto& e : enemies) {
                    if (e.isAlive() && e.getCoords().x == x && e.getCoords().y == y) {
                        std::cout << 'E';
                        printed = true;
                        break;
                    }
                }

                // Towers
                if (!printed && field.hasTowerAt(x, y)) {
                    std::cout << 'T';
                    printed = true;
                }

                // Real traps on field
                if (!printed && field.hasTrapAt(x, y)) {
                    std::cout << '@';
                    printed = true;
                }

                // Floor or wall
                if (!printed)
                    std::cout << (field.isCellWalkable(x, y) ? '.' : '#');
            }
            
        }
        std::cout << "#\n";
    }
    // === BOTTOM BORDER ===
    for (int i = 0; i < width + 2; ++i) std::cout << '#';
    std::cout << "\n\n";

    // === OPTIONAL HAND DISPLAY ===
    if (game.getShowHand()) {
        int count = hand->getCount();
        if (count == 0) {
            std::cout << "[Hand empty]\n\n";
        }
        else {
            std::cout << "=== Hand ===\n";
            for (int i = 0; i < count; ++i) {
                CastInfo info = hand->getSpell(i)->getCastInfo();
                std::cout << i << ": ";

                switch (info.type) {
                case SpellType::Damage: std::cout << "Damage"; break;
                case SpellType::Splash: std::cout << "Splash"; break;
                case SpellType::Trap:   std::cout << "Trap"; break;
                }

                std::cout << " (" << info.damage << " dmg";
                if (info.type == SpellType::Splash)
                    std::cout << ", r=" << info.radius;
                if (info.type == SpellType::Damage || info.type == SpellType::Trap)
                    std::cout << ", dist=" << info.distance;
                std::cout << ")\n";
            }
            std::cout << "=============\n\n";
        }
    }

    // === CONTROLS ===
    std::cout << "[W|A|S|D] Move | [E] Attack | [Q] Switch Mode | [R] Hand | [Y] Save | [Ctrl+C] Exit\n";
}
