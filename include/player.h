#pragma once
#include "raylib.h"
#include "global.h"
#include <string>
#include <unordered_map>
#include <vector>
#include "pellet.h"
#include "block.h"
#include <functional>
#include "ghost.h"











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
        Movement currentDirection;
        float animationInterval;
        float animationTimer;
        float rotation;
        std::vector<Pellet>* pellets;
        const std::vector<Block>* blocks;
        int points;
        bool stoppedForCollision;
        float positionBounds[2];
        int eatenPellets;
        float notEatingInterval;
        float notEatingTimer;
        bool notEating;


        auto mouthHitbox(void) const -> Rectangle;
        auto bodyHitbox(void) const -> Rectangle;
        auto upHitbox(void) const -> Rectangle;
        auto downHitbox(void) const -> Rectangle;
        auto leftHitbox(void) const -> Rectangle;
        auto rightHitbox(void) const -> Rectangle;
        auto checkAndSetNextMovement(std::function<Rectangle(void)>, float) -> void;
        auto updateMovement(float) -> void;
        auto updatePosition(float) -> void;
        auto updateAnimation(float) -> void;
        auto isOutOfBounds(void) -> bool;
        auto updateNotEating(float) -> void;



    public:
        auto drawLife(Vector2) -> void;
        auto draw(void) -> void;
        auto input(void) -> void;
        auto update(float) -> void;
        auto setPellets(std::vector<Pellet>*) -> void;
        auto setBlocks(const std::vector<Block>*) -> void;
        auto getPoints(void) const noexcept -> int;
        auto checkPelletsCollision(void) -> void;
        auto checkBodyBlocksCollision(void) -> void;
        auto checkGhostCollision(const Ghost*) -> void;
        auto setRightPositionBound(float) -> void;
        auto getPosition(void) const noexcept -> const Vector2*;
        auto getMovement(void) const noexcept -> const Movement*;
        auto getEatenPellets(void) const noexcept -> const int*;
        auto isNotEating(void) const noexcept -> bool;
        auto resetNotEating(void) -> void;


        const short int width;
        const short int height;


        Player(void);
        ~Player();
};