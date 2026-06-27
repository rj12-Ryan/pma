#include<iostream>
#include<raylib.h>
#include<cmath>
#include<cstdlib>
#include<vector>
#include<string>
#include<format>
#include "pma.h"
#include "Ball.h"
#include "Wall.h"
#include "Scenario.h"
#include "PhysicsEngine.h"
#include "UI.h"

#define GRAVITY 700
#define TARGET_PHYSICS_PER_SECOND 1000.0f


Scenario CurrentScenario;

int main(){
    //CREATE A UI
    UI ui(1920, 1080, true, static_cast<int>(Scenario::SavedScenarios::EMPTY), 30);
    ui.InitPMAWindow();

    //LOAD SCENARIO
    CurrentScenario.LoadScenario(ui);

    //GENTLEMEN... START YOUR ENGINE
    PhysicsEngine engine(GRAVITY, 1.0f/TARGET_PHYSICS_PER_SECOND, CurrentScenario);
 
    while(!WindowShouldClose()){
        BeginDrawing();
        ui.ProcessInput(CurrentScenario);
        engine.Step();
        ui.Draw(CurrentScenario);
        ClearBackground(BLANK);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}



