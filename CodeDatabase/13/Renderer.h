#ifndef GAME_RENDERER_H
#define GAME_RENDERER_H
#include <SFML/Graphics.hpp>
#include <map>
#include "../basics/GameLogic.h"
#include "../basics/Map.h"


class Renderer {
private:
    sf::Font font_;
    sf::Text hpText_;
    sf::Text scoreText_;
    sf::Text overText_;
    sf::Text spellName_;
    sf::Text spellDesc_;
    sf::Text spellLevel_;
    bool showSpellInfo_ = true;
    bool isOver = false;
    sf::RectangleShape fadeRect_;
    sf::RectangleShape spellBar_;
    bool isFade = false;
    sf::Color background_ = sf::Color(198, 154, 98);
    Coordinates map_size_;
    const float sprite_size_ = 60;
    const std::string game_name_ = "Chaos";
    GameLogic *glogic_;
    Map *map_;
    float map_offset_x_ = 0;
    float map_offset_y_ = 0;
    sf::RenderWindow window_;
    std::map<std::string, std::string> files_map_{
        {"placeholder", "placeholder.png"},
        {"weapon_placeholder", "weapon_placeholder.png"},
        {"stun", "stun.png"},
        {"EnemyBase", "enemy_base.png"},
        {"SpellTower", "spell_tower.png"},
        {"Knife", "knife.png"},
        {"Enemy", "enemy.png"},
        {"StrongerEnemy", "stronger_enemy.png"},
        {"Ally", "ally.png"},
        {"AllyBase", "ally_base.png"},
        {"SlowTrap", "slow_trap.png"},
        {"PlayerTrap", "player_trap.png"},
        {"Pan", "pan.png"},
        {"Mixer", "mixer.png"},
        {"SpellBook", "spell_book.png"},
        {"Block", "block.png"},
        {"Cell", "tile.png"},
        {"Player", "cock.png"},
        {"Frame", "frame.png"},
        {"Mana0", "mana0.png"},
        {"Mana1", "mana1.png"},
        {"Mana2", "mana2.png"},
        {"Mana3", "mana3.png"},
        {"Mana4", "mana4.png"},
        {"Mana5", "mana5.png"},
        {"NeedMana1", "need_mana1.png"},
        {"NeedMana2", "need_mana2.png"},
        {"NeedMana3", "need_mana3.png"},
        {"NeedMana4", "need_mana4.png"},
        {"NeedMana5", "need_mana5.png"},
        {"Paper", "paper.png"},
        {"RangeMark", "range_mark.png"},
        {"ChosenMark", "chosen_mark.png"},
        {"AvailableMark", "available_mark.png"},
        {"Direct damage spell", "direct_damage_spell.png"},
        {"Direct damage spell advance", "direct_damage_spell_advance.png"},
        {"Area damage spell", "area_damage_spell.png"},
        {"Area damage spell advance", "area_damage_spell_advance.png"},
        {"Trap spell", "trap_spell.png"},
        {"Trap spell advance", "trap_spell_advance.png"},
        {"Ally spell", "ally_spell.png"},
        {"Ally spell advance", "ally_spell_advance.png"},
        {"Improving spell", "improving_spell.png"},
        {"Improving spell advance", "improving_spell_advance.png"}
    };
    std::map<std::string, sf::Texture> texstures_map_;
    std::map<std::string, sf::Sprite> sprites_map_;

public:
    explicit Renderer(GameLogic *glogic) : glogic_(glogic), map_(&glogic->getMap()) {
    };

    void run();

    void close();

    bool pollEvent(sf::Event &event);

    void renderGame();

    void load_sprites();

    sf::Sprite prepareSprite(const std::string &name, int x, int y, bool weapon = false, bool need_offset = true);

    sf::RenderWindow *getWindow();

    sf::Sprite &getSprite(const std::string &name, bool weapon = false);

    void prepareText(sf::Text &text, int size = 32) const;

    void setStatusText(int hp, int score);

    void switchFade(bool sw);

    void switchOver(bool sw);

    void switchSpellInfoState();

    void updateMapSize();

    int renderMenu(const std::vector<std::string>& options);

    bool isOpen() const;
};


#endif
