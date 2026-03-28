#pragma once
#include "Game.h"
#include "FieldRenderer.h"

struct ConsolePolicy {
    static void render(const Game& game) {
        FieldRenderer::print(
            game.getField(),
            game.getPlayer().getHp(),
            game.getTurns(),
            game.getPlayer().attackIsRange(),
            game.getPlayer().getMana()
        );
    }
};
