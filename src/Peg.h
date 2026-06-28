#ifndef PEG_H
#define PEG_H

#include<raylib.h>
class Peg{
    public:
        enum class PegType{
            TARGET,
            DEFAULT
        };
        PegType CurrentPegType;
        Vector2 Position;
        int Radius;
        float Bounciness;
        bool Hit = false;
        Peg(Vector2, int, float, PegType);
};

#endif