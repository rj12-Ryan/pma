#ifndef UI_H
#define UI_H

#include<string>
#include<raylib.h>

//forward declare Scenario
class Scenario;

class UI{
    private:
        bool _statusBarEnabled;
        int _statusBarHeight;
        int _windowX;
        int _windowY;
        void _drawStatusBar(Scenario&);
        void _drawPopup(Scenario&);
        int _drawCounter = 0;
    public:
        enum class UIMode{
            DEVELOPER,
            CANNON,
            POPUP
        };
        UIMode CurrentMode = UIMode::CANNON;
        std::string CurrentModeStr = "CANNON";
        UI(int, int, bool, int, int);
        int WindowX();
        int WindowY();
        void InitPMAWindow();
        void ToggleStatus();
        void Draw(Scenario&);
        void ProcessInput(Scenario&);
        int DesiredScenario;
};

#endif

