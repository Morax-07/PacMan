#include "./../include/player.h"
#include <functional>
#include <raylib.h>










//PLAYER CLASS METHODS
//player's constructor
//sets initial attributes value and load data
Player::Player(): width(34), height(32) {
    this->life = 4;
    this->texture = LoadTexture("../bin/data/texture/pacman.png");
    this->textureRectangle = {0, 0, (float)width, (float)height};
    this->position = {(float)(308 - (float)width / 2), 501};
    this->movement = Movement::still;
    this->nextMovement = Movement::still;
    this->speed = 120;
    this->animationInterval = 0.05f;
    this->animationTimer = 0.0f;
    this->textureRecXDirection = 42;
    this->rotation = 0;
    this->points = 0;
    this->stoppedForCollision = false;
    this->positionBounds[0] = 0;
    this->currentDirection = Movement::still;
    this->eatenPellets = 0;
    this->notEatingInterval = 4.0f;
    this->notEatingTimer = 0.0f;
    this->notEating = false;
}



//player's destructor
//unloads data
Player::~Player() {
    UnloadTexture(texture);
}



//draws pacman lifes to the bottom right of the screen
auto Player::drawLife(Vector2 position) -> void {
    for(int i = 0; i < life - 1; i++) {
        DrawTexturePro(
            texture, 
            Rectangle {126, 0, (float)width, (float)height}, 
            Rectangle {position.x, position.y, 30, 30}, 
            Vector2 {0, 0}, 
            0, 
            WHITE
        );
        position.x += 37;
    }
}



//draws pacman
auto Player::draw() -> void {
    DrawTexturePro(
        texture, 
        textureRectangle, 
        Rectangle{
            position.x + (float)width / 2, 
            position.y + (float)height / 2, 
            (float)width, 
            (float)height
        }, 
        Vector2{(float)width / 2, (float)height / 2}, 
        rotation, 
        WHITE
    );
}



//checks the user inputs for the player
//updates the nextMovement attribute
auto Player::input() -> void {
    KeyboardKey pressedKey = (KeyboardKey)GetKeyPressed();
    switch(pressedKey) {
        case KEY_UP:
            nextMovement = Movement::up;
            break;

        case KEY_DOWN:
            nextMovement = Movement::down;
            break;

        case KEY_LEFT:
            nextMovement = Movement::left;
            break;

        case KEY_RIGHT:
            nextMovement = Movement::right;
            break;

        default:
            break;
    }
}



//updates the player 
auto Player::update(float dt) -> void {
    //updates the movement to nextMovement if possible
    updateMovement(dt);


    //sets the current direction for the ghosts target updating
    if(movement != Movement::still) currentDirection = movement;


    //updates not eating timer
    updateNotEating(dt);


    //stops updating position and animation if the player collides with an obstacle
    if(stoppedForCollision) return;


    //updates the position with movement
    updatePosition(dt);
    

    //updates the player's animation
    updateAnimation(dt);
}



//return the player's body hitbox for block collision detection
auto Player::bodyHitbox() const -> Rectangle {
    return Rectangle{position.x + (float)width / 2 - 10.95f, position.y + (float)height / 2 - 10.95f, 21.9f, 21.9f};
}



//return the player's mouth hitbox for pellet collision detection
auto Player::mouthHitbox() const -> Rectangle {
    return Rectangle{position.x + 2.5f, position.y + 2.5f, (float)width - 5, (float)height - 5};
}



//returns the hitbox directly above the player
//for movement update with nextMovement
auto Player::upHitbox() const -> Rectangle {
    return Rectangle{position.x + (float)width / 2 - 10.95f, position.y - 21.9f, 21.9f, 21.9f};
}



//returns the hitbox directly under the player
//for movement update with nextMovement
auto Player::downHitbox() const -> Rectangle {
    return Rectangle{position.x + (float)width / 2 - 10.95f, position.y + (float)height, 21.9f, 21.9f};
}



//returns the hitbox directy to the left of the player
//for movement update with nextMovement
auto Player::leftHitbox() const -> Rectangle {
    return Rectangle{position.x - 21.9f, position.y + (float)height / 2 - 10.95f, 21.9f, 21.9f};
}



//returns the hitbox directly to the right of the player
//for movement update with nextMovement
auto Player::rightHitbox() const -> Rectangle {
    return Rectangle{position.x + (float)width, position.y + (float)height / 2 - 10.95f, 21.9f, 21.9f};
}



//sets the pellets pointer to 
//std::vector<Pellet> Map::pellets
auto Player::setPellets(std::vector<Pellet>* pellets) -> void {
    this->pellets = pellets;
}



//sets the blocks pointer to
//std::vector<Block> Map::blocks
auto Player::setBlocks(const std::vector<Block>* blocks) -> void {
    this->blocks = blocks;
}



//checks the collision between mouth hitbox and pellet hitbox
//erases the pellet if collision is detected
auto Player::checkPelletsCollision() -> void {
    std::erase_if(*pellets, [&](const Pellet& pellet) -> bool {
        if(CheckCollisionRecs(pellet.hitbox(), mouthHitbox())) {
            points += pellet.getPoints();
            eatenPellets++;
            resetNotEating();
            Global::bigPelletEaten = pellet.isBig();
            return true;
        }
        return false;
    });
}



//checks the collision between body gitbox and block hitbox
//backtracks the player position if collision is detected to "free" the hitbox from the block
//sets stoppedForCollision to true if the collision is detected
auto Player::checkBodyBlocksCollision() -> void {
    bool collisionDetected = false;

    for(const auto& block : *blocks) {
        while(CheckCollisionRecs(block.hitbox(), bodyHitbox()) && !stoppedForCollision) {
            collisionDetected = true;
            switch(movement) {
                case Movement::up:
                    position.y += 0.1;
                    break;

                case Movement::down:
                    position.y -= 0.1;
                    break;
                
                case Movement::left:
                    position.x += 0.1;
                    break;
                
                case Movement::right:
                    position.x -= 0.1;
                    break;

                default:
                    break;
            }
        }

        if(collisionDetected) break;
    }

    if(collisionDetected) stoppedForCollision = true;
}



//returns the value of the point to draw in MainPage
auto Player::getPoints() const noexcept -> int {
    return this->points;
}



//checks if the external hitbox in the same direction of the movement collides with a wall
//it slides the hitbox rectangle for the player alignement in the interval [-speed * dt * 1.5f, +speed * dt * 1.5f]
//it corrects the position of the player by an offset to align the player to the corridor
auto Player::checkAndSetNextMovement(std::function<struct Rectangle(void)> hitbox, float dt) -> void {
    if(isOutOfBounds()) return;

    const float tolerance = speed * dt * 1.5f; 
    const float step = 0.05f;      
    bool vertical = (nextMovement == Movement::up || nextMovement == Movement::down);

    //lambda function that checks if the slided hitbox is free
    static auto isFree = [&](const Rectangle& r) {
        for(const auto& block : *blocks)
            if(CheckCollisionRecs(r, block.hitbox())) return false;
        return true;
    };


    for(float offset = 0; offset <= tolerance; offset += step) {
        for(float sign : {1.0f, -1.0f}) {
            Rectangle r = hitbox();

            //slides the hitbox
            if(vertical) 
                r.x += sign * offset;
            else 
                r.y += sign * offset;

            
            if(isFree(r)) {
                //if the slided hitbox is free, it alignes the player
                if(vertical) position.x += sign * offset;
                else         position.y += sign * offset;

                movement = nextMovement;

                //since the movement is set to nextMovement, stoppedForCollision is set to false
                //if the nextMovement causes the player to collide with a wall again it will be 
                //set again to true
                stoppedForCollision = false;
                return;
            }
        }
    }
}



//updates the movement by checking which is the nextMovement and checking if it can be set
auto Player::updateMovement(float dt) -> void {
    switch(nextMovement) {
        case Movement::up:
            checkAndSetNextMovement([this]() -> Rectangle {return upHitbox();}, dt);
           break;
        
        case Movement::down:
            checkAndSetNextMovement([this]() -> Rectangle {return downHitbox();}, dt);
            break;

        case Movement::left:
            checkAndSetNextMovement([this]() -> Rectangle {return leftHitbox();}, dt);
            break;

        case Movement::right:
            checkAndSetNextMovement([this]() -> Rectangle {return rightHitbox();}, dt);
            break;

        default:
            break;
    }
}



//updates the position by switching the current movement
auto Player::updatePosition(float dt) -> void {
    switch (movement) {
        case Movement::up:
            position.y -= speed * dt;
            rotation = 270;
            break;

        case Movement::down:
            position.y += speed * dt;
            rotation = 90;
            break;

        case Movement::left:
            position.x -= speed * dt;
            rotation = 180;
            break;

        case Movement::right:
            position.x += speed * dt;
            rotation = 0;
            break;

        default:
            rotation = 0;
            break;
    }


    //pacman effect
    if(position.x <= positionBounds[0] - width - 30 && movement == Movement::left) 
        position.x = positionBounds[1] + 30;
    else if(position.x >= positionBounds[1] + 30 && movement == Movement::right) 
        position.x = positionBounds[0] - width - 30;
}



//updates the player animation
auto Player::updateAnimation(float dt) -> void {
    if(movement != Movement::still) {
        animationTimer += dt;

        if(animationTimer >= animationInterval) {
            animationTimer -= animationInterval;
            
            if(textureRectangle.x == 0) textureRecXDirection = 42;
            if(textureRectangle.x == 126) textureRecXDirection -= 42;

            textureRectangle.x += textureRecXDirection;
        }
    }
}



//sets the right coordinate for the map boundries
auto Player::setRightPositionBound(float x) -> void {
    positionBounds[1] = x;
}



//checks if the player is out of bounds and sets outOfBound
auto Player::isOutOfBounds() -> bool {
    return (position.x <= positionBounds[0] || position.x >= positionBounds[1] - (float)width / 2);
}



//returns a pointer to the player position
auto Player::getPosition() const noexcept -> const Vector2* {
    return &this->position;
}



//returns a pointer to the player movement
auto Player::getMovement() const noexcept -> const Movement* {
    return &this->currentDirection;
}



//checks the collisions with ghosts
auto Player::checkGhostCollision(const Ghost* ghost) -> void {
    if(CheckCollisionRecs(bodyHitbox(), ghost->bodyHitbox())) {
        switch(ghost->getState()) {
            case GhostState::frightened:
                ghost->setStateToEaten();


                Global::ghostPoints = 200 * (1 << Global::ghostEaten);
                Global::ghostPointsDrawingPosition = {
                    .x = ghost->getPosition().x + (float)Ghost::width / 2,
                    .y = ghost->getPosition().y
                };


                points += Global::ghostPoints;
                Global::ghostEaten++;
                Global::stopForEatenGhost = true;
                break;

            case GhostState::scatter:
            case GhostState::chase:
                break;

            default:
                break;
        }
    }
}



//gets a pointer to the number of pellets eaten
auto Player::getEatenPellets() const noexcept -> const int* {
    return &this->eatenPellets;
}



//updates not eating
auto Player::updateNotEating(float dt) -> void {
    if(notEating) return;

    notEatingTimer += dt;
    if(notEatingTimer >= notEatingInterval) {
        notEatingTimer = 0.0f;
        notEating = true;
    }
}



//returns if the player is not eating
auto Player::isNotEating() const noexcept -> bool {
    return this->notEating;
}



//resets not eating status
auto Player::resetNotEating() -> void {
    notEatingTimer = 0.0f;
    notEating = false;
}