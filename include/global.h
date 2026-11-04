#pragma once 
#include "raylib.h"



#define WIDTH 916
#define HEIGHT 682

typedef unsigned long long int ULLInt;



enum class PageType {typeMenuPage, typeMainPage};
enum class Movement {still, up, down, left, right};
enum class GhostState {caged, scatter, chase, frightened, eaten};





namespace Global {
    extern const Color pacBlue;
    extern bool inputAvailable;
}



