#include "GameSession.h"
#include <iostream>

int main() {
    try {
        GameSession session;
        session.run();
    } catch (const std::exception& e) {
        std::cerr << "Critical error in GameSession: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}