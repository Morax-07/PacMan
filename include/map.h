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
        
        

        auto loadMapComponents(std::vector<std::vector<short int>>&) -> void;
        auto loadMapTexture(void) -> void;
        



    public:
        const int width;
        const int height;


        auto draw(void) -> void;
        auto update(float) -> void;
        auto getBlocks(void) const -> const std::vector<Block>*;
        auto getPellets(void) -> std::vector<Pellet>*;


        Map();
        ~Map();
};