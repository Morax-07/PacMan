#pragma once
#include "block.h"
#include "global.h"
#include "player.h"
#include <raylib.h>








class Ghost {
    protected:
        static const Vector2* playerPosition;
        static bool playerPositionSet;
        static Rectangle textureRectangle;
        static bool staticTexturesLoaded;
        static Texture2D frightenedTexture[2];
        static Texture2D eyes;
        static Vector2 eatenTarget;
        static float animationInterval;
        static float animationTimer;
        static std::vector<Block>* blocks;
        static Vector2 mapDimension;
        Vector2 position;
        Vector2 target;
        Vector2 scatterTarget;
        Texture2D ghostTexture;
        Movement movement;
        GhostState state;
        Rectangle eyeTextureRectangle;
        float speed;
        float lastCol;
        float lastRow;
        
        

        static void updateAnimation(float);
        void updatePosition(float);
        void updateMovement(float);
        Rectangle neighborTileHitbox(const Vector2&, Movement) const;
        bool isFree(const Rectangle&);
        void updateWithoutTarget(float);
        void updateWithTarget(const Vector2&, bool);
        Movement oppositeMovement(void);
        virtual void updateTarget(void) = 0;

        


    public:
        static const short width;
        static const short height;

        static void unloadStaticTextures(void);
        static void setBlocks(std::vector<Block>*);
        static void setMapDimension(Vector2&&);
        static void setPlayerPosition(const Vector2*);
        void update(float);
        void draw(void);
        Rectangle bodyHitbox(void) const;
        void setGhostTexture(const char*);


        Ghost();
        virtual ~Ghost();
};