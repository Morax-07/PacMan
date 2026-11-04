#pragma once
#include "ghost.h"








class RedGhost final: public Ghost {
    protected:
        void updateTarget(void) override;


    public:
        void setScatterTarget(void);

        RedGhost();
        ~RedGhost() override;
};