#ifndef POINT_H        
#define POINT_H        

class Point {         
public:                
    explicit Point(bool isPassable = true);  
    bool isPassable() const;                 
    void setPassable(bool passable);         

private:               
    bool isPassable_;  
};

#endif                