#include "../include/pinkGhost.h"
#include <raylib.h>







PinkGhost::PinkGhost(): Ghost() {
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 11.5f * 22 - (float)Ghost::height / 2};
}

PinkGhost::~PinkGhost() {}

auto PinkGhost::setScatterTarget() -> void {
    this->scatterTarget = Vector2{55, -77};
}

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