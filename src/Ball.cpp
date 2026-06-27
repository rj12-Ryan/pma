#include "Ball.h"

Ball::Ball(Vector2 position, Vector2 velocity, int radius, Color ballcolor)
    : Position(position), Velocity(velocity), Radius(radius), BallColor(ballcolor)
{
    Age = 0;
};

void Ball::ApplyAcceleration(Vector2 accel, float dt){
    Velocity.x = Velocity.x + (accel.x * dt);
    Velocity.y = Velocity.y + (accel.y * dt);
    Position.x = Position.x + (Velocity.x * dt);
    Position.y = Position.y + (Velocity.y * dt);
}