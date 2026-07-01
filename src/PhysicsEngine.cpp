#include "PhysicsEngine.h"
#include "Scenario.h"
#include "Ball.h"
#include<raylib.h>
#include<math.h>
#include <iostream>

PhysicsEngine::PhysicsEngine(int gravity, float dt, Scenario& currentScenario)
    : Gravity(gravity), DT(dt), CurrentScenario(currentScenario)
    {}

void PhysicsEngine::_updatePhysics(){
    for(Ball& b : CurrentScenario.Balls){
        //Rainbow Balls!
        if(CurrentScenario.HasFlag(Scenario::ScenarioFlags::RainbowBalls)){
            b.BallColor = (Color){static_cast<unsigned char>((b.Position.y/3)),static_cast<unsigned char>((b.Position.y/2)),static_cast<unsigned char>((b.Position.y)),255};
        }
        b.ApplyAcceleration({0, (float)Gravity}, DT);
        b.Age++;
    }
    _ballWallCollision();
    _ballPegCollision();
}

void PhysicsEngine::_ballWallCollision(){
    for(Wall& wall : CurrentScenario.Walls){
        Rectangle r;
        r.x = wall.Position.x;
        r.y = wall.Position.y;
        r.width = wall.Size.x;
        r.height = wall.Size.y;
        
        for(Ball& ball : CurrentScenario.Balls){
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

void PhysicsEngine::_ballPegCollision(){
    for(Peg& peg : CurrentScenario.Pegs){
        for(Ball& ball: CurrentScenario.Balls){
            float dx = ball.Position.x - peg.Position.x;
            float dy = ball.Position.y - peg.Position.y;
            float distance = sqrt(dx*dx + dy*dy);
            float minDistance = ball.Radius + peg.Radius;
            
            if (distance<minDistance){
                if(!peg.Hit){
                    peg.Hit = true;
                    ball.PegsHit.push_back(peg.ID);
                }
                
                ball.BallColor = RED;
                Vector2 normal;
                if(distance > 0.0001f){
                    normal.x = dx/distance;
                    normal.y = dy/distance;
                }
                else{
                    normal.x = 1.0f;
                    normal.y = 0.0f;
                }

                float penetration = minDistance - distance;

                ball.Position.x += normal.x * penetration;
                ball.Position.y += normal.y * penetration;

                float vn = ball.Velocity.x * normal.x + ball.Velocity.y * normal.y;

                if (vn < 0.0f){
                    float dir = GetRandomValue(-1,1);
                    ball.Velocity.x -= (1.0f + peg.Bounciness) * vn * normal.x + dir*0.04f;
                    ball.Velocity.y -= (1.0f + peg.Bounciness) * vn * normal.y;
                }
            }
            else{
                ball.BallColor = WHITE;
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