#include "AimingSystem.h"

AimingSystem::AimingSystem(std::string center) : centerTile(center) {}

void AimingSystem::setDeathNoteAim() 
{
    centerTile = "x ";
    patternTiles = 
    {
        {Coordinates(-1, 0), "( "},
        {Coordinates(1, 0), " )"}
    };
}

void AimingSystem::setPsiStormAim(int AoE) 
{
    centerTile = ". ";
    patternTiles.clear();
    for(int dx = 0; dx < AoE; dx++)
        for(int dy = 0; dy < AoE; dy++)
            patternTiles.push_back({Coordinates(dx, dy), ". "});
}

void AimingSystem::setTrapAim() { centerTile = "փ "; patternTiles.clear(); }
void AimingSystem::setDoppelgangerAim() { centerTile = "ʘ "; patternTiles.clear(); }

void AimingSystem::drawAim(Map& game_map, Coordinates target) const 
{
    game_map.getCell(target.getX_coord(), target.getY_coord()).CellSetTile(centerTile);
    for(const auto& tile : patternTiles) 
    {
        int x = target.getX_coord() + tile.first.getX_coord();
        int y = target.getY_coord() + tile.first.getY_coord();
        if(game_map.iswithinlimits(x, y))
            game_map.getCell(x, y).CellSetTile(tile.second);
    }
}

void AimingSystem::clearAim(Map& game_map, Coordinates target) const 
{
    game_map.getCell(target.getX_coord(), target.getY_coord()).CellResetTile();
    for(const auto& tile : patternTiles) 
    {
        int x = target.getX_coord() + tile.first.getX_coord();
        int y = target.getY_coord() + tile.first.getY_coord();
        if(game_map.iswithinlimits(x, y))
            game_map.getCell(x, y).CellResetTile();
    }
}