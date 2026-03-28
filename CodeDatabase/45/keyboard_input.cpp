#include "keyboard_input.hpp"

static void setNonCanonicalMode() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

static void restoreTerminalMode() {
    termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &t);
}

Command KeyboardInput::get_string() {
    restoreTerminalMode();
    std::string str;
    std::getline(std::cin, str);
    Command com(str);
    return com;

}

Command KeyboardInput::get_char() {
    setNonCanonicalMode();
    char key;
    ssize_t bytesRead = read(STDIN_FILENO, &key, 1);
    (void)bytesRead; // Чтобы избежать warning о неиспользуемой переменной
    Command com(std::string(1, key));  // Преобразовать char в string
    return com;
}