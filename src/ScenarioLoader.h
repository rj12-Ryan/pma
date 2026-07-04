#ifndef SCENARIOLOADER_H
#define SCENARIOLOADER_H

#include "pma.h"
#include <fstream>
#include <optional>
#include "Scenario.h"

class ScenarioLoader{
    private:
        bool _parseError = false;
    public:
        std::ifstream File;
        Scenario& CurrentScenario;
        float WindowX, WindowY, CenterX, CenterY;
        ScenarioLoader(std::string path, Scenario& currentScenario, float windowX, float windowY);
        int LineCounter;
        void Parse();
        enum class Section
        {
            none,
            INFO,
            BASKET,
            PEG,
            WALL,
            BALL,
            GRID
        };
        Section CurrentSection = Section::none;
        Section ParseSectionName(std::string line);
        std::string Trim(std::string);
        Section StrToSection(std::string);
        bool IsSection(std::string line);
        bool IsAssignment(std::string line);
        void FinishCurrentSection();
        void StartNewSection(std::string line);
        void ParseAssignment(std::string line);
        Vector2 ParseVector2(std::string str);
        float ParseFloat(std::string str);
        void ParserError(std::string errorText);
        void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
        Color ParseColor(const std::string& str);
        struct BallConfig {
            Vector2 Position{};
            Vector2 Velocity{};
            int Radius = 10;
            Color BallColor = RED;

            bool hasPosition = false;
            bool hasVelocity = false;
            bool hasRadius = false;
            bool hasColor = false;

            Ball Build() const {
                return Ball(Position, Velocity, Radius, BallColor);
            }
        
            bool IsComplete() const {
                return hasPosition &&
                       hasVelocity &&
                       hasRadius &&
                       hasColor;
            }
        };
        std::optional<BallConfig> CurrentBall;
};

#endif