#pragma once
#include "GameObjects/Map.hpp"
#include "types.hpp"

class MapController {
public:
    MapController(int level);
    MapDTO toDTO();
    void loadLevel(int level);

    bool canStep(Coordinates pos, Coordinates delta, FieldType whoseStep);
    std::vector<Coordinates> getAllowedSteps(Coordinates pos, FieldType whoseStep);

    Field* getField(Coordinates coord);
    Field* getField(int x, int y);

    int getWidth();
    int getHeight();

private:
    LevelManager LM;
    int width;
    int height;
    Map map;
};
