#include <raylib.h>
#include "../include/mainPage.h"









MainPage::MainPage(): Page() {
    pacman.setPellets(map.getPellets());
    pacman.setBlocks(map.getBlocks());
}

MainPage::~MainPage() {}

void MainPage::draw() {
    map.draw();
    pacman.draw();
    DrawTextEx(
        font, 
        TextFormat("POINTS: %i", pacman.getPoints()), 
        Vector2{626, 30}, 
        20, 
        3, 
        WHITE
    );
    pacman.drawLife(Vector2{626, 642});
}

void MainPage::update(float dt) {
    map.update(dt);
    pacman.update(dt);
    pacman.checkPelletsCollision();
    pacman.checkBodyBlocksCollision();
}

void MainPage::input() {
    pacman.input();
}