#pragma once
class Point{
public:
    int x;
    int y;

    Point();
    Point(int a, int b);
    bool operator==(Point& other);
    ~Point();
};


