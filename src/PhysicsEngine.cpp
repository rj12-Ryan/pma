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
    _moveBasket();
    _updateCannon();
    _ballBasketCollision();
    _checkGameState();
}

void PhysicsEngine::_checkGameState(){
    //make sure we are actually using a cannon in this scenario
    if(CurrentScenario.BallCannon.Rendered){
        bool AllBallsGone = (CurrentScenario.Balls.size() == 0);
        bool CannonEmpty = (CurrentScenario.BallCannon.BallsRemaining == 0);
        bool AllTargetPegsCleared = true;
        
        for(Peg& p : CurrentScenario.Pegs){
            if(p.CurrentPegType == Peg::PegType::TARGET){
                AllTargetPegsCleared = false;
                break;
            }
        }
    
        if(AllBallsGone && CannonEmpty){
            if(AllTargetPegsCleared){
                CurrentScenario.Popup.Text = "YOU ARE WINNER!";
                CurrentScenario.Popup.Enabled = true;
            }
            else{
                CurrentScenario.Popup.Text = "YOU LOSE";
                CurrentScenario.Popup.Enabled = true;
            }
        }
    }


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

void PhysicsEngine::_ballBasketCollision(){
    Rectangle r;
    r.x = CurrentScenario.BallBasket.Position.x;
    r.y = CurrentScenario.BallBasket.Position.y;
    r.width = CurrentScenario.BallBasket.Size.x;
    r.height = CurrentScenario.BallBasket.Size.y;

    std::vector<BallID> toRemove; //defer ball deletions until after we iterate entire ball list 
    toRemove.reserve(CurrentScenario.Balls.size());
    for(Ball& ball : CurrentScenario.Balls){
           if(CheckCollisionCircleRec(ball.Position,ball.Radius,r)){
                toRemove.push_back(ball.ID);
           }
    }
    for(BallID id : toRemove){
        CurrentScenario.RemoveBall(id);
        CurrentScenario.BallCannon.BallsRemaining++;
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
        }
    }
}

void PhysicsEngine::_moveBasket(){
    Basket& bb = CurrentScenario.BallBasket;
    bb.Age++;
    float ageTime = DT*bb.Age;

    float prog = ageTime * bb.Speed;

    float ppt = 1.0f - abs((fmod(prog,2)) - 1);

    bb.Position.x = bb.Start.x + ppt * (bb.End.x - bb.Start.x);
    bb.Position.y = bb.Start.y + ppt * (bb.End.y - bb.Start.y);
}

void PhysicsEngine::_updateCannon(){
    Vector2 p1 = CurrentScenario.BallCannon.Position;
    Vector2 p2 = GetMousePosition();

    const int MAXANGLE = 170;
    const int MINANGLE = 10;

    float delta_y = p2.y - p1.y;
    float delta_x = p2.x - p1.x;

    float radians = atan2(delta_y, delta_x);

    float degrees = (radians * (180.0/PI));
    
    //check mouse collision
    if(CheckCollisionPointCircle(p2, CurrentScenario.BallCannon.Position, CurrentScenario.BallCannon.BaseRad+10)){
        CurrentScenario.BallCannon.MouseColliding = true;
    }
    else{
        CurrentScenario.BallCannon.MouseColliding = false;
    }

    if(degrees>MAXANGLE){
        degrees=MAXANGLE;
    }
    if(degrees<MINANGLE){
        degrees=MINANGLE;
    }
    
    CurrentScenario.BallCannon.SetPointerAngle(degrees);
}

void PhysicsEngine::Step(){
        _accumulator += GetFrameTime();
        while(_accumulator>=DT){
            _updatePhysics();
            _accumulator -= DT;
        }
}