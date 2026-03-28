#include "console_renderer.h"
#include "game.h"
#include "field.h"
#include "cell.h"
#include <iostream>

void ConsoleRenderer::render(const Game& game) {
    const Field* field = game.getField();

    if (!field) {
        std::cout << "Игра не инициализирована!" << std::endl;
        return;
    }
    
    if (field->getLength() <= 0 || field->getWidth() <= 0) {
        std::cout << "Некорректный размер поля!" << std::endl;
        return;
    }
    
    std::cout << "\nИГРОВОЕ ПОЛЕ" << std::endl;
    std::cout << "Уровень: " << game.getCurrentLevel() << std::endl;
    
    for (int y = 0; y < field->getLength(); y++) {
        for (int x = 0; x < field->getWidth(); x++) {
            const Cell& cell = field->getCell(x, y);
            char symbol = '.';
            
            switch (cell.getType()) {
                case Type::PLAYER: symbol = 'P'; break;
                case Type::ENEMY: symbol = 'E'; break;
                case Type::OBSTACLE: symbol = '#'; break;
                case Type::TRAP: symbol = 'T'; break;
                case Type::DAMAGE_TRAP: symbol = 'X'; break;
                case Type::TOWER: symbol = 'B'; break;
                case Type::DAMAGE_TOWER: symbol = 'D'; break;
                default: symbol = '.'; break;
            }
            
            std::cout << symbol << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nЛегенда: P-игрок, E-враг, #-препятствие, T-ловушка, X-ловушка урона, B-башня, D-башня урона" << std::endl;
}

void ConsoleRenderer::showMessage(const std::string& message) {
    std::cout << ">> " << message << std::endl;
}