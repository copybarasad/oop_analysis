#include "Game.h"
#include "GameExceptions.h"
#include <iostream>

int main() {
    try {
        Game game;
        game.run();
        std::cout << "=== Game finished successfully ===\n";
        return 0;
        
    } catch (const InitializationException& e) {
        std::cout << "Fatal Initialization Error: " << e.what() << "\n";
        std::cout << "Game cannot start. Please check your installation.\n";
        return 1;
    } catch (const FileException& e) {
        std::cout << "File Error: " << e.what() << "\n";
        return 1;
    } catch (const GameException& e) {
        std::cout << "Game Error: " << e.what() << "\n";
        return 1;
    } catch (const std::bad_alloc& e) {
        std::cout << "Memory Error: Not enough memory to run the game\n";
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Unexpected Error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cout << "Unknown Fatal Error: Something went terribly wrong\n";
        return 1;
    }
}