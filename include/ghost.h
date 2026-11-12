#pragma once
#include "block.h"
#include "global.h"
#include <raylib.h>
#include <vector>








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
        static const int* eatenPellets;
        static GhostState globalState;
        static GhostState prevState;
        static bool isSetPrevState;
        mutable float speed;
        Rectangle eyeTextureRectangle;
        float lastCol;
        float lastRow;
        Texture2D ghostTexture;
        bool reverse;
        bool respowning;
        bool gettingInCageForRespown;
        bool gettingOutCageForRespown;
        bool spowning;
        bool isSetCagedFirstMovement;
        bool forcedSpown;


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
        auto pacmanEffect(void) -> void;
        auto updateToSetRespowningPosition(float) -> void;
        auto updatePositionWhileRespowning(float, float) -> void;
        auto updateSpowning(void) -> void;
        auto updateCagedPosition(float) -> void;
        auto updateCagedPositionSpowning(float) -> void;
        auto updateCagedPositionNotSpowning(float) -> void;





    protected:
        static const Vector2* playerPosition;
        static const Movement* playerMovement;
        static Vector2 mapDimension;
        mutable GhostState state;
        mutable Vector2 target;
        Vector2 position;
        Vector2 scatterTarget;
        Movement movement;
        int minPelletsForSpown;
        
        
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
        static auto setEatenPellets(const int*) -> void;
        auto update(float) -> void;
        auto draw(void) -> void;
        auto bodyHitbox(void) const -> Rectangle;
        auto setGhostTexture(const char*) -> void;
        auto getState(void) const -> GhostState;
        auto setStateToEaten(void) const -> void;
        auto getPosition(void) const noexcept -> Vector2;
        auto spowned(void) -> bool;
        auto forceSpown(void) -> void;


        virtual auto setScatterTarget(void) -> void = 0;
        virtual auto drawTarget(void) -> void = 0;



        Ghost();
        virtual ~Ghost() = 0;
};