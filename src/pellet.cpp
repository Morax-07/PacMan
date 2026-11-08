#include "../include/pellet.h"
#include <raylib.h>







const Color Pellet::color = {255, 184, 174, 255};
bool Pellet::drawFoodFlag = true;
float Pellet::updateFoodInterval = 0.6f;
float Pellet::updateFoodTimer = 0.0f;



//pellet parametrized constructor
Pellet::Pellet(short int radius, Vector2 position): radius(radius), position(position) {
    this->bigFoodFlag = radius > 2;
    this->points = (bigFoodFlag) ? 50 : 10;
}



//pellet default constructor
Pellet::Pellet(): radius(2), position(Vector2 {0, 0}) {
    bigFoodFlag = false;
    points = 10;
}



//draws the pellet 
auto Pellet::draw() -> void {
    if(!drawFoodFlag && bigFoodFlag) return;

    DrawCircleV(position, radius, color);
}



//updates the pellets
auto Pellet::update(float dt) -> void {
    updateFoodTimer += dt;

    if(updateFoodTimer >= updateFoodInterval) {
        updateFoodTimer -= updateFoodInterval;
        drawFoodFlag = !drawFoodFlag;
    }
}



//returns the pellet hitboc
auto Pellet::hitbox() const -> Rectangle {
    return {position.x - radius, position.y - radius, (float)radius * 2, (float)radius * 2};
}



//returns the points for eating this pellet
auto Pellet::getPoints() const noexcept -> int {
    return this->points;
}



//returns if the food is a pig pellet
auto Pellet::isBig() const noexcept -> bool {
    return this->bigFoodFlag;
}