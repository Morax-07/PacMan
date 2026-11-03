#include "../include/menuPage.h"
#include "../include/global.h"
#include <cmath>
#include <raylib.h>
#include "raymath.h"








// CONSTRUCTOR
// assignes values to attributes
MenuPage::MenuPage() {
    this->showingLogo = true;
    this->showingMenu = false;
    this->pageChanging = false;
    this->raylibLogo = LoadTexture("../bin/data/texture/raylibLogo.png");
    this->title[0] = LoadTexture("../bin/data/texture/title1.png");
    this->title[1] = LoadTexture("../bin/data/texture/title2.png");
    this->raylibLogoPos = {
        .x = (float)GetScreenWidth() / 2 - (float)raylibLogo.width / 2, 
        .y = (float)GetScreenHeight() / 2 - (float)raylibLogo.height / 2
    };
    this->titlePos = {
        .x = (float)GetScreenWidth() / 2 - (float)title[0].width / 2, 
        .y = (float)GetScreenHeight() / 2 - (float)title[0].height / 2 - 100
    };
    this->pressStartPos = {
        .x = (float)GetScreenWidth() / 2 - MeasureTextEx(font, "PRESS START", 26, 3).x / 2,
        .y = titlePos.y + title[0].height + 10
    };
    this->raylibLogoAlpha = 0;
    this->logoFadeIn = 2.0f;
    this->logoHold = 2.0f;
    this->logoFadeOut = 2.0f;
    this->logoFadeTimer = 0.0f;
    this->titleTexIndex = 0;
    this->titleTexChangeInerval = 0.6f;
    this->titleTexChangeTimer = 0.0f;
    this->pressStartTimer = 0.0f;
    this->pressStartInterval = 0.5f;
    this->showPressStart = true;
}



// DESTRUCTOR
// unloads textures and other assets
MenuPage::~MenuPage() {
    UnloadTexture(raylibLogo);
    UnloadTexture(title[0]);
    UnloadTexture(title[1]);
}



// draws the logo and the main menu
void MenuPage::draw() {
    if(showingLogo) DrawTextureV(
        raylibLogo, 
        raylibLogoPos, 
        Color{255, 255,255, raylibLogoAlpha}
    );
    if(showingMenu) {
        for(auto& c : confettis) c.draw();
        DrawTextureV(title[titleTexIndex], titlePos, WHITE);

        if(showPressStart) DrawTextEx(
            font, 
            "PRESS START", 
            pressStartPos, 
            26, 
            3, 
            Color{244, 193, 15, 255}
        );
    }
}



//checks inputs in the main page
void MenuPage::input() {
    if(!Global::inputAvailable) return;
    
    if(IsKeyPressed(KEY_ENTER) && showingLogo) {
        showingLogo = false;
        showingMenu = true;
    } else if(IsKeyPressed(KEY_ENTER) && showingMenu) {
        pageChanging = true;
    }
}



// returns the total time for logo fading
float MenuPage::logoTotalFadeTime() const noexcept {
    return this->logoFadeIn + this->logoHold + this->logoFadeOut + 0.5f;
}



// updates the logo showing and the main menu
void MenuPage::update(float dt) {
    if(showingLogo) {
        logoFadeTimer += dt;

        if(logoFadeTimer >= logoTotalFadeTime()) {
            showingLogo = false;
            showingMenu = true;
        }


        // logo's alpha channel update
        float t = 0.0f;
        if(logoFadeTimer < logoFadeIn) t = logoFadeTimer / logoFadeIn;
        else if(logoFadeTimer < logoFadeIn + logoHold) t = 1.0f;
        else t = 1.0f - (logoFadeTimer - logoFadeIn - logoHold) / logoFadeOut;

        t = Clamp(t, 0.0f, 1.0f);
        raylibLogoAlpha = (unsigned char)Lerp(0.0f, 255.0f, (1.0f - cosf(PI * t)) * 0.5);
    }




    if(showingMenu) {
        for(auto& c : confettis) c.update(dt);

        titleTexChangeTimer += dt;
        if(titleTexChangeTimer >= titleTexChangeInerval) {
            titleTexChangeTimer -= titleTexChangeInerval;
            titleTexIndex = 1 - titleTexIndex;
        }

        pressStartTimer += dt;
        if(pressStartTimer >= pressStartInterval) {
            pressStartTimer -= pressStartInterval;
            showPressStart = !showPressStart;
        }
    }
}



// signals if the page has to be changed
bool MenuPage::isPageChanging() const noexcept {
    return this->pageChanging;
}