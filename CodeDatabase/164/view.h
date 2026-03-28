#ifndef VIEW_H
#define VIEW_H

template<typename RendererT>
class Game_view {
    private:
    RendererT renderer;
    
    public:
    Game_view() = default;
    
    void print_game_state(Field& field, Hero& hero, Hand& hand, unsigned int win_points);
    
    void print_main_menu(Control& control);
    
    void print_turn_menu(Control& control, bool canUseSpell);

    void print_new_exit(Control& control);

    void print_save_exit(Control& control);

    void print_message(const std::string& msg);
};

#endif