#ifndef WALL_H
#define WALL_H

#include<raylib.h>
class Wall{
    public:
        Vector2 Position;
        Vector2 Size;
        Color WallColor;
        float Bounciness;
        Wall(Vector2, Vector2, Color, float);
};
#endif