#include "Scenario.h"
#include "UI.h"
#include <cassert>

void Scenario::NewBall(Ball ball){
    Balls.push_back(ball);
}

void Scenario::RemoveBall(int ballIndex){
    Balls.erase(Balls.begin() + ballIndex);
}

void Scenario::NewWall(Wall wall){
    Walls.push_back(wall);
}

void Scenario::RemoveWall(int wallIndex){
    Walls.erase(Walls.begin() + wallIndex);
}

void Scenario::ClearBalls(){
    Balls.clear();
}

void Scenario::SetFlag(ScenarioFlags flag){
    flags |= static_cast<u_int32_t>(flag);
}

void Scenario::ClearFlag(ScenarioFlags flag){
    flags &= ~static_cast<u_int32_t>(flag);
}

bool Scenario::HasFlag(ScenarioFlags flag) const{
    return (flags & static_cast<u_int32_t>(flag)) !=0;
}

Scenario::SavedScenarios Scenario::NextScenario(SavedScenarios current){
    int next = (static_cast<int>(current) + 1) % static_cast<int>(SavedScenarios::Count);
    return static_cast<SavedScenarios>(next);
}

Scenario::SavedScenarios Scenario::PreviousScenario(SavedScenarios current){
    int prev;
    if(static_cast<int>(current)==0){
        prev = static_cast<int>(SavedScenarios::Count)-1;
    }
    else{
        prev = static_cast<int>(current)-1;
    }
    return static_cast<SavedScenarios>(prev);
}

void Scenario::LoadScenario(UI& ui){
    //RESET SCENARIO
    Balls.clear();
    Walls.clear();
    flags = 0;

    switch(static_cast<SavedScenarios>(ui.DesiredScenario)){
        //SCENARIO COUNT (UNREACHABLE)
        case SavedScenarios::Count:
            assert(("UNREACHABLE: Cannot load sentinel value of Count", false));
        break;

        //SCENARIO EMPTY
        case SavedScenarios::EMPTY:
            LoadedScenarioName = "EMPTY";
        break;

        //SCENARIO APPOSING WALLS
        case SavedScenarios::APPOSING_WALLS:
            LoadedScenarioName = "APPOSING WALLS";
            this->NewWall({{30,0},{100,1000},DARKGREEN, 0.99});
            this->NewWall({{600,100},{100,950},DARKGREEN, 0.99});
        break;

        //SCENARIO VARIOUS BOUNCE
        case SavedScenarios::VARIOUS_BOUNCE:
            LoadedScenarioName = "VARIOUS BOUNCE";
            this->NewWall({{0, (float)ui.WindowY()-50}, {ui.WindowX()/2, 50}, DARKGREEN, 0.99f});
            this->NewWall({{ui.WindowX()/2, (float)ui.WindowY()-50}, {ui.WindowX()/2, 50}, DARKBLUE, 0.50f});
        break;

        //SCENARIO THE WAVE
        case SavedScenarios::THE_WAVE:
            LoadedScenarioName = "THE WAVE";
            this->SetFlag(ScenarioFlags::RainbowBalls);
            this->NewWall({{0, (float)ui.WindowY()-50}, {(float)ui.WindowX(), 50}, DARKGREEN, 0.99});
            int ballR = 10;
            int ballCount = ui.WindowX() / (ballR*2);
            for(int i=0; i<ballCount; i++){
                this->NewBall({(Vector2){(float)ballR+(2*ballR)*i, 10.0f+(0.5*ballR*i)}, (Vector2){0,0}, ballR, BLUE});
            }
        break;


    }
}