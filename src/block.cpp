#include "./../include/block.h"
#include <raylib.h>
#include <stdexcept>








//static attributes initialization
std::array<Texture2D, 10> Block::blockTextures = {};
const short int Block::width = 22;
const short int Block::height = 22;
bool Block::loadedTexturesFlag = false;





//block's parametrized constructor
Block::Block(Vector2 position, short int textureIndex, float rotation, bool flipFlag = false): textureIndex(textureIndex), position(position) {
    if(!loadedTexturesFlag) loadTextures();
    

    this->rotation = rotation;
    this->flipFlag = flipFlag;
    Block::loadedTexturesFlag = true;
}



//block's default constructor
Block::Block(): textureIndex(0), position(Vector2 {0, 0}) {
    if(blockTextures.empty()) loadTextures(); 

    this->rotation = 0;
    this->flipFlag = false;
}



//loads all the textures of the blocks
auto Block::loadTextures() -> void {
    for(std::size_t i = 0; i < blockTextures.size(); i++) {
        switch(i) {
            case 0:
                blockTextures[i] = LoadTexture("../bin/data/texture/externWall.png");
                break;

            case 1:
                blockTextures[i] = LoadTexture("../bin/data/texture/bigTurn.png");
                break;

            case 2:
                blockTextures[i] = LoadTexture("../bin/data/texture/ghostSpownEdge.png");
                break;

            case 3:
                blockTextures[i] = LoadTexture("../bin/data/texture/ghostSpownTurn.png");
                break;

            case 4:
                blockTextures[i] = LoadTexture("../bin/data/texture/ghostSpownDoor.png");
                break;

            case 5:
                blockTextures[i] = LoadTexture("../bin/data/texture/smallTurn.png");
                break;

            case 6:
                blockTextures[i] = LoadTexture("../bin/data/texture/mediumTurn.png");
                break;

            case 7:
                blockTextures[i] = LoadTexture("../bin/data/texture/externTurn1.png");
                break;

            case 8:
                blockTextures[i] = LoadTexture("../bin/data/texture/externTurn2.png");
                break;

            case 9:
                blockTextures[i] = LoadTexture("../bin/data/texture/innerObstacle.png");
                break;

            default:
                throw std::runtime_error("It seems impossible to open the file at index \"" + std::to_string(i) + "\"");
                break;
        }
    }
}

//unloads all the textures of the blocksh
auto Block::unloadTextures() -> void {
    for(auto& texture : blockTextures) UnloadTexture(texture);
}

//gets the block's width
auto Block::getWidth() noexcept -> short {
    return Block::width;
}

//gets the block's height
auto Block::getHeight() noexcept -> short {
    return Block::height;
}

//draws the block
auto Block::draw() -> void {
    Rectangle source = (!flipFlag) ? Rectangle {
        0, 
        0, 
        Block::width, 
        Block::height
    } : Rectangle {
        22, 
        22, 
        -Block::width, 
        Block::height
    };

    DrawTexturePro(blockTextures[textureIndex], source, Rectangle {position.x, position.y, width, height}, Vector2 {0 ,0}, rotation, WHITE);
}



//returns the block's hitbox translated based on it's rotation
auto Block::hitbox() const -> Rectangle {
    float x, y;

    switch((int)rotation) {
        case 0:
            x = position.x;
            y = position.y;
            break;
        
        case 90:
            x = position.x - Block::width;
            y = position.y;
            break;

        case 180:
            x = position.x - Block::width;
            y = position.y - Block::height;
            break;

        case 270:
            x = position.x;
            y = position.y - Block::height;
            break;

        default:
            x = position.x;
            y = position.y;
            break;
    }

    return Rectangle{x, y, Block::height, Block::width};
}