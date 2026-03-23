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
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//#include "camera.h"
#include "assimp_glm_helpers.h"
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
    unsigned int id;
    /*offset matrix which transform vertex from model space to bone space*/
    glm::mat4 offset;
};

struct Texture{
    unsigned int id;
    string type;
    string path;
};

struct Mesh{
    unsigned int VBO, EBO;
    unsigned int VAO;
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    // MESH constructor
    Mesh( vector<Vertex> vertices,
          vector<unsigned int> indices,
          vector<Texture> textures):vertices(vertices), indices(indices), textures(textures)
          {
          }    
};

void SetVertexBoneDataToDefault(Vertex* vertex){
    for(int i = 0; i < MAX_BONE_INFLUENCE; i++){
        vertex->m_BoneIDs[i] = -1;
        vertex->m_Weights[i] = 0.0f;
    }
};

void SetVertexBoneData(Vertex* vertex, int boneID, float weight);
void setupMesh(Mesh* mesh = nullptr);
void Draw(Mesh* mesh = nullptr, GLuint* progID = nullptr, bool32 vampire_ = false);
#endif
