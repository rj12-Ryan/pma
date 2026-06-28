#include "Scenario.h"
#include "UI.h"

void Scenario::NewBall(Ball ball){
    Balls.push_back(ball);
}

void Scenario::RemoveBallID(int ballIndex){
    Balls.erase(Balls.begin() + ballIndex);
}

void Scenario::RemoveBall(Ball* ball){
    Balls.erase(Balls.begin() + (ball - Balls.data()));
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

void Scenario::NewPeg(Peg peg){
    Pegs.push_back(peg);
}

void Scenario::RemovePeg(int pegIndex){
    Pegs.erase(Pegs.begin() + pegIndex);
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

