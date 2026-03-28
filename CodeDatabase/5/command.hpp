#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <cstdint>

//направление атаки
enum class Direction { None, Up, Down, Left, Right };

//глв. меню
enum class MenuCommandType { NewGame, LoadGame, Exit, Invalid };

struct MenuCommand {
  MenuCommandType type{MenuCommandType::Invalid};
};

//конец игры
enum class GameOverCommandType { RestartGame, BackToMainMenu, Exit, Invalid };

struct GameOverCommand {
  GameOverCommandType type{GameOverCommandType::Invalid};
};

//управление игрой в моменте
enum class LevelCommandType {
  None,
  Move,         //w/a/s/d
  SwitchStyle,  //c
  QuitLevel,    //q
  SaveGame,     //save
  BuySpell,     //b
  ShowSpells,   //m
  RangedAttack, //xw/xs/xa/xd
  UseSpell      //1,2,3
};

struct LevelCommand {
  LevelCommandType type{LevelCommandType::None};
  Direction direction{Direction::None};
  std::int32_t spellIndex{-1}; 
};

#endif 
