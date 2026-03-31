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
            glUniform1i(glGetUniformLocation(*progID, (name+std::to_string(number)).c_str()), mesh->textures[i].id);
            //printf("TextureID: mesh->textures[i].id: %d, sampler2D name: %s\n", mesh->textures[i].id, (name+std::to_string(number)).c_str());
        }

        // Draw the mesh(Bind array, Load model, draw element/array)
        useProgram(*progID);
        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

        // Always a good practice to set everything back to default once configured
        glActiveTexture(GL_TEXTURE0);    
};

void showUniformVarValuePerVertex(GLuint* programeId, Mesh* mesh, bool32 showPos, bool32 showBoneIds, bool32 showWeights, bool32 showFinalBoneMatrices){
//We already bind the specific VAO before
    int VertexSizeforVec4 = MAX_BONE_INFLUENCE * 100;
    int VertexSizeforVec3 = 3 * 100;
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    if(showPos){
        glm::vec3 readbackPositions;        
            for(int k = 0; k < 100; k++){
                GLintptr offset = (k*sizeof(struct Vertex));
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::vec3), &readbackPositions);
                printf("Position ID %d:", k);
                std::cout<<glm::to_string(readbackPositions);
                printf("\n");
            }
    }

    if(showBoneIds){
        int readbackBoneIDs[4];
        //readbackBoneIDs = new int (VertexSizeforVec4);
        if(readbackBoneIDs!=nullptr){
            for(int k = 0; k < 100; k++){
                GLintptr offset = (k*sizeof(Vertex)) + offsetof(struct Vertex, m_BoneIDs);
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(int)*MAX_BONE_INFLUENCE, readbackBoneIDs);
                for(int i = 0; i < MAX_BONE_INFLUENCE; i++){

                    if(i==0)
                        printf("Bone IDs per vertex %d is: ", k);
                    i==0?((readbackBoneIDs[i]<30)&&(readbackBoneIDs[i]>=-1))?printf("%d", readbackBoneIDs[i]):printf("NA"):printf(", %d", readbackBoneIDs[i]);
                }
                printf("\n");
            }
        }
    }

    if(showWeights){
        float readbackWeights[4];
        //readbackWeights = new float (VertexSizeforVec4);
        if(readbackWeights != nullptr){
            for(int k = 0; k < 100; k++){
                GLintptr offset = sizeof(Vertex)*k + offsetof(struct Vertex, m_Weights);
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float)*MAX_BONE_INFLUENCE, readbackWeights);
                    if(readbackWeights[0]!=0.0f){
                    for(int i = 0; i < MAX_BONE_INFLUENCE; i++){
                        if(i==0)
                        printf("Bone weights per vertex %d: ", k);
                        i==0?((i==0.0f||i==1.0f)?printf("%f", readbackWeights[i]):printf("NA")):printf(", %f", readbackWeights[i]);
                        }
                    printf("\n");
                } else {
                    continue;   
                }                
            }
        };

    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);    
    if(showFinalBoneMatrices){
        GLfloat matVal[16];
        char index[1];
        std::string matrixName_;
        for(int k = 0; k < 100; k++){
            matrixName_.clear();
            matrixName_ = "finalBoneMatrices[]";
            sprintf(index, "%d", k);
            matrixName_.insert(matrixName_.size()-1, 1, index[0]);

            glGetUniformfv(*programeId, glGetUniformLocation(*programeId,                                       matrixName_.c_str()), matVal);

            printf("\nBone Matrix index %d is:", k);

            for(int i = 0 ; i < 16; i++){
                if(i==0||i==4||i==8||i==12){
                    i==0?printf("["):printf(", [");
                }
                (i==0||i==4||i==8||i==12)?printf("%f", matVal[i]):printf(", %f", matVal[i]);
                if(i==3||i==7||i==11||i==15){
                    printf("]");
                }
            };
            printf("\n");            
        }
    };
};
