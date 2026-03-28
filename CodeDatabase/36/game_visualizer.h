#ifndef GAME_VISUALIZER_H
#define GAME_VISUALIZER_H

template <typename TRenderer>
class GameVisualizer {
public:
    // Конструктор создает объект отрисовщика
    GameVisualizer() : renderer_() {}

    // Реагирует на обновление состояния (в данном случае просто вызывает рендер)
    void Update(const World& world) {
        renderer_.Render(world);
    }

private:
    TRenderer renderer_;
};

#endif