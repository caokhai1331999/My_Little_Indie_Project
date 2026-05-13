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

class global_light{
    Material material;
    light_in_general light;
    DirLight dirLight;
    PointLight pointLights [NR_POINT_LIGHTS];
};

class Object_Mesh_Specs{
    glm::vec3 Position;
    Mesh mesh;
    Material material;
}

std::vector<>;

// Finds the material of water
void SetGlobalLight(std::vector<B_shader_program*>* shader_list, global_Light* light, std::vector<Object_Mesh_Specs*>*Object_List){
    for(B_shader_program* &shader const: shader_list){
        for(Object_Mesh_Specs* &object const: Object_List){
            shader->propramID = object->programID;
            //shader->set()....
        }
    };
}
// TODO: How to apply material property to shader for it to draw.
//===========================================================================
