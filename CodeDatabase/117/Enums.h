#ifndef ENUMS_H
#define ENUMS_H

enum class UpgradeType {HealthUpgrade=1, MeleeUpgrade, RangeUpgrade, SpellUpgrade};

enum class Mode {Read, Write};

enum class SpellType {DirDMGSpell, AreaDMGSpell, SummonSpell, TrapSpell, UpgradeSpell};

enum class ActionType {MoveUp, MoveLeft, MoveDown, MoveRight, Attack,
    Switch, Pass, Surrender, Info, UseSpell, BuySpell, Save, Load };

enum class MainMenuChoice {NewGame=1, LoadSave, Exit};

#endif