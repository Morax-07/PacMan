#pragma once
#include "raylib.h"
#include "global.h"








class Page {
    public: 
        virtual void draw(void) = 0;
        virtual void update(float) = 0;
        virtual void input(void) = 0;

        static Font font;



        Page(void);
        virtual ~Page() = default;
};