#include "ScenarioLoader.h"
#include <iostream>
#include <unordered_set>
#include <string>
#include <algorithm> 

ScenarioLoader::ScenarioLoader(std::string path, Scenario& currentScenario, int windowX, int windowY)
    : CurrentScenario(currentScenario), WindowX(windowX), WindowY(windowY)
{
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
        if(IsSection(line)){
            FinishCurrentSection();
            StartNewSection(line);
            continue;
        }

        if(IsAssignment(line)){
            if(CurrentSection == Section::none){
                ParserError("Cannot assign outside of section");
            }
            //printf("Assignment: %s\n", line.c_str());
            ParseAssignment(line);
        }
    }   
}

bool ScenarioLoader::IsSection(std::string line){
    std::unordered_set<std::string> valid_sections = {"INFO", "BASKET", "PEG", "WALL", "GRID"};
    return valid_sections.count(line) == 1;
}

bool ScenarioLoader::IsAssignment(std::string line){
    return (line.find('=') != std::string::npos);
}

void ScenarioLoader::FinishCurrentSection(){
    CurrentSection = Section::none;
}

void ScenarioLoader::StartNewSection(std::string line){
    CurrentSection = StrToSection(line);
    //printf("Start new section: %s\n", line.c_str());
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
    try {
        return std::stof(str);
    } 
    catch (const std::invalid_argument& e) {
        ParserError("ParseFloat, No conversion could be performed");
        return 0.0f;
    } 
    catch (const std::out_of_range& e) {
        ParserError("ParseFloat, Value is out of range for a float");
        return 0.0f;
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