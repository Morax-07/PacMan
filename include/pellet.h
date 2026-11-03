#pragma once
#include "raylib.h"







class Pellet final {
    private:
        short int radius;
        static const Color color;
        bool bigFoodFlag;
        int points;
        Vector2 position;
        static bool drawFoodFlag;
        static float updateFoodInterval;
        static float updateFoodTimer;

    
    public:
        void draw(void);
        static void update(float);
        Rectangle hitbox(void) const;
        int getPoints(void) const noexcept;


        Pellet(short int, Vector2);
        Pellet(void);
};