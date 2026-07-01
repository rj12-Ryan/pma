#include "Basket.h"

Basket::Basket(Vector2 size, Vector2 start, Vector2 end, float speed, float rotation, Color basketColor)
    :Size(size), Start(start), End(end), Speed(speed), Rotation(rotation), BasketColor(basketColor)
{}

bool Basket::IsMoving(){
    return _moving;
}

void Basket::BeginMoving(){
    _moving = true;
}

void Basket::PauseMoving(){
    _moving = false;
}