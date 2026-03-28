#include "color_vis.h"
#include "game.h"
#include "field.h"
#include "node.h"

void ColorfulRenderer::render(const Game& game) {
    const Field& field = *game.getField();
    std::cout << PURPLE << "=== ЦВЕТНОЙ РЕЖИМ ===" << RESET << std::endl;
    
    for (int y = 0; y < field.getlength(); y++) {
        for (int x = 0; x < field.getwidth(); x++) {
            const Node& node = field.getNode(x, y);
            char symbol = '.';
            
            if (node.hasPlayer()) symbol = 'P';
            else if (node.hasEnemy()) symbol = 'E';
            else if (node.hasTower()) symbol = 'T';
            else if (node.hasTrap()) symbol = 'X';
            else if (node.hasObstacle()) symbol = '#';
            
            if (symbol == 'P') std::cout << GREEN << symbol << RESET;
            else if (symbol == 'E') std::cout << RED << symbol << RESET;
            else if (symbol == 'T') std::cout << PURPLE << symbol << RESET;
            else if (symbol == 'X') std::cout << YELLOW << symbol << RESET;
            else if (symbol == '#') std::cout << CYAN << symbol << RESET;
            else std::cout << BLUE << symbol << RESET;
            
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    
    std::cout << PURPLE << "=====================" << RESET << std::endl;
}

void ColorfulRenderer::renderMessage(const std::string& message) {
    std::cout << YELLOW << "💬 " << message << RESET << std::endl;
}

void ColorfulRenderer::showPrompt(const std::string& prompt) {
    std::cout << GREEN << "➤ " << prompt << RESET;
    std::cout.flush();
}