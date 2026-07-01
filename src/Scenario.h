#ifndef SCENARIO_H
#define SCENARIO_H

#include<raylib.h>
#include<vector>
#include<string>
#include <unordered_map>

#include "Ball.h"
#include "Wall.h"
#include "UI.h"
#include "Peg.h"
#include "Basket.h"

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
            PEG_TESTING,
            PACHINKO,
            BASIC_PEGGLE,
            Count //Sentinel value must be last
        };
        enum class ScenarioFlags : u_int32_t{
            None = 0,
            RainbowBalls = 1 << 0
        };

        std::string LoadedScenarioName;
        std::vector<Ball> Balls;
        std::vector<Wall> Walls;
        std::vector<Peg> Pegs;
        Basket BallBasket = Basket({(Vector2){0,0},(Vector2){0,0},(Vector2){0,0}, 100.0f, 0.0f, MAGENTA}); //Construct Basket with default values to be overwritten by SavedScenario

        std::unordered_map<BallID, size_t> BallLookup;
        std::unordered_map<WallID, size_t> WallLookup;
        std::unordered_map<PegID, size_t> PegLookup;


        BallID NextBallID = 1;
        PegID NextPegID = 1;
        WallID NextWallID =1;

        void NewBall(Ball ball);
        void RemoveBall(BallID id);
        Ball& GetBall(BallID);
        void NewWall(Wall wall);
        void RemoveWall(WallID id);
        Wall& GetWall(WallID);
        void NewPeg(Peg peg);
        void RemovePeg(PegID id);
        Peg& GetPeg(PegID);
        void ClearBalls();
        void SetFlag(ScenarioFlags flag);
        void ClearFlag(ScenarioFlags flag);
        bool HasFlag(ScenarioFlags flag) const;
        SavedScenarios NextScenario(SavedScenarios current);
        SavedScenarios PreviousScenario(SavedScenarios current);
        void LoadScenario(UI&);
};

#endif