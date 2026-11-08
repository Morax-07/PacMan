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
        auto update(float) -> void;
        auto draw(void) -> void;

        Confetti(void);
        ~Confetti() = default;
};