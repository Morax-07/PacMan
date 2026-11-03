#pragma once
#include "raylib.h"
#include "array"









class Block final {
    private:
        static std::array<Texture2D, 10> blockTextures;
        const short int textureIndex;
        const Vector2 position;
        static const short int width;
        static const short int height;
        float rotation;
        bool flipFlag;
        static bool loadedTexturesFlag;


        static void loadTextures(void);


    public:
        static short int getWidth(void) noexcept;
        static short int getHeight(void) noexcept;
        static void unloadTextures(void);
        void draw(void);
        Rectangle hitbox(void) const;



        Block(Vector2, short int, float, bool);
        Block(void);
};