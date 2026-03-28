#include "ViewController/Renderers/ConsoleRenderer.hpp"

void ConsoleRenderer::draw(GameDTO& dto) {
    if (dto.state == GameState::OnWalk || dto.state == GameState::OnCast) {
        std::vector<std::string> screen(dto.map.height, std::string(dto.map.width, '.'));
        // игрок
        int px = dto.player.coord.x;
        int py = dto.player.coord.y;
        
        if (py >= 0 && py < dto.map.height &&
            px >= 0 && px < dto.map.width) {
            screen[py][px] = 'X';
        }
        // вывод
        for (auto& row : screen) {
            std::cout << row << "\n";
        }
        std::cout << "HP: " << dto.player.hp << "/" << dto.player.maxHP << "\n";
        std::cout << "-----\n";
    }
}

bool ConsoleRenderer::isAlive() {
    return true;
}
