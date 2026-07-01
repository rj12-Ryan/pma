#include "Scenario.h"
#include "UI.h"
#include <cassert>
#include "Wall.h"

void Scenario::LoadScenario(UI& ui){
    //RESET SCENARIO
    Balls.clear();
    Walls.clear();
    Pegs.clear();
    NextBallID = 1;
    NextWallID = 1;
    NextPegID = 1;
    flags = 0;
    SavedScenarios ss = static_cast<SavedScenarios>(ui.DesiredScenario);
    switch(ss){
        //SCENARIO COUNT (UNREACHABLE)
        case SavedScenarios::Count:
            //we cannot reach this state because Count is not a valid Saved Scenario
            assert(false);
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
            this->NewWall(Wall((Vector2){0, (float)ui.WindowY()-50.0f}, (Vector2){(float)ui.WindowX()/2, 50.0f}, DARKGREEN, 0.99f));
            this->NewWall(Wall((Vector2){(float)ui.WindowX()/2, (float)ui.WindowY()-50.0f}, (Vector2){(float)ui.WindowX()/2, 50.0f}, DARKBLUE, 0.50f));
            this->NewBall(Ball{(Vector2){ui.WindowX()/4, 50},(Vector2){0,0}, 10, WHITE});
            this->NewBall(Ball{(Vector2){3*ui.WindowX()/4, 50},(Vector2){0,0}, 10, WHITE});
       break;

        //SCENARIO THE WAVE
        case SavedScenarios::THE_WAVE:
        {
            LoadedScenarioName = "THE WAVE";
            this->SetFlag(ScenarioFlags::RainbowBalls);
            this->NewWall({{0, (float)ui.WindowY()-50}, {(float)ui.WindowX(), 50}, DARKGREEN, 0.99});
            int ballR = 10;
            int ballCount = ui.WindowX() / (ballR*2);
            for(int i=0; i<ballCount; i++){
                this->NewBall({(Vector2){(float)ballR+(2*ballR)*i, 10.0f+(float)(0.5*ballR*i)}, (Vector2){0,0}, ballR, BLUE});
            }
        break;
        }

        //SCENARIO MANY BALLS
        case SavedScenarios::MANY_BALLS:
        {
            LoadedScenarioName = "MANY BALLS";
            this->SetFlag(ScenarioFlags::RainbowBalls);
            this->NewWall({(Vector2){0, (float)ui.WindowY()-50}, (Vector2){(float)ui.WindowX(), 50}, YELLOW, 1.0f});
            for(int i=0; i<452; i++){
                float r = (float)(GetRandomValue(10, ui.WindowX()-10));
                float r2 = (float)(GetRandomValue(10, ui.WindowY()-100));
                this->NewBall(Ball{(Vector2){r, 10 + r2},(Vector2){0,0}, 10, BLUE});
            }
        break;
        }

        //SCENARIO PEG TESTING
        case SavedScenarios::PEG_TESTING:{
            LoadedScenarioName = "PEG TESTING";
            this->NewPeg({(Vector2){ui.WindowX()/2, ui.WindowY()/2}, 50, 0.99f, Peg::PegType::DEFAULT});
        break;
        }

        //SCENARIO PACHINKO
        case SavedScenarios::PACHINKO:{
            LoadedScenarioName = "PACHINKO";
            int radius = 10;
            int spacing = 80;
            for(int i=2*radius; i<ui.WindowX(); i+=spacing){
                for(int j=100; j<ui.WindowY(); j+=spacing){
                    this->NewPeg({(Vector2){i,j}, radius, 0.9f, Peg::PegType::DEFAULT});
                }
           }
        break;
        }

        //SCENARIO BASIC PEGGLE
        case SavedScenarios::BASIC_PEGGLE:{
            LoadedScenarioName = "BASIC PEGGLE";

            BallBasket = Basket({(Vector2){150,40},(Vector2){50,ui.WindowY()-50},(Vector2){ui.WindowX()-50-150,ui.WindowY()-50}, 0.2f, 0.0f, RED});

            int paddingX = 200;
            int paddingY = 250;

            int boardX = ui.WindowX() - 2*paddingX;
            int boardY = ui.WindowY() - 2*paddingY;

            int countX = 12;
            int countY = 8;

            float spacingX = boardX/(countX+1);
            float spacingY = boardY/(countY+1);

            for(int x=0; x<countX; x++){
                for(int y=0; y<countY; y++){
                    float offset = 0;
                    if(y%2 == 0){
                        offset = spacingX/2;
                    }
                    this->NewPeg({(Vector2){x*spacingX + 1.5*paddingX + offset ,y*spacingX + 0.75*paddingY}, 10, 0.8f, Peg::PegType::DEFAULT});
                }
            }

            std::vector<int> indicies(Pegs.size());
            for (int i=0; i < Pegs.size(); i++){
                indicies[i] = i;
            }

            for (int i = indicies.size() -1; i > 0; i--){
                int j = GetRandomValue(0,i);
                std::swap(indicies[i], indicies[j]);
            }

            for(int i=0; i<25; i++){
                Pegs[indicies[i]].CurrentPegType = Peg::PegType::TARGET;
            }
        break;
        }
    }
}