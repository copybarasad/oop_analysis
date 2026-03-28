#pragma once
#include "GameObserver.h"
#include "Game.h"

template <class T_Renderer>
class GameVisualizer : public GameObserver {
private:
    T_Renderer renderer;
    Game* game_ref;

public:
    GameVisualizer() : game_ref(nullptr) {}
    
    void set_game(Game* g) { game_ref = g; }
    T_Renderer& get_renderer() { return renderer; }

    void on_log_message(const std::string& msg) override {
        renderer.print_log(msg);
    }

    void on_field_change() override {
        if(game_ref) {
           renderer.render_game(game_ref->get_field(), game_ref->get_player());
        }
    }
    
    void force_draw() {
        on_field_change();
    }

    void on_spells_display(const std::vector<std::string>& spells) override {
        renderer.print_spells(spells);
    }

    void on_shop_display(const std::vector<std::string>& items) override {
        renderer.print_shop(items);
    }
};
