#pragma once 
#include "raylib.h"



#define WIDTH 916
#define HEIGHT 682

typedef unsigned long long int ULLInt;



enum class PageType {typeMenuPage, typeMainPage};
enum class Movement {still, up, down, left, right};





namespace Global {
    extern const Color pacBlue;
    extern bool inputAvailable;


    template<typename T> bool elapsedDelayFrom(double delay, T& lastUpdate) {
        double time = GetTime();
    
        if (time - lastUpdate >= delay) {
            lastUpdate = time;
            return true;
        } 
        else {
            return false;
        }
    }
}



