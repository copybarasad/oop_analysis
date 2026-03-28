#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

class GameController;

class ConsoleRenderer {
public:
    void render(const GameController& game);
};

#endif