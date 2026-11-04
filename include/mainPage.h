#include "page.h"
#include "map.h"
#include "player.h"
#include "ghost.h"
#include "redGhost.h"








class MainPage final: public Page {
    private:
        Map map;
        Player pacman;
        RedGhost g;
        Rectangle coverRec;
        bool paused;
        int level;


    public:
        void update(float) override;
        void draw(void) override;
        void input(void) override;


        MainPage(void);
        ~MainPage() override;
};