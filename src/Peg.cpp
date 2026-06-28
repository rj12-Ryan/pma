#include "Peg.h"

Peg::Peg(Vector2 position, int radius, float bounciness, Peg::PegType pegType) 
    :Position(position), Radius(radius), Bounciness(bounciness), CurrentPegType(pegType)
    {}