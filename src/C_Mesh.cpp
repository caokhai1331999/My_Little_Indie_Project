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
        glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size()*sizeof(Vertex), &mesh->vertices[0], GL_STATIC_DRAW);

        // Time to set vertex attribute pointers
        // POSITION
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)0);

        // NORMAL
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Normal));

        // TEXCOORDS
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

        // TANGENT
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Tangent));

        // BITANGENT
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Bitangent));

        // IDS
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, m_BoneIDs));

        // WEIGHTs
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, m_Weights));

        // The effect is that we symply pass a pointer to the struct and it traslate into a glm::vec3
        // again translate to 3/2 float which translate into a byte array 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);

        
        glBindVertexArray(0);    
};


void Draw(Mesh* mesh, GLuint* progID, bool32 vampire_){
        // bind appropriate textures
    
        unsigned int textureID  = vampire_?3:1;

        unsigned int diffuseNr  = 0;
        unsigned int specularNr = 0;
        unsigned int normalNr   = 0;
        unsigned int heightNr   = 0;

        for(unsigned int i = 0; i < mesh->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + mesh->textures[i].id); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = mesh->textures[i].type;
            if(name == "material.texture_diffused")
                number = to_string(diffuseNr++);
            else if(name == "material.texture_specular")
                number = to_string(specularNr++); // transfer unsigned int to string
            else if(name == "material.texture_normal")
                number = to_string(normalNr++); // transfer unsigned int to string
            else if(name == "material.texture_height")
                number = to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
            glUniform1i(glGetUniformLocation(*progID, (name + number).c_str()), mesh->textures[i].id);
        }

        // Draw the mesh(Bind array, Load model, draw element/array)
        useProgram(*progID);
        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
        // Always a good practice to set everything back to default once configured
        glActiveTexture(GL_TEXTURE0);    
};
