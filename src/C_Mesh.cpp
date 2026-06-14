/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "C_Mesh.h"
// Display Font Buffer.
// Use bitmap font loading on 2D squad

void IncreaseFontAlpha(Glyph_Map* map){
    uint8 *Source = map->bitmap;
    for(int y = 0; y < map->h; y++){
        for(int x = 0; x < map->w; x++){
            uint8 alpha = *Source;
            *Source = ((alpha << 24)|
                       (alpha << 16)|
                       (alpha << 8)|
                       (alpha << 0));
            *Source++; 
        }
    };
}

void LoadFont(const char* path){

    debug_read_file_result* TTFfile = DEBUGReadFileWhole(path);
    stbtt_InitFont(&Glyphs_Map.FontInfo, (unsigned char*)TTFfile->Content, stbtt_GetFontOffsetForIndex((unsigned char*)TTFfile->Content, 0));

    Glyphs_Map.bitmap = stbtt_GetCodepointBitmap(&Glyphs_Map.FontInfo, 0,stbtt_ScaleForPixelHeight(&Glyphs_Map.FontInfo, 128.0f), 'A', &Glyphs_Map.w, &Glyphs_Map.h, &Glyphs_Map.Xoffset, &Glyphs_Map.Yoffset);

    
    
    if(Glyphs_Map.bitmap != nullptr){
        printf("Load Font successfully\n");
        IncreaseFontAlpha(&Glyphs_Map);
        glGenTextures(1, &Glyphs_Map.TextureID);

        glActiveTexture(GL_TEXTURE0+Glyphs_Map.TextureID);
        glBindTexture(GL_TEXTURE_2D, Glyphs_Map.TextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, Glyphs_Map.w, Glyphs_Map.h, 0, GL_RED, GL_UNSIGNED_BYTE, Glyphs_Map.bitmap);
        glGenerateMipmap(GL_TEXTURE_2D);
        // can free temp_bitmap at this point
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        stbtt_FreeBitmap(Glyphs_Map.bitmap, 0);
    }else{
        printf("Failed Loading font\n");
    }

    
}

//==================================================================
// Bone processing step for skeletal animation
void SetVertexBoneData(Vertex* vertex, int boneID, float weight){
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++){
        if(vertex->m_Weights[i] == 0.0f){
            vertex->m_BoneIDs[i] = boneID;
            vertex->m_Weights[i] = weight;
            break;
        }
    }
};

void setupMesh(Mesh* mesh){

        glGenVertexArrays(1, &mesh->VAO);
        glGenBuffers(1, &mesh->VBO);
        glGenBuffers(1, &mesh->EBO);

        glBindVertexArray(mesh->VAO);
        // Good thing about struct is that their memory is sequential for all its time
        //glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(unsigned int), (const void*)0);
        
        // Load data into vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size()*sizeof(struct Vertex), &mesh->vertices[0], GL_STATIC_DRAW);        
        // Time to set vertex attribute pointers
        // POSITION
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);

        // NORMAL
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, Normal));

        // TEXCOORDS
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, TexCoords));

        // TANGENT
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, Tangent));

        // BITANGENT
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, Bitangent));

        // IDS
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (const void*)offsetof(struct Vertex, m_BoneIDs));
        //printf("Current pointer in vertex struct after m_BoneIDs: %d\n", (int)offsetof(struct Vertex, m_BoneIDs));
        // WEIGHTs
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, m_Weights));
        //printf("Current pointer in vertex struct after m_Weights set up: %d\n", (int)offsetof(struct Vertex, m_Weights));
        // The effect is that we symply pass a pointer to the struct and it traslate into a glm::vec3
        // again translate to 3/2 float which translate into a byte array 

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);        

        glBindVertexArray(0);
};


void Draw(Mesh* mesh, GLuint* progID){
        // bind appropriate textures

    glBindVertexArray(0);
    useProgram(0);
        //unsigned int textureID  = vampire_?3:1;

        unsigned int ambientNr  = 1;
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;

        glBindVertexArray(mesh->VAO);
        useProgram(*progID);
        int textureID;
        for(unsigned int i = 0; i < mesh->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0+mesh->textures[i].id);
// retrieve texture number (the N in diffuse_textureN)
            unsigned int number;
            std::string name = mesh->textures[i].type;
            if(strcmp(name.c_str(),"material.texture_ambient") == 0){
                number = ambientNr++;

            }else if(strcmp(name.c_str(),"material.texture_diffused") == 0){

                number = diffuseNr++;
            }else if(strcmp(name.c_str(),"material.texture_specular")==0){
                
                number = specularNr++; // transfer unsigned int to string
            }else if(strcmp(name.c_str(),"material.texture_normal")==0){
                
                number = normalNr++; // transfer unsigned int to string
            } else if(strcmp(name.c_str(),"material.texture_height")==0){
                
                number = heightNr++; // transfer unsigned int to string
            }
            // now set the sampler to the correct texture unit
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
            glUniform1i(glGetUniformLocation((*progID), (name+std::to_string(number)).c_str()), mesh->textures[i].id);
        }

        // Draw the mesh(Bind array, Load model, draw element/array)
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        useProgram(0);
        // Always a good practice to set everything back to default once configured
        glActiveTexture(GL_TEXTURE0);
};
