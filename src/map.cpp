#include "./../include/map.h"
#include <fstream>
#include <functional>
#include <raylib.h>
#include <sstream>
#include <string>
#include <vector>







//map constructor
Map::Map(): position(Vector2 {0, 0}), width(616), height(682) {
    //reads the map file and convertsthe read numbers into integers and
    //pushes them into a short ints matrix
    std::ifstream mapFile("../bin/data/file/gameMap.txt"); 
    short int number;
    std::vector<std::vector<short int>> mapNumbers;
    

    if (!mapFile.is_open()) throw std::runtime_error("An error occurred trying to open the file!");


    std::string line;
    while(std::getline(mapFile, line)) {
        std::stringstream ss(line);
        std::string strNumber;


        std::vector<short int> row;
        while(std::getline(ss, strNumber, ',')) {
            if(!strNumber.empty()) number = std::stoi(strNumber);  
            
            row.shrink_to_fit();
            row.push_back(number);
        }

        mapNumbers.push_back(row);
        mapNumbers.shrink_to_fit();
    }

    mapFile.close();


    loadMapComponents(std::ref(mapNumbers));
    loadMapTexture();
}

//map destructor
Map::~Map() {
    UnloadRenderTexture(mapTexture);
}

//loads all the components of the map
//istantiates all the Blockand the pellets of the map and puts them into blocks and pellets arrays
void Map::loadMapComponents(std::vector<std::vector<short int>>& mapNumbers) {
    float tileWidth = Block::getWidth();
    float tileHeight = Block::getHeight();


    for(float y = 0; y < mapNumbers.size(); y++) {
        for(float x = 0; x < mapNumbers[0].size(); x++) {
            switch (mapNumbers[y][x]) {
                //hollow tiles
                case 0:
                    break;


                //extern walls for map's sides and ghosts spown
                case 1:
                    blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight}, 0, 0, false));
                    break;

                case 2:
                    blocks.push_back(Block(Vector2 {x * tileWidth + tileWidth, y * tileHeight}, 0, 90, false));
                    break;

                case 3:
                    blocks.push_back(Block(Vector2 {x * tileWidth + tileWidth, y * tileHeight + tileHeight}, 0, 180, false));
                    break;

                case 4:
                    blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight + tileHeight}, 0, 270, false));
                    break;

                
                //some turns
                case 5:
                    //big left turns
                    if(x == 0) {
                        if(y == 0 || y == 19) 
                            blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight}, 1, 0, false));

                        if(y == 9 || y == 30)
                            blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight + tileHeight}, 1, 270, false));
                    }
                    //ghost spown turns
                    else if(x == 10) {
                        if(y == 12)
                            blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight}, 3, 0, false));

                        if(y == 16)
                            blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight + tileHeight}, 3, 270, false));
                    }
                    else if(x == 17) {
                        if(y == 12)
                            blocks.push_back(Block(Vector2 {x * tileWidth + tileWidth, y * tileHeight}, 3, 90, false));

                        if(y == 16) 
                            blocks.push_back(Block(Vector2 {x * tileWidth + tileWidth, y * tileHeight + tileHeight}, 3, 180, false));
                    }
                    //big right turn
                    else if(x == mapNumbers[0].size() - 1) {
                        if(y == 0 || y == 19) 
                            blocks.push_back(Block(Vector2 {x * tileWidth + tileWidth, y * tileHeight}, 1, 90, false));

                        if(y == 9 || y == mapNumbers.size() - 1)
                            blocks.push_back(Block(Vector2 {x * tileWidth + tileWidth, y * tileHeight + tileHeight}, 1, 180, false));
                    }
                    
                    break;

                //ghost spown edges
                case 6:
                    if(mapNumbers[y][x - 1] == 4)
                        blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight}, 2, 0, false));
                    else
                        blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight}, 2, 0, true));

                    break;

                //ghosts spown door
                case 7:
                    blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight}, 4, 0, false));
                    break;

                //small turns
                case 8:
                    if ((mapNumbers[y][x + 1] != 0 && mapNumbers[y][x + 1] != 15 && mapNumbers[y][x + 1] != 16) && (mapNumbers[y + 1][x] != 0 && mapNumbers[y + 1][x] != 15 && mapNumbers[y + 1][x] != 16))
                        blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight }, 5, 0, false));
                    else if ((mapNumbers[y][x - 1] != 0 && mapNumbers[y][x - 1] != 15 && mapNumbers[y][x - 1] != 16) && (mapNumbers[y + 1][x] != 0 && mapNumbers[y + 1][x] != 15 && mapNumbers[y + 1][x] != 16))
                        blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight }, 5, 90, false));
                    else if ((mapNumbers[y][x - 1] != 0 && mapNumbers[y][x - 1] != 15 && mapNumbers[y][x - 1] != 16) && (mapNumbers[y - 1][x] != 0 && mapNumbers[y - 1][x] != 15 && mapNumbers[y - 1][x] != 16))
                        blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight + tileHeight }, 5, 180, false));
                    else if ((mapNumbers[y][x + 1] != 0 && mapNumbers[y][x + 1] != 15 && mapNumbers[y][x + 1] != 16) && (mapNumbers[y - 1][x] != 0 && mapNumbers[y - 1][x] != 15 && mapNumbers[y - 1][x] != 16))
                        blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight + tileHeight }, 5, 270, false));

                    break;

                //medium turns
                case 9:
                    if (mapNumbers[y + 1][x] == 12 && mapNumbers[y][x + 1] == 14)
                        blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight }, 6, 0, false));
                    else if (mapNumbers[y + 1][x] == 11 && mapNumbers[y][x - 1] == 14)
                        blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight }, 6, 90, false));
                    else if (mapNumbers[y - 1][x] == 11 && mapNumbers[y][x - 1] == 13)
                        blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight + tileHeight }, 6, 180, false));
                    else if (mapNumbers[y - 1][x] == 12 && mapNumbers[y][x + 1] == 13)
                        blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight + tileHeight }, 6, 270, false));
    
                    break;
    
                //external turns
                case 10:
                    if (y == 0) {
                        if (mapNumbers[y][x + 1] == 10)
                            blocks.push_back(Block(Vector2 {x * tileWidth, y * tileHeight }, 8, 0, false));
                        else if (mapNumbers[y][x - 1] == 10)
                            blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight }, 7, 0, false));
                    }
                    else if (x == 0) {
                        if (mapNumbers[y + 1][x] == 10)
                            blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight + tileHeight }, 7, 270, false));
                        else if (mapNumbers[y - 1][x] == 10)
                            blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight + tileHeight }, 8, 270, false));
                    }
                    else if (x == mapNumbers[y].size() - 1) {
                        if (mapNumbers[y + 1][x] == 10)
                            blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight }, 8, 90, false));
                        else if (mapNumbers[y - 1][x] == 10)
                            blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight }, 7, 90, false));
                    }

                    break;

                //internal walls
                case 11:
                    blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight }, 9, 0, true));
                    break;

                case 12:
                    blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight }, 9, 0, false));
                    break;

                case 13:
                    blocks.push_back(Block(Vector2 { x * tileWidth, y * tileHeight + tileHeight }, 9, 270, false));
                    break;

                case 14:
                    blocks.push_back(Block(Vector2 { x * tileWidth + tileWidth, y * tileHeight }, 9, 90, false));
                    break;

                //small pellet
                case 15:
                    pellets.push_back(Pellet(2, Vector2 {x * tileWidth + (tileWidth / 2), y * tileHeight + (tileHeight / 2)}));
                    break;
                
                //big pellet
                case 16:
                    pellets.push_back(Pellet(7, Vector2 {x * tileWidth + (tileWidth / 2), y * tileHeight + (tileHeight / 2)}));
                    break;

                default:
                    throw std::runtime_error("The value doesn't correspond to any tile!");
                    break;
            }
        }
    }

    pellets.shrink_to_fit();
    blocks.shrink_to_fit();
}

//loads the full map texture (all the blocks)
void Map::loadMapTexture() {
    mapTexture = LoadRenderTexture(width, height);

    BeginTextureMode(mapTexture);
        ClearBackground(BLACK);

        for(auto& block: blocks) block.draw();
    EndTextureMode();



    Block::unloadTextures();
}

//draws the map
void Map::draw() {
    DrawTextureRec(mapTexture.texture, {0, 0, (float)mapTexture.texture.width, -(float)mapTexture.texture.height}, position, WHITE);
    for(auto& pellet : pellets) pellet.draw();
}

//updates the map
void Map::update(float dt) {
    Pellet::update(dt);
}

std::vector<Pellet>* Map::getPellets() {
    return &this->pellets;
}

std::vector<Block>* Map::getBlocks() {
    return &this->blocks;
}