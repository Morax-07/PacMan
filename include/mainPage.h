#include "global.h"
#include "page.h"
#include "map.h"
#include "player.h"
#include "ghost.h"
#include "redGhost.h"
#include "pinkGhost.h"
#include "blueGhost.h"
#include "orangeGhost.h"
#include <array>
#include <memory>









class MainPage final: public Page {
    private:
        Map map;
        Player pacman;
        std::array<std::unique_ptr<Ghost>, 4> ghosts;
        Rectangle coverRec;
        bool paused;
        int level;
        float stopForEatenGhostInterval;
        float stopForEatenGhostTimer;


        auto collisions(void) -> void;
        auto checkPacmanGhostCollision() -> void;
        auto drawOutlineText(const char*, const Vector2&, float, float, Color, Color = Global::pacBlue) -> void;


    public:
        auto update(float) -> void override;
        auto draw(void) -> void override;
        auto input(void) -> void override;


        MainPage(void);
        ~MainPage() override;
};