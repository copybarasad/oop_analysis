#pragma once


enum AttackType {
    common_attack,
    ranged_attack
};

enum Flag {
    common_flag,
    player_flag,
    enemy_flag,
    vortex_flag,
    fish_net_flag,
    octopus_flag,
};

enum SpellFlag{
    none,
    fish_net,
    jet_flag,
    mine_flag,
};

enum CommandID{
    None,
    Move,
    SwitchAttackType,
    RangedAttack,
    UseSpell,
    SelectSpell,
    Save,
    Load,
    StartNewGame,
    LoadGame,
    SelectLevel,
    ChooseUpgrade,
    Confirm,
    Cancel,
    Quit
};