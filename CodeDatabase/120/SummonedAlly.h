#pragma once
class GameWorld;
class GameLogic;

class SummonedAlly : public Entity {
 public:
  SummonedAlly(Position pos);
  void Act(GameWorld& world, GameLogic& logic);

 private:
  int damage = 5;
};
