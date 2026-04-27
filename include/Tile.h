#if !defined(TILE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ctime>

#include "C_Model.h"
#include "win32Game.h"

#define DOWNN_ 0.0f
#define UPP_ 1.0f
#define ROLL_ 2.0f

using namespace std;

static const float TILE_Z = 1.2f;
static const float TILE_X = 1.2f;
static const int8_t TOTAL_TILE_NUMBER = 50;
float fluxY[200] = {};

// NOTE: 1st assign object postion by translate
//       2nd assign right VAOS for different flat cube shapes
//       3rd assign texture index for shader too
//       finally, draw it out by using glDrawArray
//
// Must create a map for this
//const TileIndices[] = {
    //
//};

struct rollCubeInfo{
    int index_;
    int axisIndex_;
};

// We loop over the tiles array and check for collision every frame

struct Tile{
    glm::vec2 position;
    glm::vec3 size;
    // Position, scale
    glm::mat4 tileContainer;
    unsigned int TextureID = 0;

    // Tile indices
    Tile(){
        if (position!= glm::vec2(0.0f)){
            position = glm::vec2(0.0f);
        }

        if (TextureID!= 0){
            TextureID = 0;
        }

        glm::mat4 tileContainer = glm::mat4(1.0f);
    };
};

Tile* LoadTileMap();
void drawTile(unsigned int VaoID = 0, unsigned int shaderID = 0, float speed = 1.0, float* updatedDegree = nullptr, bool32 changeAxis = false, std::vector<rollCubeInfo>* rollCubemap = nullptr);
#define TILE_H
#endif

