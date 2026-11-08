#pragma once
#include "block.h"
#include "global.h"
#include "player.h"
#include <raylib.h>








class Ghost {
    private:
        static bool playerPositionSet;
        static Rectangle textureRectangle;
        static bool staticTexturesLoaded;
        static Texture2D frightenedTexture[2];
        static Texture2D eyes;
        static Vector2 eatenTarget;
        static float animationInterval;
        static float animationTimer;
        static float changeFrightenedTextureInterval;
        static float changeFrightenedTextureTimer;
        static float frightenedModeInterval;
        static float frightenedModeTimer;
        static float startFrightenedTextureBlinkingTime;
        static bool startFrightenedTextureBlinking;
        static short frightenedTextureBlinkingIndex;
        static bool frightenedActive;
        static const std::vector<Block>* blocks;
        Rectangle eyeTextureRectangle;
        float speed;
        float lastCol;
        float lastRow;
        Texture2D ghostTexture;
        GhostState prevState;


        static auto updateAnimationRectangle(float) -> void;
        auto updatePosition(float) -> void;
        auto updateMovement(float) -> void;
        auto neighborTileHitbox(const Vector2&, Movement) const -> Rectangle;
        auto isFree(const Rectangle&) -> bool;
        auto updateWithoutTarget(const Vector2&, bool) -> void;
        auto updateWithTarget(const Vector2&, bool) -> void;
        auto oppositeMovement(void) -> Movement;
        auto isOutOfBounds(void) -> bool;
        auto switchToFrightenedState(void) -> void;
        auto switchFromFrightenedState(void) -> void;
        auto updateEyeTextureRectangle(void) -> void;
        auto updateTarget(void) -> void;
        auto distanceFromTarget(float, float) -> float;





    protected:
        static const Vector2* playerPosition;
        static const Movement* playerMovement;
        static Vector2 mapDimension;
        Vector2 position;
        Vector2 target;
        Vector2 scatterTarget;
        GhostState state;
        Movement movement;
        
        
        virtual auto updateChaseTarget(void) -> void = 0;

        


    public:
        static const short width;
        static const short height;

        static auto unloadStaticTextures(void) -> void;
        static auto setBlocks(const std::vector<Block>*) -> void;
        static auto setMapDimension(Vector2&&) -> void;
        static auto setPlayerPosition(const Vector2*) -> void;
        static auto setPlayerMovement(const Movement*) -> void;
        static auto updateFrightenedMode(float) -> void;
        auto update(float) -> void;
        auto draw(void) -> void;
        auto bodyHitbox(void) const -> Rectangle;
        auto setGhostTexture(const char*) -> void;

        virtual auto setScatterTarget(void) -> void = 0;



        Ghost();
        virtual ~Ghost() = 0;
};