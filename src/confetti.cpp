#include "./../include/confetti.h"






//Confetti's constructor
Confetti::Confetti(): 
    width(4), height(4), speed(GetRandomValue(60, 480)), 
    color(Color {(unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), (unsigned char)GetRandomValue(0, 255), 255}) 
{
    this->position = Vector2 {(float)GetRandomValue(0, GetScreenWidth() - 4), (float)GetRandomValue(-GetScreenHeight(), -4)};
}

//Confetti's draw method
void Confetti::draw() {
    DrawRectangleV(position, Vector2 {(float)width, (float)height}, color);
}

//Confetti's update method
void Confetti::update(float dt) {
    position.y += speed * dt;

    if(position.y >= GetScreenHeight() - 4) position.y = -4;
}