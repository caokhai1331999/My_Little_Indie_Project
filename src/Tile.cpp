/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Tile.h"

Tile* LoadTileMap();
void drawTile(unsigned int VaoID, unsigned int shaderID, float speed, float* updatedDegree){

    glUseProgram(shaderID);
    float MapDimensInX = 10.8f;
    float MapDimensInZ = 10.8f;
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 rrmodel = glm::mat4(1.0f);
    glm::mat4 model_ = glm::mat4(1.0f);
    glm::vec3 tempTilePos = glm::vec3(0.0f);

    
    int index = 0;
    float z__ = 0.0f;
    float x__ = 0.0f;
    for (float z_ = 0.0; z_ < MapDimensInZ; z_+=TILE_Z){
//    while(z__ < MapDimensInZ)
//    {
        //printf("z loop index :%d\n", (int)(z__/1.2));
        for (float x_ = 0.0; x_ < MapDimensInX; x_+=TILE_X){
//        while(x__ < MapDimensInX)
//        {
            //printf("x loop index :%d\n", (int)(x_/1.2));
            index = (int)((int)((z_/1.2f) * 10) + (int)(x_/1.2f));

            if(fluxY[index + 100] == (float)UPP_){
                fluxY[index]+=speed*0.01f;
            } else if (fluxY[index + 100] == (float)DOWNN_) {
                fluxY[index]-=speed*0.01f;
            } else {
                fluxY[index]-=0.0f;
            }
            
            if(fluxY[index] > 1.0f)
            {
                fluxY[index] = 1.0f;
                fluxY[index + 100] = (float)DOWNN_;
            }

            if(fluxY[index] < -1.0f)
            {
                fluxY[index] = -1.0f;
                fluxY[index + 100] = (float)UPP_;
            }

            //printf("cube index %d Y: %f, with direction %s\n", index, fluxY[index], fluxY[index+100]==UPP_?"UP":"DOWN");

            if(tempTilePos.x != x_){tempTilePos.x = x_;}
            if(tempTilePos.z != z_){tempTilePos.z = z_;}
            if(index >= 0 && index <=99){
                if(tempTilePos.y != fluxY[index])
                {
                    tempTilePos.y = fluxY[index];
                }

            }
            if(fluxY[index+100] == (float)ROLL_)
            {
                rrmodel = glm::translate(model_,tempTilePos);
                rrmodel = glm::rotate(rrmodel, glm::radians(*updatedDegree) *speed, glm::vec3(0.0f, index%2==0?0.5f:0.0f, index%2!=0?0.4f:0.0f));
                setMat4(shaderID, "model", rrmodel);
            } else {
                model = glm::translate(model_, tempTilePos); 
                setMat4(shaderID, "model", model);
            }
            glDrawArrays(GL_TRIANGLES, 0, 36);
            //x__+= TILE_X;
        }        
        //z__+= TILE_Z;
    }
    
    //while (tempTilePos.z < 10.0f){
        //if(tempTilePos.x = 10.0f){
            //tempTilePos.x = 0.0f;
            //tempTilePos.z += 1.0f;
        //} else {
            //tempTilePos.x += 1.0f;
        //
        //}
        //model = glm::translate(model_, tempTilePos) ;
        //setMat4(shaderID, "model", model);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
    //}
    // Repeatedly drawing a half cube as a tile
}
    
