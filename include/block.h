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


        static auto loadTextures(void) -> void;


    public:
        static auto getWidth(void) noexcept -> short;
        static auto getHeight(void) noexcept -> short;
        static auto unloadTextures(void) -> void;
        auto draw(void) -> void;
        auto hitbox(void) const -> Rectangle;



        Block(Vector2, short int, float, bool);
        Block(void);
};