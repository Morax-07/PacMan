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
        static auto update(float) -> void;
        auto draw(void) -> void;
        auto hitbox(void) const -> Rectangle;
        auto getPoints(void) const noexcept -> int;
        auto isBig(void) const noexcept -> bool;


        Pellet(short int, Vector2);
        Pellet(void);
        ~Pellet() = default;
};