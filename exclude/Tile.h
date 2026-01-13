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

using namespace std;

static const float TILE_Z = 1.0f;
static const float TILE_X = 1.0f;

const TOTAL_TILE_NUMBER = 50;

// NOTE: 1st assign object postion by translate
//       2nd assign right VAOS for different flat cube shapes
//       3rd assign texture index for shader too
//       finally, draw it out by using glDrawArray
//
// Must create a map for this
const TileIndices[] = {
    
};

struct Tile{
    glm::vec3 position = {};
    int TextureIndex = 0;
    // Tile indices
    Tile(){
        if (postion!= glm::vec3(0.0f)){
            postion = glm::vec3(0.0f);
        }

        if (TextureIndex!= 0){
            TextureIndex = 0;
        }
    };
};

Tile* LoadTileMap();
void drawTile(Map_content* content = nullptr, uint8_t VaoID = 0, uint8_t shaderID = 0);

#define TILE_H
#endif

