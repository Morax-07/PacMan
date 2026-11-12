#include "../include/blueGhost.h"
#include <raylib.h>










//blueGhost constructor
BlueGhost::BlueGhost(): Ghost() {
    this->redGhostPosition = nullptr;
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 14.5f * 22 - (float)Ghost::height / 2};
    this->state = GhostState::caged;
    this->minPelletsForSpown = 30;
    this->playerDirection = {0, 0};
}



//blueGhost destructor
BlueGhost::~BlueGhost() {}



//sets the redGhost scatter target
auto BlueGhost::setScatterTarget() -> void {
    this->scatterTarget = Vector2{mapDimension.x - 55, mapDimension.y + 77};
}



//updates the redGhost chase target
auto BlueGhost::updateChaseTarget() -> void {
    switch(*Ghost::playerMovement) {
        case Movement::up:
            playerDirection = Vector2{playerPosition->x + 17 - 22 * 2, playerPosition->y + 16 - 22 * 2};
            break;

        case Movement::down:
            playerDirection = Vector2{playerPosition->x + 17, playerPosition->y + 16 + 22 * 2};
            break;

        case Movement::left:
            playerDirection = Vector2{playerPosition->x + 17 - 22 * 2, playerPosition->y + 16};
            break;

        case Movement::right:
            playerDirection = Vector2{playerPosition->x + 17 + 22 * 2, playerPosition->y + 16};
            break;

        default:
            playerDirection = Vector2{playerPosition->x + 17 - 22 * 2, playerPosition->y + 16 - 22 * 2};
            break;
    }

    


    target = {
        .x = playerDirection.x - ((redGhostPosition->x + (float)Ghost::width / 2) - playerDirection.x),
        .y = playerDirection.y - ((redGhostPosition->y + (float)Ghost::height / 2) - playerDirection.y)
    };
}



//sets a pointer to the redGhost position
auto BlueGhost::setRedGhostPosition(const Vector2* redGhostPosition) -> void {
    this->redGhostPosition = redGhostPosition;
}



//debug method to draw blueGhost target
auto BlueGhost::drawTarget(void) -> void {
    DrawCircleV(target, 10, BLUE);
    DrawCircleV(playerDirection, 5, WHITE);
    DrawLine(redGhostPosition->x + (float)Ghost::width / 2, redGhostPosition->y + (float)Ghost::height / 2, playerDirection.x, playerDirection.y, WHITE);
    DrawLineV(playerDirection, target, BLUE);
}