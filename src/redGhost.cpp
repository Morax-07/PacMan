#include "../include/redGhost.h"
#include <raylib.h>










RedGhost::RedGhost(): Ghost() {
    this->movement = Movement::left;
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 11.5f * 22 - (float)Ghost::height / 2};
}

RedGhost::~RedGhost() {}

auto RedGhost::setScatterTarget() -> void {
    this->scatterTarget = {Ghost::mapDimension.x - 55, -77};
}

auto RedGhost::updateChaseTarget() -> void {
    target = Vector2{playerPosition->x + 17, playerPosition->y + 16};
}