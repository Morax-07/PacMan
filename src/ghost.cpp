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
Rectangle Ghost::textureRectangle = {0, 0, Ghost::width, Ghost::height};
float Ghost::animationInterval = 0.3f;
float Ghost::animationTimer = 0.0f;
const Vector2* Ghost::playerPosition = nullptr;
bool Ghost::playerPositionSet = false;
Vector2 Ghost::eatenTarget = {14 * 22, 14.5 * 22};
const std::vector<Block>* Ghost::blocks = nullptr;
Vector2 Ghost::mapDimension = {0, 0};
float Ghost::changeFrightenedTextureInterval = 0.2f;
float Ghost::changeFrightenedTextureTimer = 0.0f;
float Ghost::frightenedModeInterval = 6.0f;
float Ghost::frightenedModeTimer = 0.0f;
float Ghost::startFrightenedTextureBlinkingTime = 4.0f;
bool Ghost::startFrightenedTextureBlinking = false;
short Ghost::frightenedTextureBlinkingIndex = 0;
bool Ghost::frightenedActive = false;
const Movement* Ghost::playerMovement = nullptr;
const int* Ghost::eatenPellets = nullptr;
GhostState Ghost::globalState = GhostState::scatter;
GhostState Ghost::prevState = GhostState::scatter;
bool Ghost::isSetPrevState = false;







//ghost's constructor
//initializes some istance attributes and some static attributes
Ghost::Ghost() {
    this->speed = 120.0f;
    this->movement = Movement::still;
    this->lastCol = -1;
    this->lastRow = -1;
    this->eyeTextureRectangle = Rectangle{0, 0, 32, 11};
    this->reverse = false;
    this->respowning = false;
    this->gettingInCageForRespown = false;
    this->gettingOutCageForRespown = false;
    this->spowning = false;
    this->minPelletsForSpown = 0;
    this->isSetCagedFirstMovement = false;
    this->forcedSpown = false;



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
auto Ghost::setPlayerPosition(const Vector2* playerPosition) -> void {
    if(!playerPositionSet) {
        Ghost::playerPosition = playerPosition;
        Ghost::playerPositionSet = true;
    }
}



//sets the ghost's texture with its path 
auto Ghost::setGhostTexture(const char* path) -> void {
    this->ghostTexture = LoadTexture(path);
}



//draws the ghost
auto Ghost::draw() -> void {
    if(state == GhostState::frightened || (state == GhostState::caged && Ghost::frightenedActive)) {
        DrawTextureRec(
            Ghost::frightenedTexture[Ghost::frightenedTextureBlinkingIndex],
            textureRectangle, 
            position, 
            WHITE
        );
    } else {
        if(state != GhostState::eaten) {
            DrawTextureRec(ghostTexture, Ghost::textureRectangle, position, WHITE);
        }
        
        DrawTextureRec(
            Ghost::eyes, 
            eyeTextureRectangle, 
            Vector2{position.x, position.y + 6}, 
            WHITE
        );
    }
}



//updates the ghost
auto Ghost::update(float dt) -> void {
    //switching to frightened state
    if(Global::bigPelletEaten && state != GhostState::eaten && state != GhostState::caged) {
        switchToFrightenedState();
    }


    //switch back from frightened state
    if(state == GhostState::frightened && !Ghost::frightenedActive) {
        switchFromFrightenedState();
    }


    //updates the target only in scatter and chase
    if(state == GhostState::scatter || state == GhostState::chase) {
        updateTarget();
    }


    //updates to align the ghost with the respown position
    if((state == GhostState::eaten && !respowning) || gettingOutCageForRespown) {
        updateToSetRespowningPosition(dt);
    }


    //updates ghost texture rectangle
    updateAnimationRectangle(dt);



    //updates the position while the ghost is respowning
    if(respowning) {
        if(gettingInCageForRespown) {
            updatePositionWhileRespowning(eatenTarget.y - (float)Ghost::height / 2, dt);
        } else if(gettingOutCageForRespown) {
            updatePositionWhileRespowning(237.0f, dt);
        }

        return;
    }


    //updates the eye texture rectangle
    updateEyeTextureRectangle();


    //updates the position while in caged
    if(state == GhostState::caged) {
        updateSpowning();
        updateCagedPosition(dt);
        return;
    }


    //updates the ghost movement
    updateMovement(dt);


    //updates the position while it's in scatter and chase
    updatePosition(dt);
}



//updates the timer for the frightened mode and the frightened ghost texture blinking
auto Ghost::updateFrightenedMode(float dt) -> void {
    if(!frightenedActive) return;



    Ghost::frightenedModeTimer += dt;

    if(Ghost::frightenedModeTimer >= Ghost::startFrightenedTextureBlinkingTime) 
        Ghost::startFrightenedTextureBlinking = true;

    if(Ghost::frightenedModeTimer >= Ghost::frightenedModeInterval) {
        Ghost::frightenedActive = false;
        Ghost::frightenedModeTimer = 0.0f;
        Ghost::startFrightenedTextureBlinking = false;
        Ghost::frightenedTextureBlinkingIndex = 0;
    }

    if(Ghost::startFrightenedTextureBlinking) {
        Ghost::changeFrightenedTextureTimer += dt;
        if(Ghost::changeFrightenedTextureTimer >= Ghost::changeFrightenedTextureInterval) {
            Ghost::changeFrightenedTextureTimer -= Ghost::changeFrightenedTextureInterval;
            Ghost::frightenedTextureBlinkingIndex = 1 - Ghost::frightenedTextureBlinkingIndex;
        }
    }
}



//updates the ghost's position
auto Ghost::updatePosition(float dt) -> void {
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


    
    //handles pacman-effect
    pacmanEffect();
}



//handles pacman-effect
auto Ghost::pacmanEffect() -> void {
    if(position.x <= -64 && movement == Movement::left) 
        position.x = mapDimension.x + 30;
    else if(position.x >= mapDimension.x + 30 && movement == Movement::right)
        position.x = -64;
}



//updates the position while respowning
auto Ghost::updatePositionWhileRespowning(float yTarget, float dt) -> void {
    const float window = 100.0f * dt;
    const float direction = Global::sign(yTarget - position.y);

    if(std::fabs(position.y - yTarget) <= window && direction > 0) {
        position.y = yTarget;
        gettingInCageForRespown = false;
        gettingOutCageForRespown = true;
        state = Ghost::prevState;
        return;
    } else if(std::fabs(position.y - yTarget) <= window && direction < 0) {
        position.y = yTarget;
        gettingOutCageForRespown = false;
        respowning = false;
        speed = 120.0f;
        return;
    }

    position.y += window * direction;
}



//returns the opposite value of the current movement
auto Ghost::oppositeMovement() -> Movement {
    switch(movement) {
        case Movement::up: return Movement::down;
        case Movement::down: return Movement::up;
        case Movement::left: return Movement::right;
        case Movement::right: return Movement::left;
        default: return Movement::still;
    }
}



//updates the ghost's movement
auto Ghost::updateMovement(float dt) -> void {
    if(isOutOfBounds()) return;



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


    //reverses the movement for switching state
    if(reverse) {
        reverse = false;

        Movement back = oppositeMovement();

        if(
            back != Movement::still && 
            !(back == Movement::up && illegalUp) && 
            isFree(neighborTileHitbox(currentTileCenter, back))
        ) {
            movement = back;
            return;
        }
    }




    //choses which type of update should be performed
    if(state == GhostState::frightened) 
        updateWithoutTarget(currentTileCenter, illegalUp);
    else 
        updateWithTarget(currentTileCenter, illegalUp);
}



//updates the movement in frightened state
auto Ghost::updateWithoutTarget(const Vector2& currentTileCenter, bool illegalUp) -> void {
    //all the possible directions
    static constexpr Movement directions[4] = {
        Movement::up,
        Movement::down,
        Movement::left,
        Movement::right
    };


    std::vector<Movement> possibleDiections;
    possibleDiections.reserve(4);



    for(const Movement& movementOption : directions) {
        //if the possible direction is equal to the opposit of the current movement it skips (no U-turn)
        if(movementOption == oppositeMovement()) continue;

        //if it is illegal to go up it skips
        if(movementOption == Movement::up && illegalUp) continue;
        

        Rectangle neighborHitbox = neighborTileHitbox(currentTileCenter, movementOption);
        
        
        //if the current neighbor isn't free it skips
        if(!isFree(neighborHitbox)) continue;


        //adds the movement option to the possible directions
        possibleDiections.push_back(movementOption);
    }



    //if no possible direction is found the only possible one is back
    if(possibleDiections.empty()) {
        Movement back = oppositeMovement();
        if(back != Movement::still && isFree(neighborTileHitbox(currentTileCenter, back)))
            possibleDiections.push_back(back);
    }


    //if there are still no possible directions the ghost is still
    if(possibleDiections.empty()) {
        movement = Movement::still;
        return;
    }


    //sets the current movement to a random possible direction
    short index = GetRandomValue(0, possibleDiections.size() - 1);
    movement = possibleDiections.at(index);
}



//updates the movement with a target
auto Ghost::updateWithTarget(const Vector2& currentTileCenter, bool illegalUp) -> void {
    //all the possible directions
    static constexpr Movement directions[4] = {
        Movement::up,
        Movement::down,
        Movement::left,
        Movement::right
    };



    //initializes the best movement to make and the best distance from the target
    Movement bestMovement = Movement::still;
    float bestDistance = std::numeric_limits<float>::max();

    for(const Movement& movementOption : directions) {
        //if the possible direction is equal to the opposit of the current movement it skips (no U-turn)
        if(movementOption == oppositeMovement()) continue;

        //if it is illegal to go up it skips
        if(movementOption == Movement::up && illegalUp) continue;



        Rectangle neighborHitbox = neighborTileHitbox(currentTileCenter, movementOption);
        
        
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
            bestMovement = movementOption;
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
auto Ghost::unloadStaticTextures() -> void {
    UnloadTexture(Ghost::eyes);
    UnloadTexture(Ghost::frightenedTexture[0]);
    UnloadTexture(Ghost::frightenedTexture[1]);
}



//updates the ghosts animation rectangle
auto Ghost::updateAnimationRectangle(float dt) -> void {
    Ghost::animationTimer += dt;

    if(Ghost::animationTimer >= Ghost::animationInterval) {
        Ghost::animationTimer -= Ghost::animationInterval;
        Ghost::textureRectangle.x += (Ghost::textureRectangle.x == 0) ? 32 : -32;
    }
}



//gets the neighbor tile hitbox for the current movement option
auto Ghost::neighborTileHitbox(const Vector2& currentTileCenter, Movement movementOption) const -> Rectangle{
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
auto Ghost::bodyHitbox() const -> Rectangle {
    return Rectangle{position.x + (float)Ghost::width / 2 - 10.95f, position.y + (float)Ghost::height / 2 - 10.95f, 21.9f, 21.9f};
}



//sets the pointer to the map blocks
auto Ghost::setBlocks(const std::vector<Block>* blocks) -> void {
    Ghost::blocks = blocks;
}



//sets the map dimension
auto Ghost::setMapDimension(Vector2&& mapDimension) -> void {
    Ghost::mapDimension = mapDimension;
}



//returns if the block hitbox collides with the current calculated neighbor tile hitbox
auto Ghost::isFree(const Rectangle& neighbor) -> bool {
    for(const auto& block : *blocks) {
        if(CheckCollisionRecs(neighbor, block.hitbox())) return false;
    }
    return true;
}



//returns if the ghost is out of bounds
auto Ghost::isOutOfBounds() -> bool {
    return (position.x <= 0 || position.x >= mapDimension.x - 22);
}



//sets up the frightened state
auto Ghost::switchToFrightenedState() -> void {
    //eaten already excluded
    if(state != GhostState::frightened && state != GhostState::caged)
        reverse = true;

        
    //sets the previous state
    if(!Ghost::isSetPrevState) {
        Ghost::prevState = Ghost::globalState;
        Ghost::isSetPrevState = true;
    }

    speed = 60.0f;
    Ghost::frightenedModeTimer = 0.0f;
    Ghost::changeFrightenedTextureTimer = 0.0f;
    Ghost::frightenedTextureBlinkingIndex = 0;
    Ghost::startFrightenedTextureBlinking = false;
    Ghost::frightenedActive = true;
    Ghost::globalState = GhostState::frightened;
    state = GhostState::frightened;
}



//goes back from the frightened state to the previous state
auto Ghost::switchFromFrightenedState() -> void {
    Ghost::globalState = Ghost::prevState;
    Ghost::isSetPrevState = false;
    state = Ghost::globalState;
    reverse = true;
    speed = 120.0f;
}



//updates the eye orientation based on the movement
auto Ghost::updateEyeTextureRectangle() -> void {
    switch(movement) {
        case Movement::up:
            eyeTextureRectangle = Rectangle{0, 0, 32, 11};
            break;

        case Movement::down:
            eyeTextureRectangle = Rectangle{0, 0, 32, -11};
            break;

        case Movement::left:
            eyeTextureRectangle = Rectangle{32, 0, 32, 11};
            break;

        case Movement::right:
            eyeTextureRectangle = Rectangle{32, 11, -32, 11};
            break;

        default:
            break;
    }
}



//updates the ghost target
auto Ghost::updateTarget() -> void {
    switch(state) {
        case GhostState::scatter:
            target = scatterTarget;
            break;

        case GhostState::chase:
            updateChaseTarget();
            break;

        default:
            break;
    }
}



//sets the pointer to the player movement
auto Ghost::setPlayerMovement(const Movement* playerMovement) -> void {
    Ghost::playerMovement = playerMovement;
}



//calculates the distance from the coordinates passed to the current target
auto Ghost::distanceFromTarget(float x, float y) -> float {
    return std::hypotf(x - target.x, y - target.y);
}



//returns the current ghost state
auto Ghost::getState() const -> GhostState {
    return this->state;
}



//sets the state to eaten
auto Ghost::setStateToEaten() const -> void {
    state = GhostState::eaten;
    speed = 240.0f;
    target = Ghost::eatenTarget;
}



//returns the ghost position
auto Ghost::getPosition() const noexcept -> Vector2 {
    return this->position;
}



//checks if the ghost is aligned whit the respowning position and sets the x position
//to be aligned 
auto Ghost::updateToSetRespowningPosition(float dt) -> void {
    const float window = speed * dt;

    float ghostXCenter = position.x + (float)Ghost::width / 2;
    bool alignedWithCage = std::fabs(ghostXCenter - 14 * 22) <= window && position.y == 237;

    if(!alignedWithCage) return;

    position.x = 14 * 22 - (float)Ghost::width / 2;
    respowning = true;
    gettingInCageForRespown = true;
}



//sets the pointer to the number of eaten pellets by the player
auto Ghost::setEatenPellets(const int* eatenPellets) -> void {
    Ghost::eatenPellets = eatenPellets;
}



//checks if the ghost is spowning
auto Ghost::updateSpowning() -> void {
    spowning = *eatenPellets >= minPelletsForSpown || forcedSpown;
}



//updates the position while the ghost is caged and respowning
auto Ghost::updateCagedPosition(float dt) -> void {
    if(spowning) {
        updateCagedPositionSpowning(dt);
    } else {
        updateCagedPositionNotSpowning(dt);
    }
}



//updates the position to make the gost spown and get out of the cage
auto Ghost::updateCagedPositionSpowning(float dt) -> void {
    const float window = speed * dt;
    const float ghostCenterX = position.x + (float)Ghost::width / 2;
    const float ghostCenterY = position.y + (float)Ghost::height / 2;

    const bool xAligned = std::fabs(14 * 22 - ghostCenterX) <= window;
    const bool yAligned = std::fabs(11.5f * 22 - ghostCenterY) <= window;

    if(xAligned) {
        position.x = 14 * 22 - (float)Ghost::width / 2;
    } else {
        const float direction = Global::sign(14 * 22 - ghostCenterX);
        position.x += window * direction;
        movement = direction > 0 ? Movement::right : Movement::left;
    }

    if(xAligned && !yAligned) {
        position.y -= window;
        movement = Movement::up;
    } else if(xAligned && yAligned) {
        position.y = 237;
        spowning = false;
        
        if(Ghost::globalState == GhostState::frightened) speed = 60.0f;

        state = Ghost::globalState;
        movement = Movement::still;
    }
}



//updates the position while the ghost is still caged and not spowning (up and down)
auto Ghost::updateCagedPositionNotSpowning(float dt) -> void {
    float direction = 0.0f;

    if(!isSetCagedFirstMovement) {
        movement = GetRandomValue(0, 1) == 1 ? Movement::up : Movement::down;
        isSetCagedFirstMovement = true;
    }

    if(movement == Movement::down) direction = 1.0f;
    else if(movement == Movement::up) direction = -1.0f;

    if(direction > 0 && position.y + (float)Ghost::height >= 14.5f * 22 + (float)Ghost::height / 2 + 10) movement = Movement::up;
    else if(direction < 0 && position.y <= 14.5f * 22 - (float)Ghost::height / 2 - 10) movement = Movement::down;

    position.y += 60.0f * dt * direction;
}



//returns if the ghost is spowned
auto Ghost::spowned() -> bool {
    return state != GhostState::caged && !spowning;
}



//forces the ghost spown
auto Ghost::forceSpown() -> void {
    this->forcedSpown = true;
}