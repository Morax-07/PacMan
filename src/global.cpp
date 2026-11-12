#include "./../include/global.h"
#include <cmath>







const Color Global::pacBlue = {63, 72, 204, 255};
bool Global::inputAvailable = true;
bool Global::bigPelletEaten = false;
const bool Global::visualTargetDebug = false;
bool Global::stopForEatenGhost = false;
short Global::ghostEaten = 0;
int Global::ghostPoints = 0;
Vector2 Global::ghostPointsDrawingPosition{0, 0};



auto Global::sign(float x) -> float {
    return std::fabs(x) / x;
}