#include "GameHUD.h"

#include "../../Game.h"
#include "spell/Spell.h"
#include "spell/SpellHolder.h"
#include "upgrade/PlayerUpgradeRegistry.h"


void GameHUD::handle_click(int x, int y) const {
    for(auto& button : buttons_){
        button.handle_click(x, y);
    }

}

GameHUD::GameHUD(Game &game)
        : game_(game) {

}

void GameHUD::reload_buttons() {
    if(!game_.initialized_) return;

    const IRenderer& renderer = game_.visualizer_.renderer();
    int window_width = renderer.window_width();
    int window_height = renderer.window_height();

    buttons_.clear();

    int size = 25;
    int pad = 6;

    UIButton restart("restart_button", window_width - GUI_SCALE * (size + pad) , GUI_SCALE * pad, GUI_SCALE * size, GUI_SCALE * size);
    restart.set_on_click([=, this]() {
        game_.reset();
        game_.sound().play("action", 0);
    });
    buttons_.push_back(restart);

    UIButton save("save_button", window_width - GUI_SCALE * (size + pad) * 2 , GUI_SCALE * pad, GUI_SCALE * size, GUI_SCALE * size);
    save.set_on_click([=, this]() {
        game_.save_.save(game_);
        game_.sound().play("action", 0);
    });
    buttons_.push_back(save);

    UIButton load("load_button", window_width - GUI_SCALE * (size + pad) * 3 , GUI_SCALE * pad, GUI_SCALE * size, GUI_SCALE * size);
    load.set_on_click([=, this]() {
        game_.save_.load(game_);
        game_.sound().play("action", 0);
    });
    buttons_.push_back(load);



    if (game_.state_ == GameState::Upgrading) {
        int next_lvl_width = 250;
        int next_lvl_height = 70;
        UIButton next_level("next_level_button", (window_width - GUI_SCALE * next_lvl_width)/2 , window_height/1.3, GUI_SCALE * next_lvl_width, GUI_SCALE * next_lvl_height);
        next_level.set_on_click([=, this]() {
            game_.state_ = GameState::PlayerMove;
            reload_buttons();
            game_.sound().play("action", 0);
        });
        next_level.set_text("Next Level");
        next_level.set_text_font("default_large");
        next_level.set_text_y_padding(20 * GUI_SCALE);
        buttons_.push_back(next_level);



        PlayerUpgradeRegistry& upgrades = PlayerUpgradeRegistry::instance();
        const auto& ids = game_.logic().available_upgrades();

        int upgrade_width = 128 * GUI_SCALE;
        int upgrade_height = 192 * GUI_SCALE;
        int pad = 10;

        int total_width = ids.size() * (upgrade_width + pad) - pad;
        int start_x = (window_width - total_width) / 2;
        int y = window_height / 2 - upgrade_height / 2;

        for (size_t i = 0; i < ids.size(); ++i) {
            int idx = i;
            if (game_.logic_.is_upgrade_purchased(idx)) continue;

            const auto& id = ids[i];
            PlayerUpgrade* upgrade = upgrades.get(id);
            if (!upgrade) continue;

            int x = start_x + i * (upgrade_width + pad);

            UIButton button("player_upgrade", x, y, upgrade_width, upgrade_height);
            button.set_text_y_padding(25 * GUI_SCALE);

            int current_level = game_.logic_.get_upgrade_level(upgrade->id());
            int next_level = current_level + 1;

            const auto desc = upgrade->text(next_level);

            int cost = upgrade->price(next_level);
            int playerGold = game_.player_->get_attribute(Attribute::Gold);

            std::string status;
            if (playerGold >= cost)
                status = "Purchase";
            else
                status = "Not enough\ngold";

            button.set_text(
                "Upgrade\n\n" + desc +
                "\n\nCost\n" + std::to_string(cost) + " Gold"
                "\n\n" + status
            );



            button.set_on_click([=, this]() {
                if (playerGold >= cost && !game_.logic_.is_upgrade_purchased(idx)) {
                    game_.logic_.set_upgrade_purchased(idx);
                    game_.logic_.set_upgrade_level(upgrade->id(), next_level);
                    upgrade->apply(game_.logic_, next_level);
                    game_.player_->sub_attribute(Attribute::Gold, cost);
                    game_.sound().play("reward", 0);
                    reload_buttons();
                }
            });

            buttons_.push_back(button);
        }
    }
}
