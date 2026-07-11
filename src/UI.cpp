#include "UI.h"
#include<raylib.h>
#include<string>
#include "Scenario.h"
#include <ranges>
#include <cmath>
#include<string>

UI::UI(int windowX, int windowY, bool statusBarEnabled, int statusBarHeight, int desiredScenario)
    : _windowX(windowX), _windowY(windowY), _statusBarEnabled(statusBarEnabled), DesiredScenario(desiredScenario), _statusBarHeight(statusBarHeight) 
{}

int UI::WindowX(){
    return _windowX;
}

int UI::WindowY(){
    return _windowY;
}

void UI::InitPMAWindow(){
    if(_statusBarEnabled){
        InitWindow(WindowX(), WindowY()+_statusBarHeight, "Raylib");
    }
    else{
        InitWindow(WindowX(), WindowY(), "Raylib");
    }   
}

void UI::ToggleStatus(){
    _statusBarEnabled = !_statusBarEnabled;
    if(_statusBarEnabled){
        SetWindowSize(WindowX(), WindowY()+_statusBarHeight);
    }
    else{
        SetWindowSize(WindowX(), WindowY());
    }
}

void UI::_drawStatusBar(Scenario& CurrentScenario)
{
    int windowY = WindowY();
    DrawRectangle(0, windowY, WindowX(), _statusBarHeight, DARKGRAY);
    DrawFPS(20, (windowY + 0.5*_statusBarHeight - 10));
    std::string ballCountStr = "BALLS: ";
    ballCountStr.append(std::to_string(CurrentScenario.Balls.size()));
    Color c;
    if(CurrentScenario.Balls.size()>99) {c = YELLOW;} else {c = GREEN;}
    DrawText(ballCountStr.c_str(), 150, (windowY + 0.5*_statusBarHeight - 10), 20, c);
    DrawText(CurrentModeStr.c_str(), 300, (windowY + 0.5*_statusBarHeight - 10), 20, BLACK);
    std::string scenStr = "LOADED SCENARIO: " + CurrentScenario.LoadedScenarioName;
    int scenStrWidth = MeasureText(scenStr.c_str(), 20);
    DrawText(scenStr.c_str(), WindowX() - 10 - scenStrWidth, (windowY + 0.5*_statusBarHeight - 10), 20, BLACK);
}

void UI::Draw(Scenario& CurrentScenario){
    _drawCounter++;

    //Draw All Walls
    for(Wall& wall : CurrentScenario.Walls){
        DrawRectangleV(wall.Position, wall.Size, wall.WallColor);
    }
    //Draw All Pegs
    for(Peg& peg : CurrentScenario.Pegs){
        Color c;
        switch(peg.CurrentPegType){
            case Peg::PegType::DEFAULT:{
                c = BLUE;
            break;
            }
            case Peg::PegType::TARGET:{
                c = ORANGE;
            break;
            }
        }
        if(peg.Hit){
            DrawCircleV(peg.Position, peg.Radius+2, WHITE);
        }
        DrawCircleV(peg.Position, peg.Radius, c);
    }

    //Draw Basket
    if(CurrentScenario.BallBasket.Rendered){
        DrawRectangleV(CurrentScenario.BallBasket.Position, CurrentScenario.BallBasket.Size, CurrentScenario.BallBasket.BasketColor);
    }

    //Draw All Balls in the List
    for(int i=CurrentScenario.Balls.size() - 1; i>=0; i--){
        if(CurrentScenario.Balls[i].Position.y > WindowY()){
            CurrentScenario.RemoveBall(CurrentScenario.Balls[i].ID);
            continue;
        }
        DrawCircleV(CurrentScenario.Balls[i].Position, CurrentScenario.Balls[i].Radius, CurrentScenario.Balls[i].BallColor);
    }

    //Draw Cannon
    if(CurrentScenario.BallCannon.Rendered){
        DrawCircleV(CurrentScenario.BallCannon.Position, CurrentScenario.BallCannon.BaseRad, CurrentScenario.BallCannon.CannonColor);
        DrawCircleV(CurrentScenario.BallCannon.PointerPoint, CurrentScenario.BallCannon.PointerRad, RED);
        
        const int numFontSize = 50;
        std::string numStr = std::to_string(CurrentScenario.BallCannon.BallsRemaining);
        int textWidth = MeasureText(numStr.c_str(), numFontSize);

        DrawText(numStr.c_str(), CurrentScenario.BallCannon.Position.x - (textWidth/2), CurrentScenario.BallCannon.Position.y + 5, numFontSize, BLACK);
    }


    if(_statusBarEnabled){
        _drawStatusBar(CurrentScenario);
    }
}

void UI::ProcessInput(Scenario& CurrentScenario){
    switch(CurrentMode){
        case UIMode::DEVELOPER:{
            if(IsKeyPressed(KEY_R)){
                CurrentScenario.LoadScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario), WindowX(), WindowY());
            }
            if(IsKeyPressed(KEY_C)){
                CurrentScenario.ClearBalls();
            }
            if(IsKeyPressed(KEY_RIGHT)){
                DesiredScenario = static_cast<int>(CurrentScenario.NextScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario)));
                CurrentScenario.LoadScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario), WindowX(), WindowY());
            }
            if(IsKeyPressed(KEY_LEFT)){
                DesiredScenario = static_cast<int>(CurrentScenario.PreviousScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario)));
                CurrentScenario.LoadScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario), WindowX(), WindowY());
            }
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                Vector2 mouse = GetMousePosition();
                float directionalVelocity = 1500;
                Vector2 v = {0,0};
                    if(IsKeyDown(KEY_W)){
                        v = {v.x + 0, v.y + -directionalVelocity}; 
                    }
                    if(IsKeyDown(KEY_A)){
                        v = {v.x + -directionalVelocity,v.y + 0};
                    }
                    if(IsKeyDown(KEY_S)){
                        v = {v.x + 0, v.y + directionalVelocity};
                    }
                    if(IsKeyDown(KEY_D)){
                        v = {v.x + directionalVelocity, v.y + 0};
                    }
                CurrentScenario.NewBall(Ball{mouse, v, 10, WHITE});
            }

            //Right Click Fountain
            bool prevBallOldEnough;
            if(CurrentScenario.Balls.empty()){
                prevBallOldEnough = true;
            }
            else{
                prevBallOldEnough = (CurrentScenario.Balls.back().Age>50);
            }
            if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT) && prevBallOldEnough){
                Vector2 mouse = GetMousePosition();
                float directionalVelocity = 1500;
                Vector2 v = {0,0};
                    if(IsKeyDown(KEY_W)){
                        v = {v.x + 0, v.y + -directionalVelocity}; 
                    }
                    if(IsKeyDown(KEY_A)){
                        v = {v.x + -directionalVelocity,v.y + 0};
                    }
                    if(IsKeyDown(KEY_S)){
                        v = {v.x + 0, v.y + directionalVelocity};
                    }
                    if(IsKeyDown(KEY_D)){
                        v = {v.x + directionalVelocity, v.y + 0};
                    }
                    CurrentScenario.NewBall(Ball{mouse, v, 10, WHITE});
            }

            if(IsKeyPressed(KEY_TAB)){
                ToggleStatus();
            }

            if(IsKeyPressed(KEY_M)){
                CurrentMode = UIMode::CANNON;
                CurrentModeStr = "CANNON";
            }

            break;
        }

        case UIMode::CANNON:{
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CurrentScenario.BallCannon.BallsRemaining>0){
                    Vector2 ballVelocity;
                    Vector2 p1 = CurrentScenario.BallCannon.PointerPoint;
                    Vector2 p2 = GetMousePosition();
                    float dx = p2.x - p1.x;
                    float dy = p2.y - p1.y;

                    float length = sqrtf(dx*dx + dy*dy);

                    if (length == 0.0f){
                        ballVelocity = {0,0};
                    }

                    ballVelocity = {(dx/length)*CurrentScenario.BallCannon.Strength,(dy/length)*CurrentScenario.BallCannon.Strength};

                    CurrentScenario.NewBall(Ball{CurrentScenario.BallCannon.PointerPoint, ballVelocity, 10, WHITE});
                    
                    CurrentScenario.BallCannon.BallsRemaining--;
                }
                
            }

            if(IsKeyPressed(KEY_R)){
                CurrentScenario.LoadScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario), WindowX(), WindowY());
            }
            
            if(IsKeyPressed(KEY_M)){
                CurrentMode = UIMode::DEVELOPER;
                CurrentModeStr = "DEVELOPER";
            }
            break;
        }

    }
    
}
