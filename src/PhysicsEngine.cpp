#include "PhysicsEngine.h"
#include "Scenario.h"
#include "Ball.h"
#include<raylib.h>

PhysicsEngine::PhysicsEngine(int gravity, float dt, Scenario& scenarioPointer)
    : Gravity(gravity), DT(dt), ScenarioPointer(scenarioPointer)
    {}

void PhysicsEngine::_updatePhysics(){
    for(Ball& b : ScenarioPointer.Balls){
        //Rainbow Balls!
        if(ScenarioPointer.HasFlag(Scenario::ScenarioFlags::RainbowBalls)){
            b.BallColor = (Color){static_cast<unsigned char>((b.Position.y/3)),static_cast<unsigned char>((b.Position.y/2)),static_cast<unsigned char>((b.Position.y)),255};
        }
        b.ApplyAcceleration({0, (float)Gravity}, DT);
        b.Age++;
    }
    _ballWallCollision();
}

void PhysicsEngine::_ballWallCollision(){
    for(Wall& wall : ScenarioPointer.Walls){
        Rectangle r;
        r.x = wall.Position.x;
        r.y = wall.Position.y;
        r.width = wall.Size.x;
        r.height = wall.Size.y;
        
        for(Ball& ball : ScenarioPointer.Balls){
           if(CheckCollisionCircleRec(ball.Position,ball.Radius,r)){
            float dy = DT * ball.Velocity.y;
            float dx = DT * ball.Velocity.x;

            ball.Position.y -= dy;
            ball.Position.x -= dx;

            if(ball.Position.y > r.y && ball.Position.y < r.y+r.height){
                ball.Velocity.x = -wall.Bounciness*ball.Velocity.x;
            }
            if(ball.Position.y > r.y+r.height || ball.Position.y<r.y){
                ball.Velocity.y = -wall.Bounciness*ball.Velocity.y;
            }

           }
        }
    }
}

void PhysicsEngine::Step(){
        _accumulator += GetFrameTime();
        while(_accumulator>=DT){
            _updatePhysics();
            _accumulator -= DT;
        }
}