#ifndef UI_H
#define UI_H

//forward declare Scenario to avoid compiler loop
class Scenario;

class UI{
    private:
        bool _statusBarEnabled;
        int _statusBarHeight;
        int _windowX;
        int _windowY;
        void _drawStatusBar(Scenario&);
        int _drawCounter = 0;
    public:
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

