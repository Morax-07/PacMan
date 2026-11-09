#pragma once
#include "ghost.h"








class OrangeGhost final: public Ghost {
    private:
        auto updateChaseTarget(void) -> void override;


    public:
        auto setScatterTarget(void) -> void override;
        auto drawTarget(void) -> void override;


        OrangeGhost();
        ~OrangeGhost() override;
};