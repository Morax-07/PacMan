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

    pg.setScatterTarget();
    rg.setScatterTarget();
    bg.setScatterTarget();
    og.setScatterTarget();

    pg.setGhostTexture("../bin/data/texture/pinkGhost.png");
    rg.setGhostTexture("../bin/data/texture/redGhost.png");
    bg.setGhostTexture("../bin/data/texture/blueGhost.png");
    og.setGhostTexture("../bin/data/texture/orangeGhost.png");

    


    this->coverRec = {(float)map.width, 0, (float)GetScreenWidth() - map.width, (float)GetScreenHeight()};
    this->paused = false;
    this->level = 1;
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
    rg.draw();
    pg.draw();
    bg.draw();
    og.draw();

    

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

    map.update(dt);
    pacman.update(dt);

    Ghost::updateFrightenedMode(dt);
    rg.update(dt);
    pg.update(dt);
    bg.update(dt);
    og.update(dt);

    Global::bigPelletEaten = false;

    pacman.checkPelletsCollision();
    pacman.checkBodyBlocksCollision();
}



//handles main page input
auto MainPage::input() -> void{
    if(IsKeyPressed(KEY_P)) paused = !paused;

    if(paused) return;

    pacman.input();
}