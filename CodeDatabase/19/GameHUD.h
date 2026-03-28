#pragma once

#include <vector>
#include "UIButton.h"

class Game;


class GameHUD {
public:
    explicit GameHUD(Game &game);

    std::vector<UIButton> get_buttons() const{
        return buttons_;
    }
    void handle_click(int x, int y) const;
    void reload_buttons();
private:
    Game& game_;
    std::vector<UIButton> buttons_;


    void render_overlay() const;
    void render_spell_preview() const;
};
