/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Tile.h"

Tile* LoadTileMap();
void drawTile(Map_content* content, uint8_t VaoID = 0, uint8_t shaderID = 0){

    glUseProgram(shaderID);
    float MapDimensInX = 10.0f;
    float MapDimensInZ = 10.0f;
    glm::vec3 tempTilePos = glm::vec3(0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    
    for (float y = 0.0; y < MapDimensInZ; x+=TILE_Z){
        for (float x = 0.0; x < MapDimensInX; x+=TILE_X){
            glm::tempTilePos.x = x;
            glm::tempTilePos.y = y;
            model = glm::translate(model, tempTilePos);
            setMat4(shaderID, "model", model);
            glDrawArray(GL_TRIANGLES, 0, 36);
        }        
    }
    // Repeatedly drawing a half cube as a tile
}
    
