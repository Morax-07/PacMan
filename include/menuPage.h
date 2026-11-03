#pragma once
#include <array>
#include <raylib.h>
#include "page.h"
#include "confetti.h"






class MenuPage final: public Page {
    private:
        std::array<Confetti, 110> confettis;
        Texture2D title[2];
        Texture2D raylibLogo;
        Vector2 raylibLogoPos;
        Vector2 titlePos;
        Vector2 pressStartPos;
        unsigned char raylibLogoAlpha;
        bool showingLogo;
        bool showingMenu;
        bool pageChanging;
        float logoFadeIn;
        float logoHold;
        float logoFadeOut;
        float logoFadeTimer;
        short titleTexIndex;
        float titleTexChangeInerval;
        float titleTexChangeTimer;
        float pressStartTimer;
        float pressStartInterval;
        bool showPressStart;


        float logoTotalFadeTime(void) const noexcept;


    public:
        void draw(void) override;
        void update(float) override;
        void input(void) override;
        bool isPageChanging(void) const noexcept;



        MenuPage(void);
        ~MenuPage() override;
};