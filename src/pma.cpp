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
    UI ui(1920, 1080, true, 30, static_cast<int>(Scenario::SavedScenarios::FROM_FILE));
    ui.InitPMAWindow(); 

    //LOAD SCENARIO
    CurrentScenario.LoadScenario(static_cast<Scenario::SavedScenarios>(ui.DesiredScenario), ui.WindowX(), ui.WindowY());

    //INIT AUDIO
    CurrentScenario.Sounds.LoadSounds();

    //GENTLEMEN... START YOUR ENGINE
    PhysicsEngine engine(GRAVITY, 1.0f/TARGET_PHYSICS_PER_SECOND, CurrentScenario);

    RenderTexture2D sceneTexture = LoadRenderTexture(ui.WindowX(), ui.WindowY() + ui._statusBarHeight);
    SetTextureFilter(sceneTexture.texture, TEXTURE_FILTER_BILINEAR);

    //BACKGROUND TEXTURE
    Texture bgTexture = LoadTextureFromImage(LoadImage("src/resources/textures/background.png"));
    Rectangle bgSourceRect = { 0.0f, 0.0f, (float)bgTexture.width, (float)bgTexture.height };
    Rectangle bgDestRec = {0, 0, ui.WindowX(), ui.WindowY()};
    SetTextureFilter(bgTexture, TEXTURE_FILTER_BILINEAR);

    while(!WindowShouldClose()){
        UpdateMusicStream(CurrentScenario.Sounds.Background);

        BeginTextureMode(sceneTexture);
            ClearBackground(BLANK);
            ui.ProcessInput(CurrentScenario);
            engine.Step();
            ui.Draw(CurrentScenario);
        EndTextureMode();

        BeginDrawing();
           ClearBackground(BLACK);
                BeginBlendMode(BLEND_ALPHA);
                    //draw bg
                    DrawTexturePro(bgTexture, bgSourceRect, bgDestRec, {0,0}, 0, WHITE);
                    //draw scene shadow
                    DrawTextureRec(sceneTexture.texture, {2.0f, 2.0f, (float)sceneTexture.texture.width, -(float)sceneTexture.texture.height}, {0,0}, (Color){ 0, 0, 0, 50 });
                    //draw scene shadow2
                    DrawTextureRec(sceneTexture.texture, {-0.5f, -2.0f, (float)sceneTexture.texture.width, -(float)sceneTexture.texture.height}, {0,0}, (Color){ 0, 0, 0, 255});
                    //draw scene
                    DrawTextureRec(sceneTexture.texture, {0, 0.0f, (float)sceneTexture.texture.width, -(float)sceneTexture.texture.height}, {0,0}, WHITE);
                EndBlendMode();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}



