#ifndef CANNON_H
#define CANNON_H
#include <raylib.h>

class Cannon{
    public:
        Cannon(Vector2 position, Vector2 size, float aimAngle, Color cannonColor, float strength, int ballsRemaining);
        Cannon();
        bool Rendered;
        Vector2 Position;
        Vector2 Size;
        float AimAngle;
        Color CannonColor;
        float Strength;
        int BallsRemaining;
};


#endif
