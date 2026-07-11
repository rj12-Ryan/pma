#ifndef PHYSICS_ENGINE_H
#define PHYSICS_ENGINE_H

#include<raylib.h>
#include "Scenario.h"

class PhysicsEngine{
    private:
        float _accumulator = 0.0f;
        void _updatePhysics();
        void _ballWallCollision();
        void _ballPegCollision();
        void _ballBasketCollision();
        void _moveBasket();
        void _updateCannon();
    public:
        int Gravity;
        float DT;
        Scenario& CurrentScenario;
        PhysicsEngine(int, float, Scenario&);
        void Step();
};


#endif