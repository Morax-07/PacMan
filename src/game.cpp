#include "raylib.h"
#include "./../include/game.h"
#include "./../include/menuPage.h"
#include "./../include/mainPage.h"
#include "./../include/global.h"
#include <memory>













Game::Game() {
    //initialize the window
    InitWindow(WIDTH, HEIGHT, "Pac-Man");
    HideCursor();
    SetExitKey(KEY_NULL);

    this->currentPageType = PageType::typeMenuPage;
    this->page = makePage(currentPageType);

    run();
}

Game::~Game() {
    page.reset();
    UnloadFont(Page::font);
    CloseWindow();
}

void Game::run() {
    //game loop
    while(!WindowShouldClose()) {
        input();
        update();


        BeginDrawing();
            ClearBackground(BLACK);
            draw();
        EndDrawing();
    }
}

void Game::draw() {
    page->draw();
}

void Game::update() {
    float dt = std::min(GetFrameTime(), 0.05f);

    // is game is in the menu and it says that the page is changing, the game make a new page (main)
    if(currentPageType == PageType::typeMenuPage && dynamic_cast<MenuPage*>(page.get())->isPageChanging()) {
        page = makePage(PageType::typeMainPage);
        currentPageType = PageType::typeMainPage;
    }

    page->update(dt);
}

void Game::input() {
    page->input();
}

std::unique_ptr<Page> Game::makePage(PageType pt) {
    switch(pt) {
        case PageType::typeMenuPage: return std::make_unique<MenuPage>();
        case PageType::typeMainPage: return std::make_unique<MainPage>();
        default:  return nullptr;
    }
}