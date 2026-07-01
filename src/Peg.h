#ifndef PEG_H
#define PEG_H

#include "pma.h"
#include<raylib.h>

class Peg{
    public:
        PegID ID;
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