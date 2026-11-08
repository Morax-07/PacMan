#include "page.h"
#include "map.h"
#include "player.h"
#include "ghost.h"
#include "redGhost.h"
#include "pinkGhost.h"
#include "blueGhost.h"
#include "orangeGhost.h"








class MainPage final: public Page {
    private:
        Map map;
        Player pacman;
        RedGhost rg;
        PinkGhost pg;
        BlueGhost bg;
        OrangeGhost og;
        Rectangle coverRec;
        bool paused;
        int level;


    public:
        auto update(float) -> void override;
        auto draw(void) -> void override;
        auto input(void) -> void override;


        MainPage(void);
        ~MainPage() override;
};