#include "ConsoleRenderer.h"
#include "GameModel.h"
#include <iostream>

void ConsoleRenderer::render(const GameModel &m) {
    std::cout << "=== GAME (lvl " << m.level() << ") ===\n";
    std::cout << "field " << m.width() << "x" << m.height() << "\n";
    std::cout << "player (" << m.playerX() << "," << m.playerY() << ") hp=" << m.playerHp() << "\n";

    auto h = m.hand();
    std::cout << "hand:";
    if (h.empty()) std::cout << " (empty)";
    for (size_t i = 0; i < h.size(); ++i) std::cout << " [" << i << ":" << h[i] << "]";
    std::cout << "\n";

    std::cout << "units:\n";
    for (const auto &u : m.units()) {
        std::string t;
        if (u.type == UnitType::Enemy) t = "Enemy";
        else if (u.type == UnitType::Tower) t = "Tower";
        else if (u.type == UnitType::Trap) t = "Trap";
        else t = "Other";
        std::cout << " - (" << u.x << "," << u.y << ") " << t << " hp=" << u.hp;
        if (u.type == UnitType::Tower) std::cout << " cd=" << u.cooldown;
        std::cout << "\n";
    }

    std::cout << "cmds: move X Y | attack X Y | use IDX X Y | save FILE | load FILE | next | quit\n";
}
