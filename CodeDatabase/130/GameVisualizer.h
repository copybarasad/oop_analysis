#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

template <typename GameT, typename RenderStrategyT>
class GameVisualizer {
public:
    GameVisualizer(GameT& game, RenderStrategyT renderer) 
        : game_(game), renderer_(renderer) {}

    void draw() {
        renderer_.render(game_);
    }

    RenderStrategyT& getRenderer() {
        return renderer_;
    }

private:
    GameT& game_;
    RenderStrategyT renderer_;
};

#endif
