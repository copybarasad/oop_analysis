#ifndef TRAP_HPP
#define TRAP_HPP

class Creature;

class Trap {
  public:
    virtual void trigger(Creature *creature) = 0;
};

#endif /* TRAP_HPP */
