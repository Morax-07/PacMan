#pragma once
#include "raylib.h"
#include "global.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "pellet.h"
#include "block.h"
#include <functional>









class Player final {
    private:
        short int life;
        short int speed;
        Texture2D texture;
        Vector2 position;
        Rectangle textureRectangle;
        int textureRecXDirection;
        Movement movement;
        Movement nextMovement;
        float animationInterval;
        float animationTimer;
        float rotation;
        std::vector<Pellet>* pellets;
        std::vector<Block>* blocks;
        int points;
        bool stoppedForCollision;


        Rectangle mouthHitbox(void) const;
        Rectangle bodyHitbox(void) const;
        Rectangle upHitbox(void) const;
        Rectangle downHitbox(void) const;
        Rectangle leftHitbox(void) const;
        Rectangle rightHitbox(void) const;
        void checkAndSetNextMovement(std::function<Rectangle(void)>, float);
        void updateMovement(float);
        void updatePosition(float);
        void updateAnimation(float);


    public:
        void drawLife(Vector2);
        void draw(void);
        void input(void);
        void update(float);
        void setPellets(std::vector<Pellet>*);
        void setBlocks(std::vector<Block>*);
        int getPoints(void) const noexcept;
        void checkPelletsCollision(void);
        void checkBodyBlocksCollision(void);


        const short int width;
        const short int height;


        Player(void);
        ~Player();
};