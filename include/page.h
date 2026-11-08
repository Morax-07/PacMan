#pragma once
#include "raylib.h"
#include "global.h"








class Page {
    public: 
        virtual auto draw(void) -> void = 0;
        virtual auto update(float) -> void = 0;
        virtual auto input(void) -> void = 0;

        static Font font;



        Page(void);
        virtual ~Page() = default;
};