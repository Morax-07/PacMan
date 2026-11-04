#include <raylib.h>
#include "../include/mainPage.h"









MainPage::MainPage(): Page() {
    pacman.setPellets(map.getPellets());
    pacman.setBlocks(map.getBlocks());
    pacman.setRightPositionBound(map.width);

    Ghost::setPlayerPosition(pacman.getPosition()); 
    Ghost::setBlocks(map.getBlocks());
    Ghost::setMapDimension(Vector2{(float)map.width, (float)map.height});

    g.setGhostTexture("../bin/data/texture/redGhost.png");
    g.setScatterTarget();

    


    this->coverRec = {(float)map.width, 0, (float)GetScreenWidth() - map.width, (float)GetScreenHeight()};
    this->paused = false;
    this->level = 1;
}

MainPage::~MainPage() {
    Ghost::unloadStaticTextures();
}

void MainPage::draw() {
    map.draw();
    pacman.draw();
    g.draw();

    

    DrawRectangleRec(coverRec, BLACK);

    DrawLineEx(
        Vector2{(float)map.width - 1, (float)Block::getHeight()}, 
        Vector2{(float)map.width - 1, (float)GetScreenHeight() - Block::getHeight()}, 
        2, 
        Global::pacBlue
    );

    DrawTextEx(
        font, 
        TextFormat("POINTS: %i", pacman.getPoints()), 
        Vector2{626, 30}, 
        20, 
        3, 
        WHITE
    );

    DrawLineEx(
        Vector2{1, (float)Block::getHeight()}, 
        Vector2{1, (float)GetScreenHeight() - Block::getHeight()}, 
        2, 
        Global::pacBlue
    );

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

    pacman.drawLife(Vector2{(float)map.width + 10, (float)map.height - 40});
    DrawFPS(0, 0);
}

void MainPage::update(float dt) {
    if(paused) return;

    map.update(dt);
    pacman.update(dt);
    g.update(dt);

    pacman.checkPelletsCollision();
    pacman.checkBodyBlocksCollision();
}

void MainPage::input() {
    if(IsKeyPressed(KEY_P)) paused = !paused;

    if(paused) return;

    pacman.input();
}