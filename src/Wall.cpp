#include "Wall.h"
#include <cstdlib>

Wall::Wall(Vector2 position, Vector2 size, Color wallcolor, float bounciness)
    : Position(position), Size(size), WallColor(wallcolor), Bounciness(bounciness)
{
    //Bounciness Cannot Exceed 1.0
    if(Bounciness>1.0f){
        std::abort();
    }
}