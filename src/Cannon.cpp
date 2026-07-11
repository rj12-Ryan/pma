#include "Cannon.h"
#include <raylib.h>
#include <stdexcept>
#include <cmath>

Cannon::Cannon(Vector2 position, float baseRad, float pointerRad, float pointerAngle, Color cannonColor, float strength, int ballsRemaining)
    :Position(position), BaseRad(baseRad), PointerRad(pointerRad), CannonColor(cannonColor), Strength(strength), BallsRemaining(ballsRemaining)
{
    Rendered = true;
    SetPointerAngle(pointerAngle);
}

Cannon::Cannon(){
    Rendered = false;
}

void Cannon::SetPointerAngle(float angDeg){
        float angRad = angDeg * PI / 180.0;
        float pointX = Position.x + (BaseRad+PointerRad) * cos(angRad);
        float pointY = Position.y + (BaseRad+PointerRad) * sin(angRad);
        PointerPoint = {pointX, pointY};
}