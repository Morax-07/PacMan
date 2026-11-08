#pragma once
#include "ghost.h"








class RedGhost final: public Ghost {
    private:
        auto updateChaseTarget(void) -> void override;


    public:
        auto setScatterTarget(void) -> void override;


        RedGhost();
        ~RedGhost() override;
};