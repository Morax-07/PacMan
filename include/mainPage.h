#include "page.h"
#include "map.h"
#include "player.h"








class MainPage final: public Page {
    private:
        Map map;
        Player pacman;


    public:
        void update(float) override;
        void draw(void) override;
        void input(void) override;


        MainPage(void);
        ~MainPage() override;
};