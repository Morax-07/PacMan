#include "../include/blueGhost.h"
#include <raylib.h>







BlueGhost::BlueGhost(): Ghost() {
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 11.5f * 22 - (float)Ghost::height / 2};
}

BlueGhost::~BlueGhost() {}

auto BlueGhost::setScatterTarget() -> void {
    this->scatterTarget = Vector2{mapDimension.x - 55, mapDimension.y + 77};
}

auto BlueGhost::updateChaseTarget() -> void {
    // Vector2 playerDirection;

    // switch(*Ghost::playerMovement) {
    //     case Movement::up:
    //         playerDirection = Vector2{playerPosition->x + 17 - 22 * 2, playerPosition->y + 16 - 22 * 2};
    //         break;

    //     case Movement::down:
    //         playerDirection = Vector2{playerPosition->x + 17, playerPosition->y + 16 + 22 * 2};
    //         break;

    //     case Movement::left:
    //         playerDirection = Vector2{playerPosition->x + 17 - 22 * 2, playerPosition->y + 16};
    //         break;

    //     case Movement::right:
    //         playerDirection = Vector2{playerPosition->x + 17 + 22 * 2, playerPosition->y + 16};
    //         break;

    //     default:
    //         playerDirection = Vector2{playerPosition->x + 17 - 22 * 2, playerPosition->y + 16 - 22 * 2};
    //         break;
    // }


    // target = {
    //     .x = playerPosition->x + 17 - (playerDirection.x - (playerPosition->x + 17)),
    //     .y = playerPosition->y + 16 - (playerDirection.y - (playerPosition->y + 16))
    // };
}