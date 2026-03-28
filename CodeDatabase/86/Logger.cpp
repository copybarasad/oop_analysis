#include "Logger.hpp"

namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string BOLD    = "\033[1m";
    const std::string DIM     = "\033[2m";
}


void Logger::header(int turn) {
    std::cout<< Color::CYAN << Color::BOLD << "\n========== TURN " << turn << " =========="<< Color::RESET<<std::endl;
}

void Logger::info(const std::string& msg) {
    std::cout << "[INFO] " << msg << "\n";
}

void Logger::action(const std::string& msg) {
    std::cout <<Color::YELLOW <<  "  ➤ " << msg <<Color::RESET <<  "\n";
}

void Logger::warning(const std::string& msg) {
    std::cout << Color::RED <<  "[!] " << msg <<Color::RESET <<  "\n";
}

void Logger::separator() {
    std::cout << "----------------------------\n";
}
