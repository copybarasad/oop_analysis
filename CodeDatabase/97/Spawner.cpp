#include "Spawner.h"
#include "Field.h"

Spawner::Spawner(int hp, int spawnInterval)
        : Entity(hp, 0, "Spawner"),
          spawnInterval_(spawnInterval),
          timer_(spawnInterval)
{}

std::shared_ptr<Entity> Spawner::clone() const { return std::make_shared<Spawner>(*this); }
bool Spawner::readyToSpawn() const { return timer_ <= 0; }
void Spawner::resetTimer() { timer_ = spawnInterval_; }
void Spawner::onTurn(Field& field, const Coord& pos) {
}
void Spawner::decrementTimer() {
    if (timer_ > 0) --timer_;
}

void Spawner::setTimer(int t) { timer_ = t; }
int Spawner::timer() const { return timer_; }


