#pragma once
#include <Entity.hpp>
#include <ScreenSize.hpp>
#include <memory>

class Ceil {
public:
    Ceil();
    void setEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> getEntity() const;
    void setDefault();
    void draw() const;

private: 
    std::shared_ptr<Entity> entity;
};
