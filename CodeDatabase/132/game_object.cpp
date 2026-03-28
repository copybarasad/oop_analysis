#include "game_object.hpp"

// namespace screen_symbol {
//     const char PLAYER = 'P';
//     const char ENEMY = 'E';
//     const char ENEMY_BUILDING = 'B';
//     const char STONE = '#';
//     const char SWAMP = '~';
//     const char EMPTY = '.';
// }

GameObject::GameObject(char screen_symbol): screen_symbol(screen_symbol) {};

char GameObject::get_screen_symbol() const {
    return screen_symbol;
}