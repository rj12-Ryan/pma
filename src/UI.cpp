#include "UI.h"
#include<raylib.h>
#include<string>
#include "Scenario.h"

UI::UI(int windowX, int windowY, bool statusBarEnabled,  int desiredScenario, int statusBarHeight = 30)
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
      //Draw All Walls
    for(int i=0; i<(int)currentScenario.Walls.size(); i++){
        DrawRectangleV(currentScenario.Walls[i].Position, currentScenario.Walls[i].Size, currentScenario.Walls[i].WallColor);
    }
    //Draw All Balls in the List
    for(int i=0; i<(int)currentScenario.Balls.size(); i++){
        if(currentScenario.Balls[i].Position.y > WindowY()){
            currentScenario.RemoveBall(i);
            break;
        }
        DrawCircleV(currentScenario.Balls[i].Position, currentScenario.Balls[i].Radius, currentScenario.Balls[i].BallColor);
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
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
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
        Ball newBall(mouse, v, 10, WHITE);
        CurrentScenario.Balls.push_back(newBall);
    }

    if(IsKeyPressed(KEY_S)){
        ToggleStatus();
    }
}
