#include "ConsoleRenderer.h"
#include "Game.h"
#include "Player.h"
#include "GameField.h"
#include "EntityManager.h"
#include "Cell.h"
#include <iostream>
#include <algorithm>

void ConsoleRenderer::showGame(std::shared_ptr<Game>& game, const std::string& message) {
    clear();
    int viewWidth = 9, viewHeight = 9;
    auto& field = game->getGameField();
    std::cout << "Уровень " << game->getLevel() << " (Размеры поля " << field.getWidth() << "x" << field.getHeight() << ")" << std::endl;

    std::pair<int,int> mark = {-1, -1};
    int markDiameter = 1;
    std::pair<int, int> area = {-1, -1};
    int areaDiameter = 1;

    auto& player = game->getPlayer();
    auto playerPosition = player.getPosition();
    auto [px, py] = playerPosition;
    auto mode = player.getMode();
    if (mode == PlayerModes::RANGE) {
        mark = player.getTarget();
        area = player.getPosition();
        areaDiameter = player.getShootRange();
    } else if (mode == PlayerModes::SPELL) {
        auto* spell = player.getSpellsHand().getSelectedSpell();
        mark = player.getTarget();
        markDiameter = spell->visualData().first;
        area = player.getPosition();
        areaDiameter = spell->visualData().second;
    }

    std::cout << "┌";
    for (int i = 0; i < viewWidth; i++)
        std::cout << "──";
    std::cout << "┐" << std::endl;

    for (int vy = 0; vy < viewHeight; ++vy) {
        std::cout << "│";
        for (int vx = 0; vx < viewWidth; ++vx) {
            auto [x, y] = player.getPosition();
            x +=  vx - viewWidth / 2; y += vy - viewHeight / 2;
            if (!field.isInBounds({x,y})) {
                std::cout << "  ";
                continue;
            }
            std::cout << "\033[";
            if (field.areCoordinatesInArea({x, y}, area, areaDiameter))
                std::cout << ";41";
            if (field.areCoordinatesInArea({x, y}, mark, markDiameter))
                std::cout << ";101";
            if (std::max(std::abs(px - x), std::abs(py - y)) > player.getViewDiameter() / 2) {
                std::cout << "m  ";
                continue;
            }
            char symbol = '.';
            const Cell& cell = field.getCell({x, y});
            Entity* entity = nullptr;
            if (cell.getPassability() == CellType::WALL)
                symbol = '#';
            else if (cell.getPassability() == CellType::SLOW)
                symbol = '~';
            if (cell.isOccupied()) {
                entity = game->getEntityManager().findEntity({x, y});
                symbol = entity->getSymbol();
            }
            if (cell.getPoints())
                std::cout << ";33";
            if (entity) {
                if (entity->isSlowed())
                    std::cout << ";44";
                if (entity->getTeam() == EntityTeam::ALLY)
                    std::cout << ";36";
                else
                    std::cout << ";31";
            }
            std::cout << "m" << symbol << "\033[m ";
        }
        std::cout << "│" << std::endl;
    }

    std::cout << "└";
    for (int i = 0; i < viewWidth; i++)
        std::cout << "──";
    std::cout << "┘" << std::endl;
    if (!message.empty())
        std::cout << message;
    std::cout << "Здоровье игрока: " << player.getHealth();
    std::cout << "; Количество очков: \033[33m" << player.getPoints() << "\033[m" << std::endl;
    auto& hand = player.getSpellsHand();
    auto handSize = hand.getSpells().size();
    auto handMax = hand.getMaxSize();
    std::cout << "Заполненность руки заклинаний: " << handSize << "/" << handMax << std::endl;
    switch (mode) {
        case PlayerModes::SPELL:
            std::cout << "Выбранное заклинание: " << hand.getSelectedSpell()->getName() << std::endl;
            break;
        case PlayerModes::RANGE:
        case PlayerModes::MELEE:
            std::cout << "Режим атаки: " << (mode == PlayerModes::MELEE ? "ближний" : "дальний");
            std::cout << " (урон: " << player.getDamage() << ")" << std::endl;
        default:
            break;
    }
    std::cout << player.getSpellsInfo() << std::endl;
}

void ConsoleRenderer::clear() {
    system("clear");
}

void ConsoleRenderer::showMessage(const std::string& message) {
    clear();
    std::cout << message << std::endl;
}
