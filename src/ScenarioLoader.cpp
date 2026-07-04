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
        if(_parseError){
            //Load Empty On Error
            CurrentScenario.LoadScenario(Scenario::SavedScenarios::EMPTY, WindowX, WindowY);
        }
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
}

bool ScenarioLoader::IsSection(std::string line){
    std::unordered_set<std::string> valid_sections = {"INFO", "BASKET", "PEG", "WALL", "BALL", "GRID"};
    return valid_sections.count(line) == 1;
}

bool ScenarioLoader::IsAssignment(std::string line){
    return (line.find('=') != std::string::npos);
}

void ScenarioLoader::FinishCurrentSection(){
    switch(CurrentSection){
        case Section::BALL:{
            if(!CurrentBall->IsComplete()){
                ParserError("Incomplete Ball defintion");
                break;
            }
            CurrentScenario.NewBall(CurrentBall->Build());
            break;
        }
    }
    CurrentSection = Section::none;
}

void ScenarioLoader::StartNewSection(std::string line){
    CurrentSection = StrToSection(line);
    switch(CurrentSection){
        case Section::BALL:{
            CurrentBall.emplace();
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
        case Section::PEG:
        {
            break;
        }
        case Section::WALL:
        {
            break;
        }
        case Section::BALL:
        {
            if (loe == "POSITION"){
                CurrentBall->Position = ParseVector2(roe);
                CurrentBall->hasPosition = true;
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
    if (str == "GRID") s = Section::GRID;
    return s;
}

void ScenarioLoader::ParserError(std::string errorText){
    std::cerr << "ERROR: PMA Parser at line " << LineCounter << " - " << errorText.c_str() << std::endl;
    _parseError = true;
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
    {"RAYWHITE", RAYWHITE}
    };

    auto it = ColorMap.find(str);
    if (it != ColorMap.end())
        return it->second;

    ParserError("'" + str + "' is not a valid Color");
    return WHITE; // default or throw an exception
}