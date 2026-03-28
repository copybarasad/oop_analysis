#include "Command.h"
#include "GameManager.h"
#include <iostream>

bool MoveCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  return manager->executeMove(dx, dy);
}

bool AttackCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  return manager->executeAttack(dx, dy);
}

bool SwitchAttackCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  return manager->executeSwitchAttack();
}

bool CastSpellCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  return manager->executeCastSpell();
}

bool SkipCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  return true;
}

bool SaveCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  manager->executeSave();
  return false;
}

bool HelpCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  manager->executeHelp();
  return false;
}

bool QuitCommand::execute(GameManager *manager) {
  if (!manager)
    return false;
  manager->executeQuit();
  return false;
}
