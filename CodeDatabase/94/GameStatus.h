#ifndef GAMESTATUS_H
#define GAMESTATUS_H

// Статус игры для меню и игровых состояний
enum class GameStatus {
    MENU,
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    QUIT
};

#endif