#ifndef UNTITLED_GAMEVIEW_H
#define UNTITLED_GAMEVIEW_H

class GameModel;

class GameView {
public:
    GameView() = default;
    ~GameView() = default;
    void render(const GameModel& model) const;
};

#endif //UNTITLED_GAMEVIEW_H

