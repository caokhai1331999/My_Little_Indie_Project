/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "C_Mesh.h"

void setupMesh(Mesh* mesh){
        glGenVertexArrays(1, &mesh->VAO);
        glGenBuffers(1, &mesh->VBO);
        glGenBuffers(1, &mesh->EBO);

        glBindVertexArray(mesh->VAO);
        // Load data into vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
        glBufferData(GL_ARRAY_BUFFER, mesh->verticles.size()*sizeof(Vertex), &mesh->verticles[0], GL_STATIC_DRAW);
        // Good thing about struct is that their memory is sequential for all its time
        // The effect is that we symply pass a pointer to the struct and it traslate into a glm::vec3
        // again translate to 3/2 float which translate into a byte array 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size()*sizeof(unsigned int), &mesh->indices[0], GL_STATIC_DRAW);        

        // Time to set vertex attribute pointers
        // POSITION
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // NORMAL
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // TEXCOORDS
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // TANGENT
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        // BITANGENT
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        // IDS
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // WEIGHTs
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));

        glBindVertexArray(0);    
};

void Draw(Mesh* mesh, GLuint* progID){
        // bind appropriate textures
        unsigned int diffuseNr  = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr   = 1;
        unsigned int heightNr   = 1;
        for(unsigned int i = 0; i < mesh->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)
            string number;
            string name = mesh->textures[i].type;
            if(name == "texture_diffuse")
                number = to_string(diffuseNr++);
            else if(name == "texture_specular")
                number = to_string(specularNr++); // transfer unsigned int to string
            else if(name == "texture_normal")
                number = to_string(normalNr++); // transfer unsigned int to string
            else if(name == "texture_height")
                number = to_string(heightNr++); // transfer unsigned int to string

            // now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(*progID, (name + number).c_str()), i);
            // and finally bind the texture
            glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
        }
        // Draw the mesh(Bind array, Load model, draw element/array)
        useProgram(*progID);
        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
        // Always a good practice to set everything back to default once configured
        glActiveTexture(GL_TEXTURE0);    
};
