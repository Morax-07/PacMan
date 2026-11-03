#pragma once
#include "raylib.h"







class Confetti {
    private:
        const int width;
        const int height;
        const int speed;
        const Color color;
        Vector2 position;



    public:
        void update(float);
        void draw(void);

        Confetti(void);
};