#pragma once

enum class MoveDirection {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CAST_SPELL,
    OPEN_SHOP
    // QUIT можно не добавлять, мы обрабатываем его отдельно через quitRequested
};
