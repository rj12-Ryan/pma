#ifndef WALL_H
#define WALL_H

#include<raylib.h>
#include "pma.h"

class Wall{
    public:
        WallID ID;
        Vector2 Position;
        Vector2 Size;
        Color WallColor;
        float Bounciness;
        Wall(Vector2, Vector2, Color, float);
};
#endif