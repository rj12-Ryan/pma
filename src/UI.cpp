#include "UI.h"
#include<raylib.h>
#include<string>
#include "Scenario.h"
#include <ranges>

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

void UI::_drawStatusBar(Scenario& currentScenario)
{
    int windowY = WindowY();
    DrawRectangle(0, windowY, WindowX(), _statusBarHeight, DARKGRAY);
    DrawFPS(20, (windowY + 0.5*_statusBarHeight - 10));
    std::string ballCountStr = "BALLS: ";
    ballCountStr.append(std::to_string(currentScenario.Balls.size()));
    Color c;
    if(currentScenario.Balls.size()>99) {c = YELLOW;} else {c = GREEN;}
    DrawText(ballCountStr.c_str(), 150, (windowY + 0.5*_statusBarHeight - 10), 20, c);
    std::string scenStr = "LOADED SCENARIO: " + currentScenario.LoadedScenarioName;
    int scenStrWidth = MeasureText(scenStr.c_str(), 20);
    DrawText(scenStr.c_str(), WindowX() - 10 - scenStrWidth, (windowY + 0.5*_statusBarHeight - 10), 20, BLACK);
}

void UI::Draw(Scenario& currentScenario){
    _drawCounter++;

    //Draw All Walls
    for(Wall& wall : currentScenario.Walls){
        DrawRectangleV(wall.Position, wall.Size, wall.WallColor);
    }
    //Draw All Balls in the List
    for(int index = 0; Ball& ball : currentScenario.Balls){
        if(ball.Position.y > WindowY()){
            currentScenario.RemoveBallID(index);
            break;
        }
        DrawCircleV(ball.Position, ball.Radius, ball.BallColor);
        index++;
    }
    
    if(_statusBarEnabled){
        _drawStatusBar(currentScenario);
    }
}

void UI::ProcessInput(Scenario& CurrentScenario){
    if(IsKeyDown(KEY_R)){
        CurrentScenario.LoadScenario(*this);
    }
    if(IsKeyPressed(KEY_C)){
        CurrentScenario.ClearBalls();
    }
    if(IsKeyPressed(KEY_RIGHT)){
        DesiredScenario = static_cast<int>(CurrentScenario.NextScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario)));
        CurrentScenario.LoadScenario(*this);
    }
    if(IsKeyPressed(KEY_LEFT)){
        DesiredScenario = static_cast<int>(CurrentScenario.PreviousScenario(static_cast<Scenario::SavedScenarios>(DesiredScenario)));
        CurrentScenario.LoadScenario(*this);
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
}
