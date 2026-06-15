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
    glm::ivec4 m_BoneIDs;
    //GLint m_BoneIDs [MAX_BONE_INFLUENCE];
	//weights from each bone
    glm::vec4 m_Weights;
    //GLfloat m_Weights [MAX_BONE_INFLUENCE];
    //Vertex(){
        //for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
        //{
            //m_BoneIDs[i] = -1;
            //m_Weights[i] = 0.0f;       
        //}
    //}
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
void Draw(Mesh* mesh = nullptr, GLuint* progID = nullptr);

/*
if(glIsTexture((GLint)dancing_vampire->meshes[0].textures[0].id)){
                                      printf("Texture %d is created before and can be used", dancing_vampire->meshes[0].textures[0].id);
                                          }else{
                                      printf("Texture %d isn't created before or something cause it's not valid\n", dancing_vampire->meshes[0].textures[0].id);
};
                                  if(glIsTexture((GLint)dancing_vampire->meshes[0].textures[1].id)){
                                      printf("Texture %d is created before and can be used\n", dancing_vampire->meshes[0].textures[1].id);
                                          }else{
                                      printf("Texture %d isn't created before or something cause it's not valid\n", dancing_vampire->meshes[0].textures[1].id);

                          if (showMsPF) {
                          //printf("[LastFrameCount:%f,EndFrameCount:%f, "
                                 //"CounterPerFrame : %I64d], MiliS per frame: "
                                 //"%f, real FPS: %I64d \n",
                                 //(real32)LastCounter.QuadPart,
                                 //(real32)EndCounter.QuadPart, CountsPerFrame,
                                 //MsPerFrame, FPS);

                              bool isfinite = true;
                              glm::mat4& inverse_Trans = glm::inverse(danceAnimation->getRootNode()->transformation);

                              for(int i = 0; i < 4; i++)
                              {
                                  if(glm::all(glm::isfinite(inverse_Trans[i]))){
                                      continue;  
                                  }else{
                                      isfinite=false;
                                      break;
                                  }
                              }
                              
                              isfinite?printf("Global Inverse Transform matrix is finite"):printf("Global Inverse Transform matrix is not finite");
                              printf(" : %s\n", glm::to_string(inverse_Trans).c_str());
//
                          printf("Delay Ratio: %f, msPerframe: %f, SPerFrame: %f\n",
                                 DelayedRatio, MsPerFrame, SPerFrame);
                          printf("WaitTimeCounter: %f, Axis changing counter: %f\n", WaitTimeCounter, ChangeAxisCounter);
                          printf("ColorOffset is:%f\n", ColorOffset);

                          printf("updated angle :%f\n", UpdatedAngle);
                          std::cout<<"Center Cube Matrix is: "<<glm::to_string(basic_cube_core)<<std::endl;
                          std::cout<<"Current rotating axis is: "<<glm::to_string(randomRotateAxis)<<std::endl;

                          GLint loc = glGetAttribLocation(animating_shader_->GetProgramID(), "TexCoordd");
                          printf("TexCoord location: %d\n", loc);
//char Buffers[256];
                          //sprintf(Buffers,
                          //"[LastFrameCount: %f,EndFrameCount:%f, CounterPerFrame : %I64d], MiliS per frame: %I64d, real FPS: %I64d \n",(real32)LastCounter.QuadPart,(real32)EndCounter.QuadPart, CountsPerFrame,MsPerFrame, FPS);
                          //OutputDebugStringA(Buffers);

                          showMsPF = false;
                      };

*/

#endif


