#include "input_source.h"
#include <iostream>
#include <termios.h>  // Для управления терминалом
#include <unistd.h>   // Для read()

class ConsoleInputSource : public IInputSource {
private:
    termios oldt, newt;
public:

    ConsoleInputSource() {
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        
        // Только отключаем ICANON (буферизацию), но оставляем ECHO
        newt.c_lflag &= ~ICANON;  // Только это, без ~ECHO
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    }

    ~ConsoleInputSource() {
        // Восстанавливаем настройки терминала
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }

    char getCharImmediate() {
        char ch = 0;
        
        // Считываем один символ без ожидания Enter
        ssize_t n = read(STDIN_FILENO, &ch, 1);
        
        if (n <= 0) {
            return 0;  // Ничего не ввели
        }
        
        return ch;
    }

    Direction getDirection() override {
        char input = getCharImmediate();

        if (input == 0) {
            return Direction::QUIT; 
        }

        switch(input) {
            case 'w': return Direction::UP;
            case 's': return Direction::DOWN;
            case 'a': return Direction::LEFT;
            case 'd': return Direction::RIGHT;
            case 'q': return Direction::QUIT;
            case 'c': return Direction::CHANGE;
            case 'f': return Direction::SPELL;
            case 'g': return Direction::GET_SPELL;
            case 'v': return Direction::SAVE;
            case 'l': return Direction::LOAD;
            case 'p': return Direction::PAUSE;
            default: 
                std::cout << "Invalid input! Use w, a, s, d, p, c, f, q\n";
                return getDirection();
        }
    }
    
    std::string getString() override {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        std::string s;
        std::cin >> s;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        return s;
    }
    
    char getChar() override {
        char c;
        std::cin >> c;
        return c;
    }
    
    int getInt() override {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        int i;
        std::cin >> i;
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        return i;
    }
};