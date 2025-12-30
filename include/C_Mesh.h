#if !defined(C_MESH_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#define C_MESH_H

#include <vector>
/*
#if !defined
#define GLAD_LIB_AND_SHADER
#include <glad/glad.h>
#include <glad/glad.c>must be place
#endif
*/

#include "handmade.h"

#if !defined
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

//#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "camera.h"
#include "B_shader.h"


using namespace std; 
#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

// Data in bone weight[]
struct Bone_Info{
    // ID
    unsigned int vertexID;

    /*offset matrix which transform vertex from model space to bone space*/
    glm::mat4 offsetMatrix;
};

struct Texture{
    unsigned int id;
    string type;
    string path;
};

struct Mesh{
    unsigned int VBO, EBO;
    vector<Vertex> verticles;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;
    // MESH constructor
    Mesh( vector<Vertex> verticles,
          vector<unsigned int> indices,
          vector<Texture> textures):verticles(verticles), indices(indices), textures(textures)
          {
          }    
};

void setupMesh(Mesh* mesh = nullptr);

#endif
