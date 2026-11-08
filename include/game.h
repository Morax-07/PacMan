#pragma once
#include <memory>
#include "global.h"
#include "page.h"








class Game final {
    private:
        std::unique_ptr<Page> page;
        PageType currentPageType;

        auto makePage(PageType) -> std::unique_ptr<Page>;
        auto draw(void) -> void;
        auto update(void) -> void;
        auto input(void) -> void;


    public:
        void run(void);
        
        Game(void);
        ~Game();
};