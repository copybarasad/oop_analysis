#ifndef GAME_SAVER_H
#define GAME_SAVER_H

#include <string>
#include <memory>
#include <cstdint>

namespace Game {

class Game;
class Player;

class GameSaver {
public:
    static void save(const Game& gameState, const Player& player, const std::string& filename);
    static std::unique_ptr<Game> load(Player& player, const std::string& filename);

private:
    static inline const uint32_t MAGIC_NUMBER = 0xDEADBEEF;
    static inline const uint32_t SAVE_VERSION = 1;
};

}

#endif