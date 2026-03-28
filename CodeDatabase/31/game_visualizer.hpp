#ifndef GAME_VISUALIZER_HPP
#define GAME_VISUALIZER_HPP

template<typename DrawerType>
class GameVisualizer {
    DrawerType drawer;

public:
    GameVisualizer(DrawerType drawer) : drawer(drawer) {}

    void draw_field() {
        drawer.draw_field();
    }

    void draw_score() {
        drawer.draw_score();
    }

    void draw_info() {
        drawer.print_info();
    }

    void draw_pricelist() {
        drawer.print_pricelist();
    }

    void draw_all() {
        drawer.draw_field();
        drawer.draw_score();
    }
};

#endif