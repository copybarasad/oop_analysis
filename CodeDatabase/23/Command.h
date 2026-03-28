#ifndef COMMAND_H
#define COMMAND_H

#include <memory>

class GameManager;
class GameField;

class Command {
public:
  virtual ~Command() = default;
  virtual bool execute(GameManager *manager) = 0;
  virtual const char *getName() const = 0;
};

class MoveCommand : public Command {
private:
  int dx, dy;

public:
  MoveCommand(int dx, int dy) : dx(dx), dy(dy) {}
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "Move"; }
  int getDx() const { return dx; }
  int getDy() const { return dy; }
};

class AttackCommand : public Command {
private:
  int dx, dy;

public:
  AttackCommand(int dx, int dy) : dx(dx), dy(dy) {}
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "Attack"; }
  int getDx() const { return dx; }
  int getDy() const { return dy; }
};

class SwitchAttackCommand : public Command {
public:
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "SwitchAttack"; }
};

class CastSpellCommand : public Command {
public:
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "CastSpell"; }
};

class SkipCommand : public Command {
public:
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "Skip"; }
};

class SaveCommand : public Command {
public:
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "Save"; }
};

class HelpCommand : public Command {
public:
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "Help"; }
};

class QuitCommand : public Command {
public:
  bool execute(GameManager *manager) override;
  const char *getName() const override { return "Quit"; }
};

#endif 
