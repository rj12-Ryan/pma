#ifndef SCENARIO_H
#define SCENARIO_H

#include<raylib.h>
#include<vector>
#include<string>

#include "Ball.h"
#include "Wall.h"
#include "UI.h"

class Scenario{
    private:
        u_int32_t flags = 0;
    public:
        enum class SavedScenarios{
            EMPTY,
            APPOSING_WALLS,
            THE_WAVE,
            VARIOUS_BOUNCE,
            MANY_BALLS,
            Count //Sentinel value must be last
        };
        enum class ScenarioFlags : u_int32_t{
            None = 0,
            RainbowBalls = 1 << 0
        };

        std::string LoadedScenarioName;
        std::vector<Ball> Balls;
        std::vector<Wall> Walls;

        void NewBall(Ball ball);
        void RemoveBallID(int ballIndex);
        void RemoveBall(Ball* ball);
        void NewWall(Wall wall);
        void RemoveWall(int wallIndex);
        void ClearBalls();
        void SetFlag(ScenarioFlags flag);
        void ClearFlag(ScenarioFlags flag);
        bool HasFlag(ScenarioFlags flag) const;
        SavedScenarios NextScenario(SavedScenarios current);
        SavedScenarios PreviousScenario(SavedScenarios current);
        void LoadScenario(UI&);
};

#endif