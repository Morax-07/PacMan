#pragma once
#include "ghost.h"








class RedGhost final: public Ghost {
    private:
        auto updateChaseTarget(void) -> void override;


    public:
        auto setScatterTarget(void) -> void override;
        auto getPosition(void) -> const Vector2*;
        auto drawTarget(void) -> void override;


        RedGhost();
        ~RedGhost() override;
};