#pragma once
#include <iostream>

enum class State {
    Idle,
    Moving,
    Attacking
};

inline std::string stateToString(State state) {
    switch (state) {
    case State::Idle:
        return "idle";
    case State::Moving:
        return "moving";
    case State::Attacking:
        return "attacking";
    default:
        return "unknown";
    }
}

class Entity{
private:
    const float error_rate = 0.05f;
protected:
    float x_, y_;
    float target_x_, target_y_;
    float speed_;
    int direction_; // -1 = left, 1 = right
    int health_;
    int max_health_;
    int damage_;

    bool doneTurn_ = false;
    bool rangeAttack = false;

    std::string texture_name_;
    State state_ = State::Idle;

    void move(float dt);
public:
    Entity(std::string texture_name_, float speed, int health, int damage) 
        : texture_name_(texture_name_)
        , damage_(damage)
        , x_(0.0f)
        , y_(0.0f)
        , target_x_(0.0f)
        , target_y_(0.0f)
        , speed_(speed)
        , direction_(1)
        , health_(health)
        , max_health_(health)
        , state_(State::Idle) {}
    ~Entity() = default;

    float getX() const { return x_; }
    float getY() const { return y_; }
    int getDirection() const { return direction_; }
    int getHealth() const { return health_; }
    int getMaxHealth() const { return max_health_; }
    void setMaxHealth(int m) { max_health_ = m; }

    void setX(float x) { x_ = x; }
    void setY(float y) { y_ = y; }
    void setDirection(int direction) { direction_ = direction; }
    void applyHeal(int heal) { health_ += heal; if(health_ > max_health_) health_ = max_health_; }
    void applyDamage(int damage) { health_ -= damage; if(health_ < 0) health_ = 0; }
    void setDamage(int damage) { damage_ = damage; }
    std::string getTextureName() const { return texture_name_; }
    State getState() const { return state_; }

    void setTarget(float newX, float newY) {
        target_x_ = newX;
        target_y_ = newY;
    }

    virtual void update(float dt);

    void onAttackEnd() {
        state_ = State::Idle;
    }

    bool isDoneTurn() {
        return doneTurn_;
    }
    
    void doneTurn() {
        doneTurn_ = true;
    }

    void resetTurn() {
        doneTurn_ = false;
    }

    void setRangeAttack(bool value) {
        rangeAttack = value;
    }

    void toggleRangeAttack() {
        rangeAttack = !rangeAttack;
    }

    bool isRangeAttack() const {
        return rangeAttack;
    }

    int getBaseDamage() const {
        return damage_;
    }

    int getDamage() const {
        return damage_ * (rangeAttack ? 1 : 2);
    }

    float getSpeed() const {
        return speed_;
    }
};