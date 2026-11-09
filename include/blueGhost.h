#pragma once
#include "ghost.h"
#include <raylib.h>








class BlueGhost final: public Ghost {
    private:
        const Vector2* redGhostPosition;
        Vector2 playerDirection;

        auto updateChaseTarget(void) -> void override;


    public:
        auto setScatterTarget(void) -> void override;
        auto drawTarget(void) -> void override;
        auto setRedGhostPosition(const Vector2*) -> void;


        BlueGhost();
        ~BlueGhost() override;
};