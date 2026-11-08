#pragma once
#include "ghost.h"








class BlueGhost final: public Ghost {
    private:
        auto updateChaseTarget(void) -> void override;


    public:
        auto setScatterTarget(void) -> void override;


        BlueGhost();
        ~BlueGhost() override;
};