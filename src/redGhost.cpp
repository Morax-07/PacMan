#include "../include/redGhost.h"
#include <raylib.h>









//redGhost constructor
RedGhost::RedGhost(): Ghost() {
    this->movement = Movement::left;
    this->state = GhostState::scatter;
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 11.5f * 22 - (float)Ghost::height / 2};
}



//redGhost destructor
RedGhost::~RedGhost() {}



//sets redGhost scatter target
auto RedGhost::setScatterTarget() -> void {
    this->scatterTarget = {Ghost::mapDimension.x - 55, -77};
}



//updates the redGhost chase target
auto RedGhost::updateChaseTarget() -> void {
    target = Vector2{playerPosition->x + 17, playerPosition->y + 16};
}



//returns a pointer to the redGhost position
auto RedGhost::getPosition() -> const Vector2* {
    return &this->position;
}



//debug method to draw redGhost target
auto RedGhost::drawTarget() -> void {
    DrawCircleV(target, 10, RED);
}