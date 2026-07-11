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
    UI ui(1920, 1080, true, 30, static_cast<int>(Scenario::SavedScenarios::BASIC_PEGGLE));
    ui.InitPMAWindow();

    //LOAD SCENARIO
    CurrentScenario.LoadScenario(static_cast<Scenario::SavedScenarios>(ui.DesiredScenario), ui.WindowX(), ui.WindowY());

    //GENTLEMEN... START YOUR ENGINE
    PhysicsEngine engine(GRAVITY, 1.0f/TARGET_PHYSICS_PER_SECOND, CurrentScenario);
 
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(BLANK);
        ui.ProcessInput(CurrentScenario);
        engine.Step();
        ui.Draw(CurrentScenario);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}



