#include "ScenarioLoader.h"
#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm> 
#include <raylib.h>

ScenarioLoader::ScenarioLoader(std::string path, Scenario& currentScenario, float windowX, float windowY)
    : CurrentScenario(currentScenario), WindowX(windowX), WindowY(windowY)
{
    CenterX = windowX/2;
    CenterY = windowY/2;
    File.open(path);
    if(!File.is_open()){
        std::cerr << "Failed to open scenario file: " << path << "\n";
        return;
    }
    Parse();
}

void ScenarioLoader::Parse(){
    std::string line;
    LineCounter = 0;
    while (std::getline(File, line))
    {
        LineCounter++;
        line = Trim(line);
        if(line.substr(0,2) == "//"){
            continue;
        }
        if(IsSection(line)){
            FinishCurrentSection();
            StartNewSection(line);
            continue;
        }
        if(IsAssignment(line)){
            if(CurrentSection == Section::none){
                ParserError("Cannot assign outside of section");
            }
            ParseAssignment(line);
        }
    }   
    FinishCurrentSection();
    if(_expectGrid){
        ParserError("Expected Grid section But did not find Grid section");
    }
}

bool ScenarioLoader::IsSection(std::string line){
    std::unordered_set<std::string> valid_sections = {"INFO", "BASKET", "PEG", "WALL", "BALL", "CANNON", "GRID"};
    return valid_sections.count(line) == 1;
}

bool ScenarioLoader::IsAssignment(std::string line){
    return (line.find('=') != std::string::npos);
}

void ScenarioLoader::FinishCurrentSection(){
    switch(CurrentSection){
        case Section::BALL:{
            if(!CurrentBall->IsComplete() && !CurrentBall->isGridding){
                ParserError("Incomplete Ball defintion");
                break;
            }
            if(!CurrentBall->isGridding){
                CurrentScenario.NewBall(CurrentBall->Build());
            }
            else{
                _expectGrid = true;
            }
            PreviousSection = Section::BALL;
            break;
        }
        case Section::WALL:{
            if(!CurrentWall->IsComplete()){
                ParserError("Incomplete Wall defintion");
                break;
            }
            CurrentScenario.NewWall(CurrentWall->Build());
            PreviousSection = Section::WALL;
            break;
        }
        case Section::CANNON:{
            CurrentScenario.BallCannon.Rendered = true;
            break;
        }
        case Section::BASKET:{
            CurrentScenario.BallBasket.Rendered = true;
            break;
        }
        case Section::PEG:{
            if(!CurrentPeg->IsComplete() && !CurrentPeg->isGridding){
                ParserError("Incomplete Peg defintion");
                break;
            }
            if(!CurrentPeg->isGridding){
                CurrentScenario.NewPeg(CurrentPeg->Build());
            }
            else{
                _expectGrid = true;
            }
            PreviousSection = Section::PEG;
            break;
        }
        case Section::GRID:{
            if(!_expectGrid){
                ParserError("Grid specified for object that is not configured to use one");
                break;
            }
            if(!CurrentGrid->IsComplete()){
                ParserError("Incomplete Grid defintion");
                break;
            }
            float xSpacing = CurrentGrid->Size.x/CurrentGrid->XSize;
            float ySpacing = CurrentGrid->Size.y/CurrentGrid->YSize;
            float offset;

            std::vector<Peg> pegsToAdd;

            for(int x = 0; x < CurrentGrid->XSize; x++){
                for(int y = 0; y < CurrentGrid->YSize; y++){
                    if(y%2 == 0){
                        offset = CurrentGrid->OffsetFactor*xSpacing;
                    }
                    else{
                        offset = 0;
                    }
                    Vector2 p = {CurrentGrid->Position.x+xSpacing*x+offset, CurrentGrid->Position.y+ySpacing*y};
                    switch(PreviousSection){
                        case Section::PEG:{
                            CurrentPeg->Position = p;
                            //save the pegs we need to add to a temp vector
                            pegsToAdd.push_back(CurrentPeg->Build());
                            break;
                        }
                        case Section::BALL:{
                            CurrentBall->Position = p;
                            CurrentScenario.NewBall(CurrentBall->Build());
                            break;
                        }
                    }
                }
            }
            //if we are drawing peg grid apply random flips to temp peg vector and add to current scenario
            if(PreviousSection==Section::PEG){
                //check vailidity of randomflipped
                if(CurrentGrid->RandomFlipped > CurrentGrid->XSize * CurrentGrid->YSize){
                    ParserError("Random Flipped larger than total amount of Pegs drawn");
                    break;
                }

                //determine the opposite peg type based on first in temp vector
                Peg::PegType oppo = Peg::PegType::DEFAULT; //default by default
                switch(pegsToAdd[0].CurrentPegType){
                    case Peg::PegType::DEFAULT:
                        oppo = Peg::PegType::TARGET;
                        break;
                    case Peg::PegType::TARGET:
                        oppo = Peg::PegType::DEFAULT;
                        break;
                }

                std::vector<int> indicies(pegsToAdd.size());
                for (int i=0; i < pegsToAdd.size(); i++){
                    indicies[i] = i;
                }

                for (int i = indicies.size() -1; i > 0; i--){
                    int j = GetRandomValue(0,i);
                    std::swap(indicies[i], indicies[j]);
                }

                for(int i=0; i<CurrentGrid->RandomFlipped; i++){
                    pegsToAdd[indicies[i]].CurrentPegType = oppo;
                }

                for(Peg& p : pegsToAdd){
                    CurrentScenario.NewPeg(p);
                }
            }
            _expectGrid = false;
        }
    }
    CurrentSection = Section::none;
}

void ScenarioLoader::StartNewSection(std::string line){
    CurrentSection = StrToSection(line);
    if(_expectGrid && CurrentSection!=Section::GRID){
        ParserError("Expected Grid section but did not find Grid section");
    }
    switch(CurrentSection){
        case Section::BALL:{
            CurrentBall.emplace();
            break;
        }
        case Section::WALL:{
            CurrentWall.emplace();
            break;
        }
        case Section::PEG:{
            CurrentPeg.emplace();
            break;
        }
        case Section::GRID:{
            if(!_expectGrid){
                ParserError("Invalid Position for Grid Section");
                break;
            }
            CurrentGrid.emplace();
            if(PreviousSection != Section::none && PreviousSection != Section::GRID && PreviousSection != Section::INFO){
                CurrentGrid.emplace();
                break;
            }
            else{
                ParserError("Invalid Position for Grid Section");
            }
            break;
        }
    }
}

std::string ScenarioLoader::Trim(std::string str){
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

void ScenarioLoader::ParseAssignment(std::string line){
    int equalPos = line.find('=');
    std::string loe = line.substr(0, equalPos);
    std::string roe = line.substr(equalPos+1, line.size());

    switch(CurrentSection){
        case Section::INFO:
        {
            if(loe == "NAME"){
                CurrentScenario.LoadedScenarioName = roe;
            }
            else if (loe == "FLAG"){
                //TODO Flag Appending
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }
        case Section::BASKET:
        {
            if (loe == "SIZE"){
                CurrentScenario.BallBasket.Size = ParseVector2(roe);
            }
            else if(loe == "START"){
                CurrentScenario.BallBasket.Start = ParseVector2(roe);
            }
            else if(loe == "END"){
                CurrentScenario.BallBasket.End = ParseVector2(roe);
            }
            else if(loe == "SPEED"){
                CurrentScenario.BallBasket.Speed = ParseFloat(roe);
            }
            else if(loe == "ROTATION"){
                CurrentScenario.BallBasket.Rotation = ParseFloat(roe);
            }
            else if(loe == "COLOR"){
                CurrentScenario.BallBasket.BasketColor = ParseColor(roe);
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }
        case Section::CANNON:
        {
            if (loe == "POSITION"){
                CurrentScenario.BallCannon.Position = ParseVector2(roe);
            }
            else if (loe == "BASERAD"){
                CurrentScenario.BallCannon.BaseRad = ParseFloat(roe);
            }
            else if (loe == "POINTERRAD"){
                CurrentScenario.BallCannon.PointerRad = ParseFloat(roe);
            }
            else if (loe == "STRENGTH"){
                CurrentScenario.BallCannon.Strength = ParseFloat(roe);
            }
            else if (loe == "BALLSREMAINING"){
                CurrentScenario.BallCannon.BallsRemaining = ParseFloat(roe);
            }
            else if(loe == "COLOR"){
                CurrentScenario.BallCannon.CannonColor = ParseColor(roe);
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }

        case Section::PEG:
        {
            if (loe == "POSITION"){
                if(roe == "GRID"){
                    CurrentPeg->isGridding = true;
                }
                else{
                    CurrentPeg->isGridding = false;
                    CurrentPeg->Position = ParseVector2(roe);
                    CurrentPeg->hasPosition = true;
                } 
            }
            else if(loe == "RADIUS"){
                CurrentPeg->Radius = ParseFloat(roe);
                CurrentPeg->hasRadius = true;
            }
            else if(loe == "BOUNCINESS"){
                CurrentPeg->Bounciness = ParseFloat(roe);
                CurrentPeg->hasBounciness = true;
            }
            else if(loe == "PEGTYPE"){
                CurrentPeg->PegType = ParsePegType(roe);
                CurrentPeg->hasPegType = true;
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }
        case Section::WALL:
        {
            if (loe == "POSITION"){
                CurrentWall->Position = ParseVector2(roe);
                CurrentWall->hasPosition = true;
            }
            else if(loe == "SIZE"){
                CurrentWall->Size = ParseVector2(roe);
                CurrentWall->hasSize = true;
            }
            else if(loe == "BOUNCINESS"){
                CurrentWall->Bounciness = ParseFloat(roe);
                CurrentWall->hasBounciness = true;
            }
            else if(loe == "COLOR"){
                CurrentWall->WallColor = ParseColor(roe);
                CurrentWall->hasColor = true;
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }
        case Section::BALL:
        {
            if (loe == "POSITION"){
                if(roe == "GRID"){
                    CurrentBall->isGridding = true;
                }
                else{
                    CurrentPeg->isGridding = false;
                    CurrentBall->Position = ParseVector2(roe);
                    CurrentBall->hasPosition = true;
                }
            }
            else if(loe == "VELOCITY"){
                CurrentBall->Velocity = ParseVector2(roe);
                CurrentBall->hasVelocity = true;
            }
            else if(loe == "RADIUS"){
                CurrentBall->Radius = ParseFloat(roe);
                CurrentBall->hasRadius = true;
            }
            else if(loe == "COLOR"){
                CurrentBall->BallColor = ParseColor(roe);
                CurrentBall->hasColor = true;
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }
        case Section::GRID:
        {
            if (loe == "POSITION"){
                CurrentGrid->Position = ParseVector2(roe);
                CurrentGrid->hasPosition = true;
            }
            else if(loe == "SIZE"){
                CurrentGrid->Size = ParseVector2(roe);
                CurrentGrid->hasSize = true;
            }
            else if(loe == "XSIZE"){
                CurrentGrid->XSize = ParseFloat(roe);
                CurrentGrid->hasXSize = true;
            }
            else if(loe == "YSIZE"){
                CurrentGrid->YSize = ParseFloat(roe);
                CurrentGrid->hasYSize = true;
            }
            else if(loe == "OFFSETFACTOR"){
                CurrentGrid->OffsetFactor = ParseFloat(roe);
                CurrentGrid->hasOffsetFactor = true;
            }
            else if(loe == "RANDOMFLIPPED"){
                CurrentGrid->RandomFlipped = ParseFloat(roe);
            }
            else{
                ParserError("'" + loe + "' is not a valid property for its section");
            }
            break;
        }
        case Section::none:
        //UNREACHABLE
        break;
    }

    //printf("----\n %s %s\n----\n", loe.c_str(), roe.c_str());
}

Vector2 ScenarioLoader::ParseVector2(std::string str){
    int commaPos = str.find(',');
    std::string loc = str.substr(0, commaPos);
    std::string roc = str.substr(commaPos+1, str.size());

    return {ParseFloat(loc), ParseFloat(roc)};
}

float ScenarioLoader::ParseFloat(std::string str){
    ReplaceAll(str, "WINX", std::to_string(WindowX));
    ReplaceAll(str, "WINY", std::to_string(WindowY));
    ReplaceAll(str, "MIDX", std::to_string(CenterX));
    ReplaceAll(str, "MIDY", std::to_string(CenterY));

    if(str.find("+")!=std::string::npos){
        int plusPos = str.find('+');
        std::string lop = str.substr(0, plusPos);
        std::string rop = str.substr(plusPos+1, str.size());
        return ParseFloat(lop) + ParseFloat(rop);
    }
    else if(str.find("-")!=std::string::npos){
        int minusPos = str.find('-');
        std::string lom = str.substr(0, minusPos);
        std::string rom = str.substr(minusPos+1, str.size());
        return ParseFloat(lom) - ParseFloat(rom);
    }
    else{
        try {
            return std::stof(str);
          } 
        catch (const std::invalid_argument& e) {
            ParserError("Invalid argument: Could not convert to float.");
            return 0.0f;
        } 
        catch (const std::out_of_range& e) {
            ParserError("Out of range: The value overflows/underflows a float.");
            return 0.0f;
        }
    }
}

ScenarioLoader::Section ScenarioLoader::StrToSection(std::string str){
    Section s = Section::none;
    if (str == "INFO") s = Section::INFO;
    if (str == "BASKET") s = Section::BASKET;
    if (str == "PEG") s = Section::PEG;
    if (str == "WALL") s = Section::WALL;
    if (str == "BALL") s = Section::BALL;
    if (str == "CANNON") s = Section::CANNON;
    if (str == "GRID") s = Section::GRID;
    return s;
}

void ScenarioLoader::ParserError(std::string errorText){
    std::cerr << "ERROR: PMA Parser at line " << LineCounter << " - " << errorText.c_str() << std::endl;
    _parseError = true;
    CurrentScenario.LoadScenario(Scenario::SavedScenarios::EMPTY, WindowX, WindowY);
}


void ScenarioLoader::ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
}

Color ScenarioLoader::ParseColor(const std::string& str)
{
    static const std::unordered_map<std::string, Color> ColorMap = {
    {"RED", RED},
    {"GREEN", GREEN},
    {"BLUE", BLUE},
    {"WHITE", WHITE},
    {"BLACK", BLACK},
    {"YELLOW", YELLOW},
    {"ORANGE", ORANGE},
    {"PURPLE", PURPLE},
    {"PINK", PINK},
    {"BROWN", BROWN},
    {"GRAY", GRAY},
    {"SKYBLUE", SKYBLUE},
    {"LIME", LIME},
    {"GOLD", GOLD},
    {"MAROON", MAROON},
    {"RAYWHITE", RAYWHITE},
    {"HONEYSUCKLE", GetColor(0xDA7DA0FF)},
    {"BABYBLUE", GetColor(0xC1DBEDFF)},
    {"COFFEEPOT", GetColor(0x6E5E5EFF)},
    {"EMERALD", GetColor(0x7BA388FF)}
    };

    auto it = ColorMap.find(str);
    if (it != ColorMap.end())
        return it->second;

    ParserError("'" + str + "' is not a valid Color");
    return WHITE; // default or throw an exception
}

Peg::PegType ScenarioLoader::ParsePegType(std::string str){
    if(str == "DEFAULT"){
        return Peg::PegType::DEFAULT;
    }
    else if(str == "TARGET"){
        return Peg::PegType::TARGET;
    }
    else{
        ParserError("'"+str+"' is not a valid Peg Type");
        return Peg::PegType::DEFAULT;
    }
}