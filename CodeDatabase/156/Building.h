//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_BUILDING_H
#define LABA1_BUILDING_H

#include "../Entity.h"

class Building : public Entity{
public:
    Building(int xCoordinate, int yCoordinate, int healthPoint, EntityType type) : Entity(xCoordinate,
                                                                                          yCoordinate,
                                                                                          healthPoint, type) {}
};


#endif //LABA1_BUILDING_H
