#pragma once
#include <memory>
#include "global.h"
#include "page.h"








class Game final {
    private:
        std::unique_ptr<Page> page;
        PageType currentPageType;

        std::unique_ptr<Page> makePage(PageType);
        void run(void);


    public:
        void draw(void);
        void update(void);
        void input(void);
        
        Game(void);
        ~Game();
};