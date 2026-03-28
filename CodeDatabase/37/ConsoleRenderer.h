#ifndef CONSOLERENDERER_H
#define CONSOLERENDERER_H

#include "Renderer.h"

class ConsoleRenderer : public Renderer {
public:
    void render(const Game& game) override;
    void showMessage(const std::string& message) override;
    
private:
    void displayGameField(const Game& game);
    void displayStatus(const Game& game);
    void showControls();
};

#endif