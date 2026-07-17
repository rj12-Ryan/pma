#ifndef CANNON_H
#define CANNON_H
#include <raylib.h>

class Cannon{
    private:
        float _pointerAngle;
    public:
        Cannon(Vector2 position, float baseRad, float pointerRad, Color cannonColor, float strength, int ballsRemaining);
        Cannon();
        bool Rendered;
        Vector2 Position;
        float BaseRad;
        float PointerRad;
        Vector2 PointerPoint;
        Color CannonColor;
        float Strength;
        int BallsRemaining;
        void SetPointerAngle(float angDeg);
        bool MouseColliding = false;
};


#endif
