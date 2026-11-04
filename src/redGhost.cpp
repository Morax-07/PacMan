#include "../include/redGhost.h"
#include <raylib.h>










RedGhost::RedGhost(): Ghost() {
    this->movement = Movement::left;
    this->position = Vector2{14 * 22 - (float)Ghost::width / 2, 11.5f * 22 - (float)Ghost::height / 2};
}

RedGhost::~RedGhost() {}

void RedGhost::setScatterTarget() {
    this->scatterTarget = {Ghost::mapDimension.x, -100};

    if(state == GhostState::scatter) this->target = scatterTarget;
}

void RedGhost::updateTarget() {
    target = *playerPosition;
}