#include "screen.hpp"

namespace Colors {
    const std::string WHIGHT = "\033[37m";
    const std::string RESET = "\033[0m";
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE = "\033[34m";
    const std::string CYAN = "\033[36m";
}

void Screen::clearConsole() {
    // std::cout << "\033[2J\033[1;1H";
}

std::string Screen::getColorForSymbol(char symbol) {
    switch (symbol) {
        case 'P': return Colors::GREEN;
        case 'E': return Colors::RED;
        case 'B': return Colors::YELLOW;
        case '#': return Colors::WHIGHT;
        case '~': return Colors::BLUE;
        case '.': return Colors::WHIGHT; 
        default:  return Colors::RESET;
    }
}

void Screen::frame(const Field& field) {
    clearConsole();
    
    for (int i = 0; i < field.height(); i++) {
        std::string line;
        
        for (int j = 0; j < field.width(); j++) {
            GameObject* obj = field[{i, j}];

            if (obj != nullptr) {
                char symbol = obj->get_screen_symbol();
                std::string color = getColorForSymbol(symbol);
                line += color + symbol;
            } else {
                line += Colors::WHIGHT + '.';
            }

            line += ' ' + Colors::RESET;
        }
        
        std::cout << line << std::endl;
    }
    
    std::cout << std::flush;
}