#pragma  once
#include "ghost.h"







class PinkGhost final: public Ghost {
    private:
        auto updateChaseTarget(void) -> void override;



    public:
        auto setScatterTarget(void) -> void override;

        
        PinkGhost(void);
        ~PinkGhost() override;
};