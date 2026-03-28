#ifndef GAME_FIELD_H    
#define GAME_FIELD_H

#include <vector>       
#include "point.h"      
#include "01_position.h"   

class GameField {       
public:                 
    GameField(int width, int height);
    GameField(const GameField& other);           
    GameField(GameField&& other) noexcept;       
    GameField& operator=(const GameField& other); 
    GameField& operator=(GameField&& other) noexcept; 
    ~GameField();                               


    bool isPositionValid(Position pos) const;    
    bool isPositionPassable(Position pos) const; 
    int getWidth() const;                        
    int getHeight() const;                       

private:                
    int width_;        
    int height_;        
    Point** points_;    


    void initializePoints();  
    void copyFrom(const GameField& other);  
    void cleanup();           
};

#endif