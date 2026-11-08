#include "raylib.h"
#include "./../include/game.h"
#include "./../include/menuPage.h"
#include "./../include/mainPage.h"
#include "./../include/global.h"
#include <memory>












//game's constrctor
//initializes window and page
Game::Game() {
    //initialize the window
    InitWindow(WIDTH, HEIGHT, "Pac-Man");
    HideCursor();
    SetExitKey(KEY_NULL);

    this->currentPageType = PageType::typeMenuPage;
    this->page = makePage(currentPageType);
}



//game's destructor
//unloads page and font and closes window
Game::~Game() {
    page.reset();
    UnloadFont(Page::font);
    CloseWindow();
}



//game loop
auto Game::run() -> void{
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



//draws the page
auto Game::draw() -> void {
    page->draw();
}



//updates the page and creates delta time dt
auto Game::update() -> void {
    float dt = std::min(GetFrameTime(), 0.05f);

    // is game is in the menu and it says that the page is changing, the game make a new page (main)
    if(currentPageType == PageType::typeMenuPage && dynamic_cast<MenuPage*>(page.get())->isPageChanging()) {
        page = makePage(PageType::typeMainPage);
        currentPageType = PageType::typeMainPage;
    }

    page->update(dt);
}



//handles page's inputs
auto Game::input() -> void {
    page->input();
}



//returns a new unique_ptr based on the page type
auto Game::makePage(PageType pt) -> std::unique_ptr<Page> {
    switch(pt) {
        case PageType::typeMenuPage: return std::make_unique<MenuPage>();
        case PageType::typeMainPage: return std::make_unique<MainPage>();
        default:  return nullptr;
    }
}