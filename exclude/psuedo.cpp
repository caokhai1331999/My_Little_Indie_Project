/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "psuedo.h"

bool32 check_collision( space_box* box1 = nullptr, space_box* box2 = nullptr){
    //AABB BBAA
    //assume that the position point is in the center of the box
    bool32 collided = false;

    glm::vec3 spaceforBox1;
    //ON X-axis
    if (((box1->position[3][0] < box2->position[3][0])&&(box1->position[3][0] + box1->size.x > box2->position[3][0])) || ((box2->position[3][0] < box1->position[3][0])&&(box2->position[3][0] + box2->size.x > box1->position[3][0]))){
        //implement here
        spaceforBox1.x = box1->position[3][0] - box2->position[3][0];
        !collided?collided = true:collided=false;     
    }
    //ON y-axis
    else if (((box1->position[3][1] < box2->position[3][1])&&(box1->position[3][1] + box1->size.y > box2->position[3][1])) || ((box2->position[3][1] < box1->position[3][1])&&(box2->position[3][1] + box2->size.y > box1->position[3][1]))){
        spaceforBox1.y = box1->position[3][1] - box1->position[3][1];
        !collided?collided = true:collided=false;        
    }
    //ON z-axis
    else if (((box1->position[3][2] < box2->position[3][2])&&(box1->position[3][2] + box1->size.z > box2->position[3][2])) || ((box2->position[3][2] < box1->position[3][2])&&(box2->position[3][2] + box2->size.z > box1->position[3][2]))){
        spaceforBox1.z = box1->position[3][2] - box1->position[3][2];
        !collided?collided = true:collided=false;        
    }

    box2->collide_list->push_back(collided_space(spaceforBox1));

    glm::vec3 spaceforBox2 = glm::vec3(-1 * spaceforBox1.x, -1 * spaceforBox1.y, -1 * spaceforBox1.z);
    box1->collide_list->push_back(collided_space(spaceforBox2));

    return collided;
};

int main(){
    std::string Mname = "terrain";                
    Model_* land = nullptr;
    land = new Model_(false, &Mname);
    std::string terrain_path = "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_Project/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.fbx";
    loadModel_(land, terrain_path);

    
};

//What if we do this on the complex model this ain't work so well cause this is just a rough collision checking method;
glm::mat4 CamCoor_core = glm::mat4(1.0f);
CamCoor_core = glm::scale(CamCoor_core, glm::vec3(0.01, 0.01, 0.01 ));


Model_* CamCoor_ = nullptr;
loadModel_(CamCoor_, "");
model_shader_->setMat4(CamCoor_, "");
// Game Physics
void CalculateGravity(float* weight){
    ;
}

// Game Logics
struct physics{
    float weight = 0.0f;
    float FallingSpeed = 0.0f;
};
// What we do is just affect the model matrix in world space.
//================================

class level_feature{
    ;
};

class entity{
private:
    space_box collided_box;
    // This matrix will contain anything about
    //the entity in the made-up world like:
    //position, rotation, scale
    glm::mat4 world_present_model;
    
    bool32 is_moving;
    bool32 is_dead;

    int current_level;
    int storage;
    skills skill;
    int stregth;

    int Heath_Bar;
//
    controller driver;
};

// Handshake cross check for collision between entities and entities with background

// TileMap contain array of positions in xz plane
// wherein x is analogous to y and z is similar to x in 2D coordinates
//


void drawTile(const unsigned int VaoID = 0, const B_shader_program* Brush = nullptr, const Tile_Map* Map);


bool* load_bin_map(std::string* name){
    
    std::ifstream bin_map {*name, s.binary | s.trunc | s.in};

    s.read(reinterpret_cast<char*>(&d), sizeof d);
    int n;
    //std::string str;
    bool* map_content;
    if(bin_map){
        while((int)s >> map_content){
            printf("loading\n");
        }
        printf("Succeed loading map content\n");
    }else{
        printf("Failed loading map content\n");
    } 
    return map_content;
}

#define TILE_LENGTH 1.0f;
#define TILE_WIDTH 1.0f;
#define TILE_HEIGHT 0.1f;

// In order to save performance we just draw tile that is in viewing space
std::vector<Tile*>ConstructTileMap(bool* map, int width, int length, int height){

    std::vector<Tile*>map;
    map->resserve(width * length * height);
    glm::vec3 Tile_Pos;
// Try to randomize thing here
// Should I store another map here
     //for(int h = 0; h < height; h+=TILE_HEIGHT){
        for(int l = 0; l < length; l+=TILE_LENGTH){
            for(int w = 0; w < width; w+=TILE_WIDTH){

                if((*map)[w][l][h]=="1"){
// Random TextureID + texture type 
// 
                    Tile_Pos.x = l;
                    Tile_Pos.y = h;
                    Tile_Pos.z = w;

                    map->push_back(new Tile());
                }
            }                
        //};
    }
;}

std::vector<unsigned int>* RandomizeTileMap(int Map_Width, int Map_Lenght, unsigned int* textureIDs){
    std::vector<unsigned int>Map;
    std::srand(std::time(NULL));
    for(int w = 0; w < Map_Width; w++){
        for(int l = 0; l < Map_Lenght; l++){
            Map.push_back(textureIDs[std::rand()%(sizeof(*textureIDs) - 1)]);
        }
    }
    return &Map;
}

void drawTile(const unsigned int VaoID = 0, const B_shader_program* Brush = nullptr, const std::vector<Tile>* Map){

    glBindVertexArray(VaoID);
    Brush->use();
    glm::mat4 tile_container;

    for(*Tile const &tile: Map->map){
        // If in range
        tile_container = glm::mat4(1.0f);
        tile_container = glm::translate(tile_container, tile->position);

        Brush->setInt("material.diffused1", tile->TextureID);
        Brush->setMat4("model", tile_container);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };
};

class Tile_Map{
    std::vector<*Tile>map;
}
// General Tiles ID array.

unsigned int SetupTileTexture(const char* path){

    int width, height, nrComponents;
    unsigned char *data = (unsigned char*)stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    
// NOTE: Focus on this
    unsigned int textureID;

    if(data){
        glGenTexture(textureID);
        glActiveTexture(GL_TEXTURE0+textureID);

        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        
        glPixelStorei(GL_UNPACK_ROW_LENGTH, width);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    
        //Wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        //Filter
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        GLenum err = glGetError();

        if (err != GL_NO_ERROR) {
            printf("OpenGL Error after glTexImage2D: %x\n", err);
        }

    }

    glBindTexture(GL_TEXTURE_2D, 0);    

    //if(OBuffer->glData.textureHandle!=NULL){
    //printf("Texture name is: %d\n", OBuffer->glData.textureHandle);
    //} else {
    //printf("Some How texture is NULL???\n");
    //}
    
}

         glm::vec2 xy = glm::normalize(glm::vec2(camera->mouse.xPos, camera->mouse.yPos));
         glm::vec2 xy_ = glm::normalize(glm::vec2(camera->mouse.LastX, camera->mouse.LastY));
         float radAngle = (glm::acos(glm::dot(xy_, xy))) * camera->speed * SENSITIVITY;
         glm::fquat offsetOrientation = {};

         offsetOrientation.w = glm::cos(radAngle/2.0f);
         offsetOrientation.x = camera->Direction.x * glm::sin(radAngle/2.0f);
         offsetOrientation.y = camera->Direction.y * glm::sin(radAngle/2.0f);
         offsetOrientation.z = camera->Direction.z * glm::sin(radAngle/2.0f);

         camera->orientation = camera->orientation * offsetOrientation;

    //if (fDraw) {
      //DeviceContext = GetDC(Window);
      //MoveToEx(DeviceContext, ptPrevious.x, ptPrevious.y, NULL);
      //LineTo(DeviceContext, ptPrevious.x = LOWORD(Lparam),
             //ptPrevious.y = HIWORD(Lparam));
      //ReleaseDC(Window, DeviceContext);
    //}
//
    //
    // if(BackBuffer.camera.mouse.xPos > BackBuffer.BitmapWidth){
    // BackBuffer.camera.mouse.xPos = BackBuffer.BitmapWidth;
    //}
    //
    // if(BackBuffer.camera.mouse.xPos < 0){
    // BackBuffer.camera.mouse.xPos = 0;
    //}
    //
    //
    // if(BackBuffer.camera.mouse.yPos > BackBuffer.BitmapHeight){
    // BackBuffer.camera.mouse.yPos = BackBuffer.BitmapHeight;
    //}
    //
    // if(BackBuffer.camera.mouse.yPos < 0){
    // BackBuffer.camera.mouse.yPos = 0;
    //}
    //
    // printf("Mouse x pos: %d\n", BackBuffer.camera.mouse.xPos);
    // printf("Mouse y pos: %d\n", BackBuffer.camera.mouse.yPos);
         //
//
//
// feed vshader with the camera position, and sprite position.
// .vs :
// point pos = camera

// Now what we have in memory.
// what we need to store in permanent storage
// We have one win32Offscreen_Buffer, one frontbuffer, one game state
//
//
//===========================================================================
//NOTE: THIS BLOCK IS IN THE ATTEMPT OF CREATING DYNAMIC LIGHT

// We light up billboard/flat card using the environment/global light set up
// So We apply one big light source we call sun. And everyobjects have to apply
// its feature and change its look based on its position or intensity.

// Light
// May be loop over all of shader to apply this evironment 

struct light_in_general{
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
};

struct Material{
// Ambient will be the same even with change in input
   sampler2D texture_diffused1;
   sampler2D texture_specular1;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};

struct DirLight{
 // Inherent component
 vec3 direction;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
};

struct PointLight{
 // Inherent component
 vec3 position;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;
};

//uniform PointLight pointlight;

struct SpotLight{
 // Inherent component
 vec3 direction;
 vec3 position;

 // For Phong Shading
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;

 // For spotlight effect
 // spotlight area defining angle(Phi) maybe with the different name such as cutoff
 float CutOff;
 // Now the smooth/soft edge effect
 float OuterCutOff;
};

#define NR_POINT_LIGHTS 2

    //Material material;
class global_light{
    light_in_general light;
    DirLight dirLight;
    PointLight pointLights [NR_POINT_LIGHTS];
};

class Object_Mesh_Specs{
    glm::vec3 Position;
    Mesh mesh;
    Material material;
}

unsigned int LoadCubeMap(const char* path){
    unsigned int cubemap;
    glGenTextures(1, &cubemap);
    // Create and assign the state for cube map texture here
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    // Then bind each image data for each texture faces
    unsigned char* data;
    unsigned int width, height, nrchannel;
    for(unsigned int i = 0; i < textures_faces.size(); i++){
        data = stbi_load(textures_faces[i].c_str(), &width, &height, &nrchannel, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_X + i, 0, nrchannel, width, height, 0, nrchannel, GL_UNSIGNED_BYTE, data);
    }
    return cubemap;
}

// TODO: How to apply material property to shader for it to draw.
//===========================================================================

// Build world including group of mesh chunks.
//Compute normal map to make surface looked less fake
// How to arrange it
global_variable Tile* World = nullptr;

// Land vertices mesh
float land_vertices[] = 
{// vertex                    TextCoords
//BACK   , {0,0},
  {-0.125f, -0.125f, -0.125f}, {0.0f, -0.25f},
  { 0.125f, -0.125f, -0.125f}, {0.0f, -0.25f},
  { 0.125f,  0.125f, -0.125f}, {0.0f, -0.25f},
  { 0.125f,  0.125f, -0.125f}, {0.0f, -0.25f},
  {-0.125f,  0.125f, -0.125f}, {0.0f, -0.25f},
  {-0.125f, -0.125f, -0.125f}, {0.0f, -0.25f},
//FRONT  , {0,0},
  {-0.125f, -0.125f,  0.125f}, {0.0f, 0.25f},
  { 0.125f,  0.125f,  0.125f}, {0.0f, 0.25f},
  { 0.125f, -0.125f,  0.125f}, {0.0f, 0.25f},
  { 0.125f,  0.125f,  0.125f}, {0.0f, 0.25f},
  {-0.125f, -0.125f,  0.125f}, {0.0f, 0.25f},
  {-0.125f,  0.125f,  0.125f}, {0.0f, 0.25f},
//LEFT  , {0,0},
  {-0.125f,  0.125f,  0.125f}, {0.0f, 0.0f},
  {-0.125f, -0.125f, -0.125f}, {0.0f, 0.0f},
  {-0.125f,  0.125f, -0.125f}, {0.0f, 0.0f},
  {-0.125f, -0.125f, -0.125f}, {0.0f, 0.0f},
  {-0.125f,  0.125f,  0.125f}, {0.0f, 0.0f},
  {-0.125f, -0.125f,  0.125f}, {0.0f, 0.0f},
//RIGHT   , {0,0},
  {0.125f,  0.125f,  0.125f}, {0.0f,  0.0f},
  {0.125f,  0.125f, -0.125f}, {0.0f,  0.0f},
  {0.125f, -0.125f, -0.125f}, {0.0f,  0.0f},
  {0.125f, -0.125f, -0.125f}, {0.0f,  0.0f},
  {0.125f, -0.125f,  0.125f}, {0.0f,  0.0f},
  {0.125f,  0.125f,  0.125f}, {0.0f,  0.0f},
//bottom , {0,0},
  {-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f},
  { 0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f},
  { 0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f},
  { 0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f},
  {-0.5f, -0.5f, -0.5f}, {-1.0f,  0.0f},
  {-0.5f, -0.5f,  0.5f}, {-1.0f,  0.0f},
//TOP
  {-0.5f,  0.5f, -0.5f}, {1.0f,  0.0f},
  { 0.5f,  0.5f, -0.5f}, {1.0f,  0.0f},
  { 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f},
  { 0.5f,  0.5f,  0.5f}, {1.0f,  0.0f},
  {-0.5f,  0.5f,  0.5f}, {1.0f,  0.0f},
  {-0.5f,  0.5f, -0.5f}, {1.0f,  0.0f}
}


// Normal Map Lighting
// Position
//Top Left Corner
glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
// Bottom Left Corner
glm::vec3 pos2(-1.0f,-1.0f, 0.0f);
// Bottom Right Corner
glm::vec3 pos3( 1.0f,-1.0f, 0.0f);
// Top Right Corner
glm::vec3 pos4( 1.0f, 1.0f, 0.0f);

// Texture Coordinate
// Due to the origin/ We start mapping texture is/at the Bottom of the quad
// so we have 
glm::vec2 uv1(0.0, 1.0);
glm::vec2 uv2(0.0, 0.0);
glm::vec2 uv3(1.0, 0.0);
glm::vec2 uv4(1.0, 1.0);

glm::vec3 normal(0.0f, 0.0f, 1.0f);
// Manually calculate Tangent and Bitangent if
// the there are none available in model/mesh

// In Vertex Shader

out vs{
    vec2 TextCoord;
    vec3 FragPos;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
}vs_out;

void main(){
    vec3 T = normalize(vec3(model * vec4(tangent, 1.0f)));
}

void main(){
    // We will ignore the w value;
    vec3 bitangent = cross(anormal, tangent);

    vec3 T = normalize(vec3(model * vec4(tangent, 0.0f)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0f)));
    vec3 N = normalize(vec3(model * vec4(anormal, 0.0f)));

    // inverse of orthogonal matrix is its transpos form
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.TextCoord = aTextCoord;
    vs_out.FragPos = TBN * (vec3(model * vec4(aPos, 1.0f)));
    vs_out.tangentViewPos = TBN * normalize(view);
    vs_out.tangentLightPos[i] = TBN * lightPos;

    for(int i = 0; i < TOTAL_POINT_LIGHTS){
        vs_out.tangentLightPos[i] = TBN * POINT_LIGHT_POS[i];
    }
};

// motion in
void set_environment_force_(entity* object){
    object->position * envir.gravity_on_pos;
}


void Jump(motion_spec* object, float delta_time){
          object->position[3][1] = based_a_v_Pos_calc(object->jump_a, object->jump_v, object->position[3][2], delta_time);
          object->position[3][2] = based_a_v_Pos_calc(object->acceleration, object->veclocity, object->position[3][2], delta_time);          
}

              

// GLSL

uniform sampler2D texture;

in vec2 TexCoord;

uniform struct light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

void computLight_intensity_based_on_D(){
    // attenuation;
    k * d....
}

out vec4 fragColor;
 void main(){
     vec3 OutFrag = texture(sampler2D, TexCoord);
     OutFrag *= Light.abient * attenuation;
     fragColor = vec4(OutFrag, 1.0f);
 };
// We may set texture coordinate as inherent data type at the shader creating stage
//

#if !defined
#define STB_TRUE_IMPLEMENTATION 1;
#include "stb_truetype.h"
#endif

void init_font(char* path, char* , Font* font){
   fread(glyphs_map.ttf_buffer, 1, 1<<20, fopen(path, "rb"));
   // Load font data
   stbtt_BakeFontBitmap(glyphs_map.ttf_buffer,0, 32.0, glyphs_map.bitmap,512,512, 32,96, cdata);
   // no guarantee this fits!
   // can free ttf_buffer at this point
   glGenTextures(1, &font->font_texture);
   glBindTexture(GL_TEXTURE_2D, font->font_texture);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512,512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, glyphs_map.bitmap);
   // can free temp_bitmap at this point
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

// Display Font Buffer.
// Use bitmap font loading on 2D squad
// load text to font display
// Or time to render in group
//
//
// x, y, w, h
// We loop through the character of char* array. Then render each of it
void CalcGlyphProperties_(Glyph_Map* map, int index){
    ;
}

void RenderStringOnScreen(Glyph_Map* map, const char* string, B_shader_program* shader){
    char glyph_ = string[i];
    glm::vec2 glyph_Pos_Offsset = CalcGlyphProperties_(map, i);
    
    map->bitmap = stbtt_GetCodepointBitmap(&map->FontInfo, 0, stbtt_ScaleForPixelHeight(&map->FontInfo, 128.0f), glyph_, &map->w, &map->h, &map->Xoffset, &map->Yoffset);
    
    // Render glyp using opengl api
    glActiveTexture(GL_TEXTURE0+map->TextureID);
    glBindTexture(GL_TEXTURE_2D, map->TextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, map->w, map->h, 0, GL_RG, GL_UNSIGNED_BYTE, map->bitmap);

    // set offset pos
    shader->SetVec2("offset["+to_string(i)+"]".c_str(), &glyph_Pos_Offsset[0]);
};

// We need to define the area's position, size, ...
// Then calculate the glyph specs based on these.
//
Rect_ rect = {0, 0, 400, 100}

glm::vec2 void CalcGlypProperty(const glm::vec2* previous_glyp_specs, const Rect_* rect){
    glm::vec2 font_specs;
    if(*previous_glyp_p.x + w < rect->w){
        font_specs.x = *previous_glyp_specs.x + *previous_glyp_specs.w;
    }else{
        font_specs.x = 0;
        font_specs.y = *previous_glyp_specs.y + *previous_glyp_specs.h;
    }
    return font_specs;
}

// Init: OpenGL, entities's specs
// Update : position, animation
// Render : Vertex, Texture, model,
// Vertex and Texture date belong to Mesh which is bound to ID of entities which contain updated states(Position, level, health)

class RandomScene {
    // Terrain
    // Moving Object
    int ID;
    
}

void (const aiNode* Node = nullptr, int total_meshes_number = 0){

    if(node->mNumMeshes > 0){
        total_meshes_number += (int)node->mNumMeshes;
    }
    
    if(node->mNumChildren > 0){
        for(unsigned int i = 0; i < node->mNumChildren; i++){
            TotalMeshesCounter(node->mChildren[i], total_meshes_number);
        }
    }
}
// update : ;
// render : loop through entitys and used matched shader to draw;
//;

// Mesh
// coimponent:
//    . still objects: + rock 
//                     + tree
//    . moving entities : + river
//                        + animals
//    . weather elements : + wind
//                         + snow
//                         + mist
      //  components's properties: . collided volume
      //                           . moving information: T, S, R.
      //                           . primitive for drawing:(mesh)
      //                                       .. position, texture
//
//===> Render: feed shader : . fixed primitve data |  . VAO
      //                     . uniform offset data |  . shader
      //                     . camera pos for light|  . -//-

struct Game_Specs{
    int id;
}

// chunk of meshes
//
// MEMORY
#include <Memoryapi.h>
struct memory_region{
    uint8* base;
    size_t current_size;
    size_t used;
};
// ==> next version is memory_block

struct memory_block{
    memory_block* prev;
    memory_block* next;

    size_t current_size;
    uint64 Pad[6];
    uint8* base;
};

// apply to grow vertex array

void init_mem_region(size_t size, memory_region* arena){
    arena->current_size = size;
    arena->base = (uint8*)VirtualAlloc(array->current_size);
}

void reallocate_mem_region(size_t size, memory_region* arena){
        //reallocate this memory region
    if(area->size < size + arena->used)
        VirtualAlloc()
}

void free_mem_region(memory_region* arena){
    if(arena->base);
    VirtualFree(arena->base, arena->current_size);
}

// Do I understand how #define keyword work
// so actually the type and arena is indicating the variable

void* push_size_(size_t size, memory_region* primal){
void* push_size_(size_t size, memory_block* arena){
    if(arena->used + size >= arena->current_size){
        memory_block* new_block ;
// This one is not thread-safe
        // casey lock it inside the something call tick mutex, to prevent any one/app else use these kind of thread while it's on working.
        // so currently, we haven't touch this growing aray yet.
        // focus on draw scene and load gl pointer on little beast.
        new_block->base = VirtualAlloc(primal->based, (uint32)megabyte(10) + arena->size + size);

        new_block->prev = primal->prev;
        new_block->next = primal;


        new_block->prev->next = block;
        new_block->next->prev = block;

        new_block->size +=  (uint32)megabyte(10) + arena->size;
    };

    arena->used += size;
    void* result = arena->base + arena->used;

    return result;
}

// set memory here
void* set_memory(memory_region* dest, size_t size, void* source){
    void* mem_current_point = dest->base + used;

    if(used > size)
    *mem_current_point = *source;
    //CopyMemory();
}

// ===========================================================================

#define push_size(type, arena) (type* )push_size_(sizeof(type), arena)
#define push_array(type, count, arena) (type* )push_size_(count * sizeof(type), arena)

struct Mesh{
    //
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;
}
//===========================================================

struct world{
    std::vector<object*>*object_group;
    std::vector<object*>*camera_set;
    std::vector<general_light*>lights_group;
};

// PURPOSE: Create a mechanism that draw multiple object of scene using instancing method and available asset.
//

struct general_light{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 specular;
    glm::vec3 diffuse;
}

class Enviromental_Element{
    std::general_light;
}

class object{
private:
    std::string name;
    B_shader_program* program;
    Mesh* mesh;
    rigid_body* body;
public:
    update_(clock_set* clock);
    B_shader_program* get_shader(return program);
    // model path is optional
    object(const char* name_ = nullptr, char* shader_path = nullptr, char* model_path = nullptr):name{name}{
        std::string shader_name = name_;
        program = new B_shader_program(shader_name+"vs", shader_name+"fs", shader_path);

        
        // load file? or set them up manually???
        //
        // draw a map
        mesh = new Mesh();
        setupMesh(mesh);
        // model if possible
    }
    void set_mesh_data(const char* data_path);
    void set_rigid_body();
}

void init (std::vector<object*>*object_group){
    // set light

/*
    . Ambient Light - Kind of constance to store it
    . Point Light -
                   |
                   | take camera position as input
                   |
    . Spot Light  -
*/
    // set mesh;
    
    // set rigid body
        . Init Position
}

void object::set_rigid_body(glm::vec3* init_pos){
    Init_Entity_Specs(body);
}

void set_mesh_data(const char* data_path = nullptr, char* vertices data){
    // data can be loaded from text file!!;
    setupMesh(this->mesh);
    SetVertexBoneData(Vertex* vertex, int boneID, float weight){
        ;
    };

class environment_light{
    // basic ambient light
    // point light
    // spot light
}

void update(std::vector<object*>* objects_group, input, clock_set* clock){
// check for collision
    for(object* const &obj: objects_group){
        obj->update(clock);
// or
        move_object(clock, obj->rigid_body);
    };
}

void render (std::vector<object*>*objects_group, Camera* chosen_camera){    

    //for(int i = 0; i < (int)objects_group->size()-1; i++){
    for(objects* const &obj: *objects_group){
        obj->shader->use();
        obj->shader->setMat4("projection", chosen_camera[i]->projection); 
        obj->shader->setMat4("view", chosen_camera[i]->view); 
        
        Draw(obj->mesh, obj->shader);
    };
    glUseProgram(0);
}
