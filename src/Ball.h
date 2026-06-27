#ifndef BALL_H
#define BALL_H

#include<raylib.h>

class Ball{
    public:
        Vector2 Position; //pixel x pixel
        Vector2 Velocity; //p/s x p/s
        int Radius; //pixels
        Color BallColor; //raylib color
        float Age; //seconds
        Ball(Vector2, Vector2, int, Color);
        void ApplyAcceleration(Vector2, float);
};
#endif