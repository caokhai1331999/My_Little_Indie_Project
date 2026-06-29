#if !defined(TILE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ctime>

#include "C_Model.h"
#include "handmade.h"

#define DOWNN_ 0.0f
#define UPP_ 1.0f
#define ROLL_ 2.0f

using namespace std;

static const float TILE_Z = 1.2f;
static const float TILE_X = 1.2f;
static const int8_t TOTAL_TILE_NUMBER = 50;
float fluxY[200] = {};

// NOTE: 1st assign object postion by translate
//       2nd assign right VAOS for different flat cube shapes
//       3rd assign texture index for shader too
//       finally, draw it out by using glDrawArray
//
// Must create a map for this
//const TileIndices[] = {
    //
//};

struct rollCubeInfo{
    int index_;
    int axisIndex_;
};

// We loop over the tiles array and check for collision every frame
// Or Per vertex, Cause this will be more dynamic.
// So We can use C_Mesh
// So best way scenario is the Tile contain group of vertices data
 //enum 
float land_vertices[] = 
{// vertex                    TextCoords
//BACK   , {0,0},
-0.5f,  0.375f, -0.5f,     0.0f, 0.0f,
 0.5f,  0.375f, -0.5f,     1.0f, 0.0f,
 0.5f,  0.5f,   -0.5f,     1.0f, 1.0f,
 0.5f,  0.5f,   -0.5f,     1.0f, 1.0f,
-0.5f,  0.5f,   -0.5f,     0.0f, 1.0f,
-0.5f,  0.375f, -0.5f,     0.0f, 0.0f,
 //FRONT FACE                         
-0.5f,  0.375f,  0.5f,     0.0f, 0.0f,
 0.5f,  0.5f,    0.5f,     1.0f, 1.0f,
 0.5f,  0.375f,  0.5f,     1.0f, 0.0f,
 0.5f,  0.5f,    0.5f,     1.0f, 1.0f,
-0.5f,  0.375f,  0.5f,     0.0f, 0.0f,
-0.5f,  0.5f,    0.5f,     0.0f, 1.0f,
 // LEFT0.375CE                       
-0.5f,  0.5f,    0.5f,     1.0f, 0.0f,
-0.5f,  0.375f, -0.5f,     0.0f, 1.0f,
-0.5f,  0.5f,   -0.5f,     1.0f, 1.0f,
-0.5f,  0.375f, -0.5f,     0.0f, 1.0f,
-0.5f,  0.5f,    0.5f,     1.0f, 0.0f,
-0.5f,  0.375f,  0.5f,     0.0f, 0.0f,
 // RIGH0.375ACE                      
 0.5f,  0.5f,    0.5f,     1.0f, 0.0f,
 0.5f,  0.5f,   -0.5f,     1.0f, 1.0f,
 0.5f,  0.375f, -0.5f,     0.0f, 1.0f,
 0.5f,  0.375f, -0.5f,     0.0f, 1.0f,
 0.5f,  0.375f,  0.5f,     0.0f, 0.0f,
 0.5f,  0.5f,    0.5f,     1.0f, 0.0f,
 // BOTT0.375FACE                     
-0.5f,  0.375f, -0.5f,     0.0f, 1.0f,
 0.5f,  0.375f,  0.5f,     1.0f, 0.0f,
 0.5f,  0.375f, -0.5f,     1.0f, 1.0f,
 0.5f,  0.375f,  0.5f,     1.0f, 0.0f,
-0.5f,  0.375f, -0.5f,     0.0f, 1.0f,
-0.5f,  0.375f,  0.5f,     0.0f, 0.0f,
//TOP                 ,               
-0.5f,  0.5f,   -0.5f,     0.0f, 1.0f,
 0.5f,  0.5f,   -0.5f,     1.0f, 1.0f,
 0.5f,  0.5f,    0.5f,     1.0f, 0.0f,
 0.5f,  0.5f,    0.5f,     1.0f, 0.0f,
-0.5f,  0.5f,    0.5f,     0.0f, 0.0f,
-0.5f,  0.5f,   -0.5f,     0.0f, 1.0f 
}; 

float g_color_buffer_data[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

int Tile_Land_Indices[] = {
    0, 1, 2, 2, 4, 0,//0
    6, 7, 8, 7, 6, 11,
    12, 13, 14, 13, 12, 17,//2
    18, 19, 20, 20, 22, 18,
    24, 25, 26, 25, 24, 29,//4
    30, 31, 32, 32, 34, 30
};

struct TileGLObject{
    unsigned int TileVAO;
    unsigned int TileEBO;
    unsigned int TileVBO;
    unsigned int TileColorBO;
    unsigned int TextureID;
};

struct Tile{// Or Tile chunk may be

    glm::vec2 position;
    glm::vec3 size;//scale

    TileGLObject TileObj;
    
    unsigned int TextureID = 0;

    // Tile indices
    Tile(){
        if (position!= glm::vec2(0.0f)){
            position = glm::vec2(0.0f);
        }

        if (TextureID!= 0){
            TextureID = 0;
        }

        TileObj = {};
    };
};

void set_tile_vertex(B_shader_program* shader = nullptr, TileGLObject* TileObj = nullptr); 

Tile* LoadTileMap();
void drawTile(unsigned int VaoID = 0, unsigned int shaderID = 0, float speed = 1.0, float* updatedDegree = nullptr, bool32 changeAxis = false, std::vector<rollCubeInfo>* rollCubemap = nullptr);

unsigned int SetupTileTexture(const char* path = nullptr);
void drawTile(const TileGLObject* TileObj = nullptr, B_shader_program* const Brush = nullptr);

#define TILE_H
#endif

