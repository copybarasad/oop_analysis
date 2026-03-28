#pragma once

#include "Game.h"
#include "Command.h"
#include "LevelManager.h"
#include "UpgradeManager.h"
#include "Visualizer.h"
#include "Render.h"
#include <optional>
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <memory>
using namespace std;


template <typename InputProvider, typename RendererType>
class Control {
private:
    GameVisualizer<RendererType> visualizer;
    Game& game;
    InputProvider& input;
    LevelManager& lev;
    UpgradeManager ups;

public:
    Control(Game& game, InputProvider& input, LevelManager& level)
        : visualizer(game, std::make_unique<RendererType>()), game(game), input(input), lev(level), ups(3) {}

    int start();
    int cicle();
    int levelCicle(int cur_lvl, bool turn);
    bool showMenu();
    void executeCommand(const Command& cmd);
    bool save(const std::string& path) const;
    bool load(const std::string& path);
    bool useUpgrade(UpgradeManager& ups, int c);
    int upgrade();
    bool upCheck(int choise, int size);
    bool checkInput();
};

template <typename InputProvider, typename RendererType>
bool Control<InputProvider, RendererType>::save(const std::string& path) const {
    try {
        game.save(path);
        std::cout << "\033[34mИгра сохранена.\033[0m" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "\033[41mНе удалось сохранить: " << e.what() << "\033[0m" << std::endl;
        return false;
    }
}

template <typename InputProvider, typename RendererType>
bool Control<InputProvider, RendererType>::load(const std::string& path) {
    save("tmp_save.dat");
    try {
        game.load(path);
        std::cout << "\033[34mИгра загружена.\033[0m" << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cout << "\033[41mНе удалось загрузить: " << e.what() << "\033[0m" << std::endl;
        try { game.load("tmp_save.dat"); } catch (...) {}
        return false;
    }
}

template <typename InputProvider, typename RendererType>
bool Control<InputProvider, RendererType>::upCheck(int choise, int size) {
    if (choise < 0 || choise >= size) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

template <typename InputProvider, typename RendererType>
bool Control<InputProvider, RendererType>::checkInput() {
    if (std::cin.fail()) {
        std::cout << "\033[41mОшибка ввода значений!\033[0m" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

template <typename InputProvider, typename RendererType>
bool Control<InputProvider, RendererType>::useUpgrade(UpgradeManager& ups, int c) {
    Upgrade* up = ups.getUp(c);
    if (up == nullptr) {
        std::cout << "\033[41mУлучшение вне диапазона\033[0m" << std::endl;
        return false;
    }

    if (up->getId() == 0) {
        game.getPlayer().setDamage(ups.use(up, game.getPlayer().getDamage()));
        game.getPlayer().spendScoreOnSpell(up->getAmount());
    } else if (up->getId() == 1) {
        game.getPlayer().setHp(ups.use(up, game.getPlayer().getHp()));
        game.getPlayer().spendScoreOnSpell(up->getAmount());
    } else if (up->getId() == 2) {
        game.getPlayer().getBag().setMaxSize(ups.use(up, game.getPlayer().getBag().getMaxSize()));
        game.getPlayer().spendScoreOnSpell(up->getAmount());
    }
    return true;
}

template <typename InputProvider, typename RendererType>
bool Control<InputProvider, RendererType>::showMenu(){
    Divers_bag& bag = game.getPlayerBag();
    if (bag.bagIsEmpty()) {
        std::cout << "\033[41mВ сумке нет заклинаний!\033[0m" << std::endl;
        return false;
    }
    bag.display_bag();
    return true;
}

template <typename InputProvider, typename RendererType>
void Control<InputProvider, RendererType>::executeCommand(const Command& cmd) {
    switch (cmd.id) {
        case CommandID::Move:
            game.plMovement(cmd.dx, cmd.dy); break;
        case CommandID::SwitchAttackType:
            game.plSwitchAttackType(); break;
        case CommandID::RangedAttack:
            game.plRangedAttack(cmd.x, cmd.y); break;
        case CommandID::UseSpell:
            game.useSpell(cmd.spellIndex, cmd.x, cmd.y);
            break;
        case CommandID::Save:
            load("save.dat"); break;
        case CommandID::Load:
            load(cmd.path); break;
        case CommandID::Quit:
            break;
        case CommandID::None:
            break;
        case CommandID::StartNewGame:
        case CommandID::SelectLevel: {
            int lvl = (cmd.id == CommandID::SelectLevel) ? cmd.levelIndex : 1;
            GameConfig cfg = lev.getLevel(lvl);
            game.reset(cfg);
            break;
        }
        case CommandID::LoadGame:
            load("save.dat");
            break;
        case CommandID::ChooseUpgrade:
            useUpgrade(ups, cmd.upgradeIndex);
            break;
        default:
            break;
    }
}

template <typename InputProvider, typename RendererType>
int Control<InputProvider, RendererType>::start() {
    bool flag = false;
    while (!flag) {
        visualizer.renderMenu();
        auto cmdOpt = input.getNextCommand();
        if (!cmdOpt) continue;
        
        const Command& cmd = *cmdOpt;
        executeCommand(cmd);

        if (cmd.id == CommandID::Quit) {
            return 0;
        }
        int level = 1;
        if (cmd.id == CommandID::SelectLevel) {
            level = cmd.levelIndex;
        }
        int res = levelCicle(level, true);
        if(!res){
            flag = true;
        }
    }
    
    return 0;
}

template <typename InputProvider, typename RendererType>
int Control<InputProvider, RendererType>::cicle() {
    int freezeTime = 0;

    while (game.getIsGameOn()) {
        visualizer.clear();
        visualizer.renderStatistics();
        visualizer.renderControls();
        visualizer.renderField();

        auto cmdOpt = input.getNextCommand();
        if (!cmdOpt) continue;
        const Command& cmd = *cmdOpt;

        executeCommand(cmd);

        if (cmd.id == CommandID::Quit) {
            return 0;
        }

        bool isPlayerAction =
            cmd.id == CommandID::Move ||
            cmd.id == CommandID::RangedAttack ||
            cmd.id == CommandID::UseSpell;

        if (isPlayerAction) {
            if (freezeTime == 0) {
                game.octopus_attack();
                freezeTime = 2;
            }
            freezeTime--;
            game.enMovement();
            game.resetPlayerTurn();
        }

        std::cout << std::endl;
    }

    if (game.getIsWin()) {
        std::cout << std::setw(37) << std::right << "\033[44mYou are win!\033[0m" << std::endl;
    } else {
        std::cout << std::setw(37) << std::right << "\033[41mGame is over\033[0m" << std::endl;
    }
    visualizer.renderStatistics();

    return 0;
}

template <typename InputProvider, typename RendererType>
int Control<InputProvider, RendererType>::levelCicle(int cur_lvl, bool turn) {
    if (turn) {
        GameConfig new_cfg = lev.getLevel(cur_lvl);
        game.reset(new_cfg);
        input.switchToGameplay();
    }
    LevelConfig new_lvl_cfg = lev.getLevel(cur_lvl);
    cicle();

    if (new_lvl_cfg.next_lvl_ind && game.getIsWin()) {
        std::cout << "\033[34mЖелаете перейти на следующий уровень?:\033[0m ";
        char choise;
        do {
            std::cout << "\033[34mВыберите действие (Y|N):\033[0m ";
            std::cin >> choise;
        } while (choise != 'Y' && choise != 'N');

        switch (choise) {
            case 'Y':
                if (upgrade() == 1) {
                    levelCicle(cur_lvl + 1, true);
                    break;
                }
                levelCicle(cur_lvl + 1, true);
                break;
            case 'N':
                input.switchToMenu();
                return 1;
        }
    } else if (!new_lvl_cfg.next_lvl_ind && game.getIsWin()) {
        std::cout << "ПОЗДРАВЛЯЕМ!" << std::endl << "игра пройдена" << std::endl;
    } else if (!game.getIsWin()){
        GameConfig new_cfg;
        game.reset(new_cfg);
        input.switchToMenu();
        return 1;
    }
    return 0;
}

template <typename InputProvider, typename RendererType>
int Control<InputProvider, RendererType>::upgrade() {
    std::cout << "\033[44mВам доступны улучшения персонажа! Желаете прокачаться?\033[0m" << std::endl;
    UpgradeManager new_ups(3);
    ups = new_ups;

    while (ups.getSize() > 0) {
        ups.display();
        char choise;
        do {
            std::cout << "\033[34mВыберите действие (Y|N):\033[0m ";
            std::cin >> choise;
        } while (choise != 'Y' && choise != 'N');

        switch (choise) {
            case 'Y': {
                int c;
                do {
                    std::cout << "Выберите улучшение: " << std::endl;
                    std::cin >> c;
                } while (!checkInput() || !upCheck(c, ups.getSize()));

                if (!useUpgrade(ups, c)) {
                    std::cout << "\033[41mОшибка!\033[0m" << std::endl;
                    break;
                }
                break;
            }
            case 'N':
                return 0;
        }
    }
    ups.reset();
    return 0;
}