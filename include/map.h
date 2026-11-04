#pragma once
#include "player.h"
#include "raylib.h"
#include "pellet.h"
#include "block.h"
#include <vector>







class Map final {
    private:
        RenderTexture2D mapTexture;
        const Vector2 position;
        std::vector<Block> blocks;
        std::vector<Pellet> pellets;
        
        

        void loadMapComponents(std::vector<std::vector<short int>>&);
        void loadMapTexture(void);
        



    public:
        const int width;
        const int height;


        void draw(void);
        void update(float);
        std::vector<Block>* getBlocks(void);
        std::vector<Pellet>* getPellets(void);


        Map();
        ~Map();
};