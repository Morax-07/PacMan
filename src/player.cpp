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
}



//player's destructor
//unloads data
Player::~Player() {
    UnloadTexture(texture);
}



//draws pacman lifes to the bottom right of the screen
void Player::drawLife(Vector2 position) {
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
void Player::draw() {
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
    DrawFPS(0, 0);
}



//checks the user inputs for the player
//updates the nextMovement attribute
void Player::input() {
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
void Player::update(float dt) {
    //updates the movement to nextMovement if possible
    updateMovement(dt);


    //stops updating position and animation if the player collides with an obstacle
    if(stoppedForCollision) return;


    //updates the position with movement
    updatePosition(dt);
    

    //updates the player's animation
    updateAnimation(dt);
}



//return the player's body hitbox for block collision detection
Rectangle Player::bodyHitbox() const {
    return Rectangle{position.x + (float)width / 2 - 10.95f, position.y + (float)height / 2 - 10.95f, 21.9f, 21.9f};
}



//return the player's mouth hitbox for pellet collision detection
Rectangle Player::mouthHitbox() const {
    return Rectangle{position.x + 2.5f, position.y + 2.5f, (float)width - 5, (float)height - 5};
}



//returns the hitbox directly above the player
//for movement update with nextMovement
Rectangle Player::upHitbox() const {
    return Rectangle{position.x + (float)width / 2 - 10.95f, position.y - 21.9f, 21.9f, 21.9f};
}



//returns the hitbox directly under the player
//for movement update with nextMovement
Rectangle Player::downHitbox() const {
    return Rectangle{position.x + (float)width / 2 - 10.95f, position.y + (float)height, 21.9f, 21.9f};
}



//returns the hitbox directy to the left of the player
//for movement update with nextMovement
Rectangle Player::leftHitbox() const {
    return Rectangle{position.x - 21.9f, position.y + (float)height / 2 - 10.95f, 21.9f, 21.9f};
}



//returns the hitbox directly to the right of the player
//for movement update with nextMovement
Rectangle Player::rightHitbox() const {
    return Rectangle{position.x + (float)width, position.y + (float)height / 2 - 10.95f, 21.9f, 21.9f};
}



//sets the pellets pointer to 
//std::vector<Pellet> Map::pellets
void Player::setPellets(std::vector<Pellet>* pellets) {
    this->pellets = pellets;
}



//sets the blocks pointer to
//std::vector<Block> Map::blocks
void Player::setBlocks(std::vector<Block>* blocks) {
    this->blocks = blocks;
}



//checks the collision between mouth hitbox and pellet hitbox
//erases the pellet if collision is detected
void Player::checkPelletsCollision() {
    std::erase_if(*pellets, [&](const Pellet& pellet) -> bool {
        if(CheckCollisionRecs(pellet.hitbox(), mouthHitbox())) {
            points += pellet.getPoints();
            return true;
        }
        return false;
    });
}



//checks the collision between body gitbox and block hitbox
//backtracks the player position if collision is detected to "free" the hitbox from the block
//sets stoppedForCollision to true if the collision is detected
void Player::checkBodyBlocksCollision() {
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
int Player::getPoints() const noexcept {
    return this->points;
}



//checks if the external hitbox in the same direction of the movement collides with a wall
//it slides the hitbox rectangle for the player alignement in the interval [-speed * dt * 1.5f, +speed * dt * 1.5f]
//it corrects the position of the player by an offset to align the player to the corridor
void Player::checkAndSetNextMovement(std::function<struct Rectangle(void)> hitbox, float dt) {
    const float tolerance = speed * dt * 1.5f; 
    const float step = 0.05f;      
    bool vertical = (nextMovement == Movement::up || nextMovement == Movement::down);

    //lambda function that checks if the slided hitbox is free
    auto isFree = [&](const Rectangle& r) {
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
void Player::updateMovement(float dt) {
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
void Player::updatePosition(float dt) {
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
}



//updates the player animation
void Player::updateAnimation(float dt) {
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