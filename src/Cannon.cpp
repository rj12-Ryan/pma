#include "Cannon.h"
#include <raylib.h>

Cannon::Cannon(Vector2 position, Vector2 size, float aimAngle, Color cannonColor, float strength, int ballsRemaining)
    :Position(position), Size(size), AimAngle(aimAngle), CannonColor(cannonColor), Strength(strength), BallsRemaining(ballsRemaining)
{
    Rendered = true;
}

Cannon::Cannon(){
    Rendered = false;
}