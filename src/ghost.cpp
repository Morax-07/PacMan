#include "../include/ghost.h"
#include <cmath>
#include <limits>
#include <raylib.h>
#include <vector>







bool Ghost::staticTexturesLoaded = false;
Texture2D Ghost::frightenedTexture[2] = {};
Texture2D Ghost::eyes = {};
const short Ghost::width = 32;
const short Ghost::height = 32;
Rectangle Ghost::textureRectangle = {32, 0, Ghost::width, Ghost::height};
float Ghost::animationInterval = 0.15f;
float Ghost::animationTimer = 0.0f;
const Vector2* Ghost::playerPosition = nullptr;
bool Ghost::playerPositionSet = false;
Vector2 Ghost::eatenTarget = {14 * 22, 14.5 * 22};
std::vector<Block>* Ghost::blocks = nullptr;
Vector2 Ghost::mapDimension = {0, 0};







//ghost's constructor
//initializes some istance attributes and some static attributes
Ghost::Ghost() {
    this->speed = 120.0f;
    this->movement = Movement::still;
    this->state = GhostState::scatter;
    this->lastCol = -1;
    this->lastRow = -1;


    if(!staticTexturesLoaded) {
        Ghost::frightenedTexture[0] = LoadTexture("../bin/data/texture/scaredGhost1.png");
        Ghost::frightenedTexture[1] = LoadTexture("../bin/data/texture/scaredGhost2.png");
        Ghost::eyes = LoadTexture("../bin/data/texture/eyes.png");
        Ghost::staticTexturesLoaded = true;
    }
}



//ghost's destructor
//unloads the ghost's texture
Ghost::~Ghost() {
    UnloadTexture(ghostTexture);
}



//sets the pointer to the player position
void Ghost::setPlayerPosition(const Vector2* playerPosition) {
    if(!playerPositionSet) {
        Ghost::playerPosition = playerPosition;
        Ghost::playerPositionSet = true;
    }
}



//sets the ghost's texture with its path 
void Ghost::setGhostTexture(const char* path) {
    this->ghostTexture = LoadTexture(path);
}



//draws the ghost
void Ghost::draw() {
    DrawTextureRec(ghostTexture, Ghost::textureRectangle, position, WHITE);
}



//updates the ghost
void Ghost::update(float dt) {
    updateTarget();


    updateMovement(dt);


    updatePosition(dt);


    updateAnimation(dt);
}



//updates the ghost's position
void Ghost::updatePosition(float dt) {
    switch(movement) {
        case Movement::up:
            position.y -= speed * dt;
            break;
        
        case Movement::down:
            position.y += speed * dt;
            break;

        case Movement::left:
            position.x -= speed * dt;
            break;

        case Movement::right:
            position.x += speed * dt;
            break;

        default:
            break;
    }
}



//returns the opposite value of the current movement
Movement Ghost::oppositeMovement() {
    switch(movement) {
        case Movement::up: return Movement::down;
        case Movement::down: return Movement::up;
        case Movement::left: return Movement::right;
        case Movement::right: return Movement::left;
        default: return Movement::still;
    }
}



//updates the ghost's movement w
void Ghost::updateMovement(float dt) {
    //caluculates the ghost's center coordinates
    const Vector2 ghostCenter = {
        .x = position.x + (float)Ghost::width / 2,
        .y = position.y + (float)Ghost::height / 2
    };

    //calculates the current ghost's tile column and row
    const int currentCol = (int)std::floor(ghostCenter.x / 22);
    const int currentRow = (int)std::floor(ghostCenter.y / 22);

    //gets the current tile center coordinates
    const Vector2 currentTileCenter = {
        .x = (currentCol + 0.5f) * 22,
        .y = (currentRow + 0.5f) * 22
    };

    
    //window size in the center of the current tile to determin if the ghost is centered
    const float window = speed * dt;

    //the ghost is centered if its x and y coordinates are inside the window
    bool centered = std::fabs(ghostCenter.x - currentTileCenter.x) <= window && std::fabs(ghostCenter.y - currentTileCenter.y) <= window;
    
    //the ghost is inside a new tile if the current tile's column and row are different from the last ones
    bool newTile = (currentCol != lastCol || currentRow != lastRow);


    //if the ghost is already moving and it is not centered or it is not in a new tile, it doesn't update the movement
    if(movement != Movement::still && !(centered && newTile)) return;



    //snapping to the tile center and lastCol and lastRow update
    position.x = currentTileCenter.x - (float)Ghost::width / 2;
    position.y = currentTileCenter.y - (float)Ghost::height / 2;
    lastCol = currentCol;
    lastRow = currentRow;


    //checks if the position is the same as the position where it is illegal to go up
    bool illegalUp = false;
    if(
        ((position.x == 259 || position.x == 325) && position.y == 237) ||
        ((position.x == 259 || position.x == 325) && position.y == 501)
    ) {
        illegalUp = true;
    }



    //choses which type of update should be performed
    if(
        state == GhostState::scatter || 
        state == GhostState::chase || 
        state == GhostState::eaten
    ) {
        updateWithTarget(currentTileCenter, illegalUp);
    } else {
        //updateWithoutTarget(dt);
    }
}



//updates the movement with a target
void Ghost::updateWithTarget(const Vector2& currentTileCenter, bool illegalUp) {
    //calculates the distance from the coordinates passed to the current target
    static const auto distanceFromTarget = [&](float x, float y) -> float {
        return std::hypotf(x - target.x, y - target.y);
    };

    //all the possible directions
    static const Movement directions[4] = {
        Movement::up,
        Movement::down,
        Movement::left,
        Movement::right
    };



    //initializes the best movement to make and the best distance from the target
    Movement bestMovement = Movement::still;
    float bestDistance = std::numeric_limits<float>::max();

    for(const Movement& possibleDirection : directions) {
        //if the possible direction is equal to the opposit of the current movement it skips (no U-turn)
        if(possibleDirection == oppositeMovement()) continue;

        //if it is illegal to go up it skips
        if(possibleDirection == Movement::up && illegalUp) continue;



        Rectangle neighborHitbox = neighborTileHitbox(currentTileCenter, possibleDirection);
        
        
        //if the current neighbor isn't free it skips
        if(!isFree(neighborHitbox)) continue;


        //calculates the distance from the center of the current neighbor tile hitbox and the target
        float distance = distanceFromTarget(
            neighborHitbox.x + neighborHitbox.width / 2, 
            neighborHitbox.y + neighborHitbox.height / 2
        );


        //if the distance is better than the current one, the current one is updated and the best movement
        //is updated with the current possible direction
        if(distance < bestDistance) {
            bestDistance = distance;
            bestMovement = possibleDirection;
        }
    }



    //if no bestMovement has been found, the best action is to make a U-turn
    if(bestMovement == Movement::still) {
        Movement back = oppositeMovement();
        if(back != Movement::still && isFree(neighborTileHitbox(currentTileCenter, back)))
            bestMovement = back;
    }


    //sets the movement to the best one found
    movement = bestMovement;
}



//unloads all the static textures
void Ghost::unloadStaticTextures() {
    UnloadTexture(Ghost::eyes);
    UnloadTexture(Ghost::frightenedTexture[0]);
    UnloadTexture(Ghost::frightenedTexture[1]);
}



//updates the ghosts animation rectangle
void Ghost::updateAnimation(float dt) {
    Ghost::animationTimer += dt;

    if(Ghost::animationTimer >= Ghost::animationInterval) {
        Ghost::animationTimer -= Ghost::animationInterval;
        Ghost::textureRectangle.x += (Ghost::textureRectangle.x == 0) ? 32 : -32;
    }
}



//gets the neighbor tile hitbox for the current movement option
Rectangle Ghost::neighborTileHitbox(const Vector2& currentTileCenter, Movement movementOption) const {
    const float hitboxDimension = 21.9f;

    /*
    right -> 1
    left -> -1
    up -> -1
    down -> 1
    */
    float directionX = (float)(movementOption == Movement::right) - (float)(movementOption == Movement::left);
    float directionY = (float)(movementOption == Movement::down) - (float)(movementOption == Movement::up);

    //neighbor tile hitbox for current movement (top-left corner for position)
    return Rectangle{
        currentTileCenter.x + directionX * 22 - hitboxDimension / 2, 
        currentTileCenter.y + directionY * 22 - hitboxDimension / 2, 
        hitboxDimension, 
        hitboxDimension
    };
}



//gets the ghost's body hitbox
Rectangle Ghost::bodyHitbox() const {
    return Rectangle{position.x + (float)Ghost::width / 2 - 10.95f, position.y + (float)Ghost::height / 2 - 10.95f, 21.9f, 21.9f};
}



//sets the pointer to the map blocks
void Ghost::setBlocks(std::vector<Block>* blocks) {
    Ghost::blocks = blocks;
}



//sets the map dimension
void Ghost::setMapDimension(Vector2&& mapDimension) {
    Ghost::mapDimension = mapDimension;
}



//returns if the block hitbox collides with the current calculated neighbor tile hitbox
bool Ghost::isFree(const Rectangle& neighbor) {
    for(const auto& block : *blocks) {
        if(CheckCollisionRecs(neighbor, block.hitbox())) return false;
    }
    return true;
}