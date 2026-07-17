#include "Scenario.h"
#include "UI.h"

void Scenario::NewBall(Ball ball){
    Ball b = ball;
    b.ID = NextBallID++;
    Balls.push_back(b);

    size_t index = Balls.size() - 1;
    BallLookup[b.ID] = index;
}

void Scenario::RemoveBall(BallID id, bool playMissSound){
    std::vector<PegID> pegsHit = GetBall(id).PegsHit;
    if(!pegsHit.empty()){
        for(PegID p : pegsHit){
            RemovePeg(p);
            Sounds.Play(Sounds.Poof);
        }
    }

    if(pegsHit.empty() && MissAudioEnabled && playMissSound){
        Sounds.Play(Sounds.Miss);
    }

    size_t index = BallLookup.at(id);

    size_t last = Balls.size() - 1;
    if(index != last)
    {
        std::swap(Balls[index], Balls[last]);
        BallLookup[Balls[index].ID] = index;
    }
    Balls.pop_back();
    BallLookup.erase(id);
}

Ball& Scenario::GetBall(BallID id){
    return Balls[BallLookup.at(id)];
}

void Scenario::NewWall(Wall wall){
    Wall w = wall;
    w.ID = NextWallID++;
    Walls.push_back(w);

    size_t index = Walls.size() - 1;
    WallLookup[w.ID] = index;
}

void Scenario::RemoveWall(WallID id){
    size_t index = WallLookup.at(id);

    size_t last = Walls.size() - 1;
    if(index != last)
    {
        std::swap(Walls[index], Walls[last]);
        WallLookup[Walls[index].ID] = index;
    }
    Walls.pop_back();
    WallLookup.erase(id);
}

Wall& Scenario::GetWall(WallID id){
    return Walls[WallLookup.at(id)];
}

void Scenario::ClearBalls(){
    while (!Balls.empty()){
        RemoveBall(Balls.back().ID, false);
    }
}

void Scenario::NewPeg(Peg peg){
    Peg p = peg;
    p.ID = NextPegID++;
    Pegs.push_back(p);

    size_t index = Pegs.size() - 1;
    PegLookup[p.ID] = index;
}

void Scenario::RemovePeg(PegID id){
    size_t index = PegLookup.at(id);

    size_t last = Pegs.size() - 1;
    if(index != last)
    {
        std::swap(Pegs[index], Pegs[last]);
        PegLookup[Pegs[index].ID] = index;
    }
    Pegs.pop_back();
    PegLookup.erase(id);
}

Peg& Scenario::GetPeg(PegID id){
    return Pegs[PegLookup.at(id)];
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

