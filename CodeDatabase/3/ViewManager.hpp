#include "GameClass/GameClass.hpp"

template<typename Renderer>
class ViewManager {
public:
    template<typename ... Args>
    ViewManager(GameClass& game, Args&& ... args)
        : game(game), renderer(std::forward<Args>(args)...) {}

    void render() {
        GameDTO dto = game.toDTO();
        renderer.draw(dto);
    }

    bool isAlive() {
        return renderer.isAlive();
    }

private:
    GameClass& game;
    Renderer renderer;
};
