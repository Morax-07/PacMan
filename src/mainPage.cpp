#include <memory>
#include <raylib.h>
#include "../include/mainPage.h"










//MainPage's constructor
//sets some player, ghost and game attributes
MainPage::MainPage(): Page() {
    pacman.setPellets(map.getPellets());
    pacman.setBlocks(map.getBlocks());
    pacman.setRightPositionBound(map.width);

    Ghost::setPlayerPosition(pacman.getPosition()); 
    Ghost::setPlayerMovement(pacman.getMovement());
    Ghost::setBlocks(map.getBlocks());
    Ghost::setMapDimension(Vector2{(float)map.width, (float)map.height});
    Ghost::setEatenPellets(pacman.getEatenPellets());


    //creates unique pointer to all ghosts types
    auto red    = std::make_unique<RedGhost>();
    auto pink   = std::make_unique<PinkGhost>();
    auto blue   = std::make_unique<BlueGhost>();
    auto orange = std::make_unique<OrangeGhost>();

    //sets the ghosts textures
    red->setGhostTexture("../bin/data/texture/redGhost.png");
    pink->setGhostTexture("../bin/data/texture/pinkGhost.png");
    blue->setGhostTexture("../bin/data/texture/blueGhost.png");
    orange->setGhostTexture("../bin/data/texture/orangeGhost.png");

    //sets the red position pointer in the blue ghost
    blue->setRedGhostPosition(red->getPosition());


    //moves the ghosts in the array
    this->ghosts = {
        std::move(red),
        std::move(pink),
        std::move(blue),
        std::move(orange),
    };

    //sets the ghosts scatter targets
    for(auto& ghost : ghosts) {
        ghost->setScatterTarget();
    }



    this->coverRec = {(float)map.width, 0, (float)GetScreenWidth() - map.width, (float)GetScreenHeight()};
    this->paused = false;
    this->level = 1;
    this->stopForEatenGhostInterval = 0.5f;
    this->stopForEatenGhostTimer = 0.0f;
}



//MainPage's destructor
//unload all ghost's static textures
MainPage::~MainPage() {
    Ghost::unloadStaticTextures();
}



//draws the main page
auto MainPage::draw() -> void {
    map.draw();
    pacman.draw();
    

    for(auto& ghost : ghosts) {
        ghost->draw();
    }


    if(Global::stopForEatenGhost) {
        const Vector2 textDimension = MeasureTextEx(
            font, 
            TextFormat("%i", Global::ghostPoints), 
            15, 
            1
        );
        const Vector2 pointPosition = {
            .x = Global::ghostPointsDrawingPosition.x - textDimension.x / 2,
            .y = Global::ghostPointsDrawingPosition.y
        };

        drawOutlineText(
            TextFormat("%i", Global::ghostPoints), 
            pointPosition, 
            15, 
            1,
            RAYWHITE
        );
    }


    //draws all ghosts targets for visual debug
    if(Global::visualTargetDebug) {
        for(auto& ghost : ghosts) {
            ghost->drawTarget();
        }
    }
    

    

    //covers pacman and ghosts during pacman effect
    DrawRectangleRec(coverRec, BLACK);



    //draws left and right border of the map
    DrawLineEx(
        Vector2{(float)map.width - 1, (float)Block::getHeight()}, 
        Vector2{(float)map.width - 1, (float)GetScreenHeight() - Block::getHeight()}, 
        2, 
        Global::pacBlue
    );
    DrawLineEx(
        Vector2{1, (float)Block::getHeight()}, 
        Vector2{1, (float)GetScreenHeight() - Block::getHeight()}, 
        2, 
        Global::pacBlue
    );



    //draws the points
    DrawTextEx(
        font, 
        TextFormat("POINTS: %i", pacman.getPoints()), 
        Vector2{626, 30}, 
        20, 
        3, 
        WHITE
    );

    

    //draws game name
    DrawTextEx(
        font, 
        "PAC-MAN", 
        Vector2{
            .x = coverRec.x + (coverRec.width / 2 - MeasureTextEx(font, "PAC-MAN", 35, 3).x / 2),
            .y = coverRec.y + 10
        }, 
        35, 
        3, 
        Color{244, 193, 15, 255}
    );



    //draws paused 
    if(paused) {
        DrawTextEx(
            font, 
            "PAUSED ||", 
            Vector2{
                .x = coverRec.x + (coverRec.width / 2 - MeasureTextEx(font, "PAUSED ||", 15, 3).x / 2),
                .y = coverRec.y + 20 + MeasureTextEx(font, "PAC-MAN", 35, 3).y 
            }, 
            15, 
            3, 
            WHITE
        );
    }



    //draws current player's life
    pacman.drawLife(Vector2{(float)map.width + 10, (float)map.height - 40});
    DrawFPS(0, 0);
}



//updates the main page
auto MainPage::update(float dt) -> void {
    if(paused) return;


    //if the ghost is eaten stop the game for a while
    if(Global::stopForEatenGhost) {
        stopForEatenGhostTimer += dt;

        if(stopForEatenGhostTimer >= stopForEatenGhostInterval) {
            stopForEatenGhostTimer = 0.0f;
            Global::stopForEatenGhost = false;
        }

        return;
    }


    map.update(dt);
    pacman.update(dt);

    Ghost::updateFrightenedMode(dt);
    
    for(auto& ghost : ghosts) {
        if(pacman.isNotEating() && !ghost->spowned()) {
            ghost->forceSpown();
            pacman.resetNotEating();
        }

        ghost->update(dt);
    }


    //if a big pellet is eaten and the number of ghost eaten is different from 0, it is set to 0
    //gets the ghost eating points back to 200 every time a big pellet is eaten
    if(Global::bigPelletEaten && Global::ghostEaten != 0) Global::ghostEaten = 0;

    Global::bigPelletEaten = false;



    collisions();
}



//handles main page input
auto MainPage::input() -> void{
    if(IsKeyPressed(KEY_P)) paused = !paused;

    if(paused) return;

    pacman.input();
}



auto MainPage::collisions() -> void {
    pacman.checkPelletsCollision();
    pacman.checkBodyBlocksCollision();


    for(auto& ghost : ghosts) {
        pacman.checkGhostCollision(ghost.get());
    }
}



//draws a text with an outline
auto MainPage::drawOutlineText(const char* text, const Vector2& position, float size, float spacing, Color color, Color outline) -> void {
    for(int dx = -1; dx <= 1; dx++) {
        for(int dy = -1; dy <= 1; dy++) {
            DrawTextEx(
                font, 
                text, 
                Vector2{position.x + dx, position.y + dy}, 
                size, 
                spacing, 
                outline
            );
        }
    }
    DrawTextEx(font, text, position, size, spacing, color);   
}