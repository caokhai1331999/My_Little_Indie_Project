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
float fluxY[200] = {};
static const int8_t TOTAL_TILE_NUMBER = 50;

// NOTE: 1st assign object postion by translate
//       2nd assign right VAOS for different flat cube shapes
//       3rd assign texture index for shader too
//       finally, draw it out by using glDrawArray
//
// Must create a map for this
//const TileIndices[] = {
    //
//};

struct Tile{
    glm::vec3 position = {};
    int TextureIndex = 0;
    // Tile indices
    Tile(){
        if (position!= glm::vec3(0.0f)){
            position = glm::vec3(0.0f);
        }

        if (TextureIndex!= 0){
            TextureIndex = 0;
        }
    };
};

Tile* LoadTileMap();
void drawTile(unsigned int VaoID = 0, unsigned int shaderID = 0, float speed = 1.0, float* updatedDegree = nullptr);

#define TILE_H
#endif

