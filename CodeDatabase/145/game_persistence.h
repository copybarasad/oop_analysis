#ifndef GAME_PERSISTENCE_H
#define GAME_PERSISTENCE_H

#include <iosfwd>
#include <string>

class Game;

class GamePersistence {
public:
    static void saveToStream(const Game& game, std::ostream& out);
    static void loadFromStream(Game& game, std::istream& in);
};

#endif