#include "../include/orangeGhost.h"
#include <raylib.h>
#include "raymath.h"






OrangeGhost::OrangeGhost(): Ghost() {
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 11.5f * 22 - (float)Ghost::height / 2};
}

OrangeGhost::~OrangeGhost() {}

auto OrangeGhost::setScatterTarget() -> void {
    this->scatterTarget = Vector2{55, mapDimension.y + 77};
}

auto OrangeGhost::updateChaseTarget() -> void {
    const float distanceFromPlayer = std::hypotf(
        (playerPosition->x + 17) - (position.x + (float)Ghost::width / 2),
        (playerPosition->y + 16) - (position.y + (float)Ghost::height / 2)
    );

    if(distanceFromPlayer <= 22 * 8) {
        target = scatterTarget;
        return;
    }


    target = Vector2{playerPosition->x + 17, playerPosition->y + 16};
}

auto OrangeGhost::drawTarget() -> void {
    if(Vector2Equals(target, scatterTarget)) {
        DrawCircleV({11, mapDimension.y - 11}, 10, ORANGE);
    } else {
        DrawCircleLinesV({
        target
        }, 15, ORANGE);
    }
    DrawCircleLinesV({
        playerPosition->x + 17,
        playerPosition->y + 16
    }, 22*8, ORANGE);
}