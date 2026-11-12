#include "../include/pinkGhost.h"
#include <raylib.h>








//pinkGhost destructor
PinkGhost::PinkGhost(): Ghost() {
    this->state = GhostState::caged;
    this->minPelletsForSpown = 0;
    this->position = Vector2{14 * 22 - (float)Ghost::width * 1.75f, 14.5f * 22 - (float)Ghost::height / 2};
}



//pinkGhost destructor
PinkGhost::~PinkGhost() {}



//sets the pinkGhost scatter target
auto PinkGhost::setScatterTarget() -> void {
    this->scatterTarget = Vector2{55, -77};
}



//updates the pinkGhost chase target
auto PinkGhost::updateChaseTarget() -> void {
    switch(*Ghost::playerMovement) {
        case Movement::up:
            target = Vector2{playerPosition->x + 17 - 22 * 4, playerPosition->y + 16 - 22 * 4};
            break;

        case Movement::down:
            target = Vector2{playerPosition->x + 17, playerPosition->y + 16 + 22 * 4};
            break;

        case Movement::left:
            target = Vector2{playerPosition->x + 17 - 22 * 4, playerPosition->y + 16};
            break;

        case Movement::right:
            target = Vector2{playerPosition->x + 17 + 22 * 4, playerPosition->y + 16};
            break;

        default:
            target = Vector2{playerPosition->x + 17 - 22 * 4, playerPosition->y + 16 - 22 * 4};
            break;
    }
}



//debug method to draw pinkGhost target
auto PinkGhost::drawTarget() -> void {
    DrawCircleV(target, 10, PINK);
}