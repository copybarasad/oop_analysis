#ifndef VISUALIZER_H
#define VISUALIZER_H

template<typename RendererT>
class Visualizer {
public:
    Visualizer() : r_() {}
    void show(const class GameModel &m) { r_.render(m); }
private:
    RendererT r_;
};

#endif // VISUALIZER_H
