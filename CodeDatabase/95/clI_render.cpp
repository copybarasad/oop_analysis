#include "cli_render.hpp"
#include "game_world/map.hpp"
#include "game/entity_manager.hpp"

void CLIRenderer::draw(eManager& eManager) {
    const Map* map = eManager.getMap();

    int w = map->getWidth();
    int h = map->getHeight();

    std::vector<std::vector<char>> grid(h, std::vector<char>(w, '\0'));

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            TileType type = map->getTile(x, y).getType();
            switch(type) {
                case TileType::Grass: grid[y][x] = static_cast<char>(Symbols::Grass); break;
                case TileType::Wall:  grid[y][x] = static_cast<char>(Symbols::Wall);  break;
                case TileType::Trap:  grid[y][x] = static_cast<char>(Symbols::Trap);  break;
            }
        }
    }

    Position pos;

    auto p = eManager.getPlayer();
    pos = p->getPos();
    if ((p->isAlive()) && !(pos.x < 0 || pos.x >= w || pos.y < 0 || pos.y >= h)) {
        grid[pos.y][pos.x] = static_cast<char>(Symbols::Player);
    }

    auto t = eManager.getTower();
    pos = t->getPos();
    if ((t->isAlive()) && !(pos.x < 0 || pos.x >= w || pos.y < 0 || pos.y >= h)) {
        grid[pos.y][pos.x] = static_cast<char>(Symbols::Tower);
    }

    for (auto& e : eManager.getEnemies()) {
        if (!e->isAlive()) continue;

        pos = e->getPos();
        if (pos.x < 0 || pos.x >= w || pos.y < 0 || pos.y >= h) continue;

        grid[pos.y][pos.x] = static_cast<char>(Symbols::Enemy);
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            std::cout << grid[y][x] << ' ';
        }
        std::cout << '\n';
    }
}

void CLIRenderer::renderMainMenu() {
    std::cout << "\nГЛАВНОЕ МЕНЮ\n";
    std::cout << "1. Новая игра\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Выйти\n";
}

void CLIRenderer::renderPauseMenu() {
    std::cout << "\nМЕНЮ ПАУЗЫ\n";
    std::cout << "1. Продолжить игру\n";
    std::cout << "2. Загрузить игру\n";
    std::cout << "3. Сохранить игру\n";
    std::cout << "4. Сохранить и выйти в меню\n";
    std::cout << "5. Выйти без сохранения\n";
}

void CLIRenderer::renderSaveMenu(const std::vector<std::string>& saves) {
    std::cout << "\nВЫБЕРИТЕ СОХРАНЕНИЕ\n";
    for (size_t i = 0; i < saves.size(); ++i) {
        std::cout << i + 1 << ". " << saves[i] << "\n";
    }
    std::cout << "b. Назад\n";
}

void CLIRenderer::renderSaveNamePrompt() {
    std::cout << "Введите название сохранения: ";
}

void CLIRenderer::renderSaveSuccess() {
    std::cout << "Игра успешно сохранена!" << "\n";
}

void CLIRenderer::renderNextLevel(int level) {
    std::cout << "Переход на уровень " << level << "!\n";
}

void CLIRenderer::renderMessage(const std::string& message) {
    std::cout << message << "\n";
}

void CLIRenderer::renderError(const std::string& error) {
    std::cout << error << "\n";
}