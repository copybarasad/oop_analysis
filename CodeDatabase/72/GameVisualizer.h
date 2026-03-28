#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Field;

template<typename RendererType>
class GameVisualizer {
    std::unique_ptr<RendererType> renderer;

public:

    explicit GameVisualizer(std::unique_ptr<RendererType> renderer)
        : renderer(std::move(renderer)) {}

    explicit GameVisualizer(const RendererType& renderer)
        : renderer(std::make_unique<RendererType>(renderer)) {}

    void render(const Field& field) {
        if (renderer) {
            renderer->drawField(field);
        }
    }

    void renderGameOver(const Field& field, bool won = false) {
        render(field);
        if (renderer) {
            renderer->drawGameOver(won);
        }
    }

    void renderMainMenu(const std::vector<std::string>& menuItems,
                        int selectedIndex = 0) {
        if (renderer) {
            renderer->drawMainMenu(menuItems, selectedIndex);
        }
    }

    void renderLoadMenu(const std::vector<std::string>& saveFiles,
                       int selectedIndex = 0) {
        if (renderer) {
            renderer->drawLoadMenu(saveFiles, selectedIndex);
        }
    }

    void renderSaveMessage(const std::string& filename) {
        if (renderer) {
            renderer->drawSaveMessage(filename);
        }
    }

    void renderMessage(const std::string& title, const std::string& message) {
        if (renderer) {
            renderer->drawMessage(title, message);
        }
    }

};

#endif // GAME_VISUALIZER_H