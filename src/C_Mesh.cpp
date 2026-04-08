/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "C_Mesh.h"

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
        
        glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(unsigned int), (const void*)0);

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
        glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, m_BoneIDs));

        // WEIGHTs
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(struct Vertex, m_Weights));

        // The effect is that we symply pass a pointer to the struct and it traslate into a glm::vec3
        // again translate to 3/2 float which translate into a byte array 

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);        

        glBindVertexArray(0);
};


void Draw(Mesh* mesh, GLuint* progID){
        // bind appropriate textures
    
        //unsigned int textureID  = vampire_?3:1;

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
            if(strcmp(name.c_str(),"material.texture_diffused") == 0){
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
        // Always a good practice to set everything back to default once configured
        //glActiveTexture(GL_TEXTURE0);
};

void showUniformVarValuePerVertex(GLuint* UBO, GLuint* programeId, Mesh* mesh, bool32 showIndices, bool32 showPos, bool32 showTexCoords, bool32 showBoneIds, bool32 showWeights, bool32 showFinalBoneMatrices){
//We already bind the specific VAO before
    int VertexSizeforVec4 = MAX_BONE_INFLUENCE * 100;
    int VertexSizeforVec3 = 3 * 100;
    glBindVertexArray(mesh->VAO);
    for(int k = 0; k < 52; k++){

        if(showIndices){
            GLuint index;   
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, k*sizeof(GLuint), sizeof(GLuint), &index);
            printf("index of %d vertex is: %d\n", k, index);
        }
        
        if(showPos){
        glm::vec3 readbackPositions;        
                GLintptr offset = (k*sizeof(struct Vertex));
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::vec3), &readbackPositions);
                printf("Position ID %d:", k);
                std::cout<<glm::to_string(readbackPositions);
                printf("\n");
            }
        
        if(showTexCoords){
        glm::vec2 readbackTexCoords;        
                GLintptr offset = (k*sizeof(struct Vertex)) + offsetof(struct Vertex, TexCoords);
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::vec2), &readbackTexCoords);
                printf("TexCoords ID %d:", k);
                std::cout<<glm::to_string(readbackTexCoords);
                printf("\n");
            }

        
    if(showBoneIds){
        int readbackBoneIDs[4];
        //readbackBoneIDs = new int (VertexSizeforVec4);
        if(readbackBoneIDs!=nullptr){
            GLintptr offset = (k*sizeof(Vertex)) + offsetof(struct Vertex, m_BoneIDs);
            glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(int)*MAX_BONE_INFLUENCE, readbackBoneIDs);
            printf("Bone IDs per vertex %d is: ", k);
            printf("[%d, %d, %d, %d] ", readbackBoneIDs[0], readbackBoneIDs[1], readbackBoneIDs[2], readbackBoneIDs[3]);
            printf("\n");
        }
    }

    if(showWeights){
        float readbackWeights[4];
        //readbackWeights = new float (VertexSizeforVec4);
        if(readbackWeights != nullptr){
            GLintptr offset = sizeof(Vertex)*k + offsetof(struct Vertex, m_Weights);
            glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float)*MAX_BONE_INFLUENCE, readbackWeights);
            if(readbackWeights[0]!=0.0f){
                printf("Bone weights per vertex %d: ", k);
                printf("[%f, %f, %f, %f]", readbackWeights[0], readbackWeights[1], readbackWeights[2], readbackWeights[3]);
                printf("\n");
            } else {
                continue;   
            }                
        }
    }
 }

    if(showFinalBoneMatrices) {

        //GLfloat matVal[16];
        GLfloat* matVal_;
        std::string matrixName_;
        glUseProgram(*programeId);

        //char index[1];
        //char indexx[2];
        GLint maxUniforms;

        for(int k = 0; k < 52; k++){
            matrixName_ = "finalBone.finalBoneMatrices[]";


                //if(k<10){
                    //sprintf(index, "%d", k);
                    //matrixName_.insert(matrixName_.size()-1, index);
                //} else {
                    //sprintf(indexx, "%d", k);
                    //matrixName_.insert(matrixName_.size()-1, indexx);                                     
                //}
//
            //printf("matrix name: %s, programmeID: %d\n", matrixName_.c_str(), *programeId);

            //GLint matrixLocation = glGetUniformLocation(*programeId,                                       (GLchar*)matrixName_.c_str());

            //printf("location of current finalbone matrices: %d\n", matrixLocation);
            
//NOTE: This one is for uniform buffer
            GLint matrixLocation = glGetUniformBlockIndex(*programeId,                                       (GLchar*)matrixName_.c_str());
//
            glBindBuffer(GL_UNIFORM_BUFFER, *UBO);

// 2. Map the buffer memory
            void* ptr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*16*52, GL_MAP_READ_BIT);

// 3. Read data using the offset obtained earlier
            matVal_ = (float*)((char*)ptr + (k*sizeof(GLfloat)*16));
            glUnmapBuffer(GL_UNIFORM_BUFFER);
// Read *member1Data...

// 4. Unmap the buffer

            //if(matrixLocation != -1){
                //glGetUniformfv(*programeId, matrixLocation, matVal);

                if(matVal_[0]!=0.0f){
                    printf("\nBone Matrix index %d is:", k);
                    for(int i = 0 ; i < 16; i++){
                        if(i==0||i==4||i==8||i==12){
                            i==0?printf("["):printf(", [");
                        }
                        (i==0||i==4||i==8||i==12)?printf("%f", matVal_[i]):printf(", %f", matVal_[i]);
                        if(i==3||i==7||i==11||i==15){
                            printf("]");
                        }
                    };
                    printf("\n");                                 
                }
            //} else {
                //GLenum error = glGetError();
                //if(error == GL_INVALID_VALUE){
                    //printf("ProgramID is not a value generated by OPENGL\n");
                //} else if(error == GL_INVALID_OPERATION){
                    //printf("ProgramID is not an object or successfully linked\n");                            
                //};                
            //}

//So function failed to retrieved matrix from shader

        }
    }
};
