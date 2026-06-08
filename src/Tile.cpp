/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Tile.h"

Tile* LoadTileMap();

void drawTile(unsigned int VaoID, unsigned int shaderID, float speed, float* updatedDegree, bool32 changeAxis, std::vector<rollCubeInfo>* rollCubemap){

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
    int Cubeindex = 0;

//NOTE: loop through z dimens then x
    for (float z_ = 0.0; z_ < MapDimensInZ; z_+=TILE_Z){
//    while(z__ < MapDimensInZ)
//    {
        //printf("z loop index :%d\n", (int)(z__/1.2));
        for (float x_ = 0.0; x_ < MapDimensInX; x_+=TILE_X){
//        while(x__ < MapDimensInX)
//        {
            //printf("x loop index :%d\n", (int)(x_/1.2));
            index = (int)((int)((z_/1.2f) * 10) + (int)(x_/1.2f));
            // NOTE: check the pre-defined moving direction
            // then update the position of it
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
            if (fluxY[index + 100] == (float)ROLL_) {

                if(index == (*rollCubemap)[Cubeindex].index_){
                    if(changeAxis){
                        (*rollCubemap)[Cubeindex].axisIndex_ = std::rand()%3;
                    }
                }

                rrmodel = glm::translate(model_,tempTilePos);
                rrmodel = glm::rotate(rrmodel, glm::radians((*updatedDegree)), randomRotateAxis_((*rollCubemap)[Cubeindex].axisIndex_));
                //rrmodel = glm::rotate(rrmodel, (*updatedDegree) , randomRotateAxis_((*rollCubemap)[Cubeindex].axisIndex_));

                setMat4(shaderID, "model", rrmodel);

                Cubeindex++;
            } else {
                model = glm::translate(model_, tempTilePos); 
                setMat4(shaderID, "model", model);
            }
            //glDrawArrays(GL_TRIANGLES, 0, 36);
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
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

unsigned int SetupTileTexture(const char* path){

    int width, height, nrComponents;
    unsigned char *data = new unsigned char;
    data = stbi_load(path, &width, &height, &nrComponents, 0);    
// NOTE: Focus on this
    unsigned int textureID = 0;

    if(data){
        glGenTextures(1, &textureID);

        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glActiveTexture(GL_TEXTURE0+textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        //glPixelStorei(GL_UNPACK_ROW_LENGTH, width);

        printf("image width: %d, height:%d, format:%d", width, height, format);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);

        //Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        //Filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

                
        GLenum err = glGetError();

        if (err != 0) {
            printf("OpenGL Error after glTexImage2D: %x\n", err);
            printf("Succeed load and assign image data to textureID: %d\n", textureID);
        }    
        stbi_image_free(data);
    } else {
        printf("image data is NULL\n");
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);        
    return textureID;
}

void set_tile_vertex(B_shader_program* shader){

    glGenVertexArrays(1, &TileObj.TileVAO);
    glGenBuffers(1, &TileObj.TileEBO);
    glGenBuffers(1, &TileObj.TileVBO);
    glGenBuffers(1, &TileObj.TileColorBO);

    glBindVertexArray(TileObj.TileVAO);

    glBindBuffer(GL_ARRAY_BUFFER, TileObj.TileVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(land_vertices), &land_vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
//TexCoords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)3);

    glBindBuffer(GL_ARRAY_BUFFER, TileObj.TileColorBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), &g_color_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, TileObj.TileEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Tile_Land_Indices), &Tile_Land_Indices, GL_STATIC_DRAW);

    // Set position for instanced draw
    shader->use();
    TileObj.TextureID = SetupTileTexture(".\media\");
    
    //Tile Object here
    // Then we have something like trees, monsters, ....
    glm::vec2 offset;
    std::string name;
        for(int l = 0; l < 30; l++){
            for(int w = 0; w < 30; w++){
            offset.x = (float)l;
            offset.y = (float)w;
            int ID = w + (30 * l);
            name = "offsets["+std::to_string(ID)+"]";
            shader->setVec2(name.c_str(), offset);
        }
    }

glUseProgram(0);
}


void drawTile(const unsigned int VaoID, const unsigned int TextureID, B_shader_program* const Brush){
    
    glBindVertexArray(TileObj.TileVAO);
    Brush->use();
// Set Texture ID here
    Brush->setInt(TileObj.TextureID, "material.diffused_texture");
// Set Light Position based on Texture Coordinated
    glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, 900);

// Scale here

            // Then set textureId here
            //Brush->setMat4("model", tile_container);
            //Brush->setInt("ttexture", TextureID);
            //Brush->setBool("textPass", true);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //}
    //}
    glUseProgram(0);
};
