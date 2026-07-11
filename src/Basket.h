#ifndef BASKET_H
#define BASKET_H

#include<raylib.h>
#include<vector>
#include "pma.h"

class Basket{
    private:
        bool _moving;
    public:
        Basket(Vector2 size, Vector2 start, Vector2 end, float speed, float rotation, Color basketColor);
        Basket();
        bool Rendered;
        Vector2 Size;
        Vector2 Start;
        Vector2 End;
        Vector2 Position = Start;
        float Speed;
        float Rotation;
        Color BasketColor;
        float Age = 0; //physics steps
        bool IsMoving();
        void BeginMoving();
        void PauseMoving();
};

#endif
