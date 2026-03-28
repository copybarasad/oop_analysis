#ifndef VISUALIZER_H
#define VISUALIZER_H

#include "gamemodel.h"
#include "observer.h"


template<typename TRenderer>
class Visualization : public Observer {
private:
    TRenderer* renderer;
    GameModel* gameModel;

public:
    Visualization(TRenderer* render = nullptr, GameModel* model = nullptr)
        : renderer(render), gameModel(model) {}


    void setRenderer(TRenderer* render) {
        renderer = render;
    }


    void setGameModel(GameModel* model) {
        gameModel = model;
        if (gameModel) {
            gameModel->add_observer(this);
        }
    }


    void draw() {
        if (renderer) {
            renderer->updating();
        }
    }


    void updating() override {
        draw();
    }
};

#endif // VISUALIZER_H
