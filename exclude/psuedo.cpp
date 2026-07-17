/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "psuedo.h"

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

   glm::vec3 ambient;
   glm::vec3 diffuse;
   glm::vec3 specular;

   float shininess;
};

struct DirLight{
 // Inherent component
 vec3 direction;

 // For Phong Shading
 glm::vec3 ambient;
 glm::vec3 diffuse;
 glm::vec3 specular;
};

struct PointLight{
 // Inherent component
 vec3 position;

 // For Phong Shading
 vec3 specular;

 // For attenuation (Point Light)
 float constant;
 float linearTerm;
 float quadraticTerm;
};

//uniform PointLight pointlight;

struct SpotLight{
 // Inherent component
 glm::vec3 direction;
 glm::vec3 position;
 
 // For Phong Shading
 glm::vec3 ambient;
 glm::vec3 diffuse;
 glm::vec3 specular;

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
//======================MEMORY_PART==========================
// This is a derivative work from Casey shown down for my dedicated senior
//struct memory_region{
    //uint8* base;
    //size_t current_size;
    //size_t used;
//};
// ==> next version is memory_block

#define minimun(a, b) return (a > b)?b:a

struct memory_block{
    memory_block* prev;
    memory_block* next;

    size_t size;
    size_t used;
    void* base;
    // In term of linear data arrangement the Pad itself is to just separate the memory_block memory address from what come after it.
    uint64 Pad[6];
};

// first we have to create ticket that is related to the threadID in cheap way.
// 
struct ticket_mutex{
    // keep in mind that the volatile is type that can be delared as an object and modified by hardware
    volatile uint64 ticket;
    // serving is current intercepting thread which id is taken from getthreadid.
    volatile uint64 serving;
    // the ticket loop is just waiting until the thread left/retire before the other get in to execute that line of code again.
}

void AtomicAddUint32(uint32* addend, uint32 value){
// use this to create threadId based ticket and loop through them.
    // until it retire in order.
    // Cause this one very fast(cpu level). --> it ensure that no 2 threads can have the same ticket numbers
    // This one is just the order that a thread hit this line, all of these satisfy the M.E.S.I protocol
    InterlockedExchangeAdd((long*)addend, value);
}

void* ALLOCATE_BLOCK_MEMORY(memory_block* mem, size_t size){

    if(mem){
        memory_block* block = (memory_block*)VirtualAlloc(0, size + sizeof(memory_block), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
        // why plus one
        void* result = block + 1;

        block->next = mem->next; 
        block->prev = mem; 

        block->next->prev = block;
        block->prev->next = block;

        return result
    };
}

void DEALLOCATE_BLOCK_MEMORY(memory_block* mem){
    if(mem){
        memory_block* block = ((memory_block*)mem - 1);
        block->prev->next = block->next;
        block->next->prev = block->prev;

        VirtualFree(block, MEM_COMMIT|MEM_RESERVE);
    };
}

void begin_ticket_mutex(ticket_mutex* mutex){
    uint64 ticket = AtomicAddUint64(&mutex->ticket, 1);
    // mutex->ticket is now auto change
    // But why when the ticket equal to ticket thread id that we know it get out.
    // 
    while(ticket != mutex->serving)
}

void end_ticket_mutex(ticket_mutex* mutex){
    AtomicAddUint64(&mutex->serving, 1);
    // Whenever the ticket equal to the threadId that mean the thread get out of code lines and bring instruction to the core
}
// apply to grow vertex array

void init_mem_region(size_t size, memory_region* arena){
    arena->current_size = size;
    arena->base = (uint8*)VirtualAlloc(array->current_size);
}

void free_mem_region(memory_region* arena){
    if(arena->base);
    VirtualFree(arena->base, arena->current_size);
}

// Do I understand how #define keyword work
// so actually the type and arena is indicating the variable
#define DEFAULT_BLOCK_SIZE GIGABYTE(1)

void* push_size_(size_t size, memory_region* sentinel, ticket_mutex* mutex){
    void* result;
    // whenever the total requested size if bigger than the current block size: allocate new space and copymemory of the old block
    if(sentinel->used + size >= sentinel->size){
        begin_ticket_mutex(mutex);
        memory_block* new_block = (memory_block*)ALLOCATE_BLOCK_MEMORY(sentinal);
        // casey lock it inside the something call tick mutex, to prevent any one/app else use these kind of thread while it's on working.
// This one is not thread-safe
        // so currently, we haven't touch this growing aray yet.
        // focus on draw scene and load gl pointer on little beast.
        end_ticket_mutex(mutex);

        result = new_block->based + size;
        new_block->used += size;

        CopyMemory();
    } else {
        sentinel->used += size;
        result = sentinel->based + sentinel->used;
    };
    return result;
}

// set memory here
// replace by copy_memory of window.


#define push_size(type, arena) (type* )push_size_(sizeof(type), arena)
#define push_array(type, count, arena) (type* )push_size_(count * sizeof(type), arena)

//======================MEMORY_PART=========================

//======================LIGHT_PART==========================

struct general_light{
    glm::vec3 ambient;
    glm::vec3 specular;
    glm::vec3 diffuse;
};

struct dir_light{
    general_light specs;
    glm::vec3 direction;
};

struct point_light{
    general_light specs;
    // For attenuation (Point Light)
    float constant;
    float linearTerm;
    float quadraticTerm;
};

struct spot_light{
    general_light specs;
    
    // Inherent component
    vec3 direction;
    vec3 position;

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

    //
struct Enviromental_Element{
    std::vector<general_light*>light_group;    
};

// Think about set this light group shrewly
void set_light(glm::vec3* position){
    ;
}

// Manually set light here.
void turn_on_light(std::vector<general_light*>* light_group){
    for(general_light* const &light: light_group){;
        set_light
    };
}
//======================MESH_PART==========================

struct Mesh{
    //
    Vertex* vertices;
    unsigned int* indices;
    Texture* textures;
}
//===========================================================

// PURPOSE: Create a mechanism that draw multiple object of scene using instancing method and available asset.
// In terms of graphics
// enviromental elements is just light

// add auto-guide
// ALL_IN_ONE    
class graphic_property{
private:
    B_shader_program* shader;
    Mesh* mesh;
public:
    update_(clock_set* clock);
    B_shader_program* get_shader(return shader);
    // model path is optional
    object(const char* name_ = nullptr, char* shader_path = nullptr, char* model_path = nullptr):name{name}{
        std::string shader_name = name_;
        program = new B_shader_program(shader_name+"vs", shader_name+"fs", shader_path);
        
        // load file? or set them up manually???
        // draw a map
        mesh = new Mesh();
        setupMesh(mesh);
        // model if possible
    }
    void set_mesh_data(const char* data_path);
};

//===============LOOP_RUNNING_THEM=====================
// In Big init : Init OpenGL
//               Turn On Light
//               Init Object: mesh, rigid body
//

void init_graphic (Win32_OffScreen_Buffer* BackBuffer, std::vector<object*>*object_group, std::vector<general_light*> light_group){
    // set light
    // where is the proper place for this light group: backbuffer or object group
    InitOpenGl(BackBuffer);
    turn_on_light(light_group);
    // consider loading textures group separatedly here.
/*
    . Ambient Light - Kind of constance to store it
    . Point Light -
                   |
                   | take camera position as input
                   |
    . Spot Light  -
*/
    // set mesh;
    set_mesh_data(path);
    // rand map
    // set rigid body
    //. Init Position
    glm::vec3 pos = glm::vec3(0.0f);
    // Time to init a 2D map here for every single entity in the current volumm
}

void Init(Win32_OffScreen_Buffer* BackBuffer){
    init_graphic(BackBuffer);
    // Init enity/rigid body specs here
    char* map_content = load_bin_map()// or random map
    for(object* const &obj : BackBuffer->object_group){
        object->set_rigid_body(&pos);
        pos.x += 1.0f;
        if(pos.x > 10.0f)
            pos.z += 1.0f;
    }
}
void object::set_rigid_body(glm::vec3* init_pos){
    Init_Entity_Specs(body);
}

void graphic_property::set_mesh_data(const char* data_path = nullptr, char* vertices data){
    // data can be loaded from text file!!;
    setupMesh(this->mesh);
    SetVertexBoneData(Vertex* vertex, int boneID, float weight){
        ;
    };
}

void update(std::vector<object*>* objects_group, input, clock_set* clock){
// check for collision
    for(object* const &obj: objects_group){
        obj->rigid_body->update(clock);
// or
        // This is O(n²) problems
        check_collision(obj->rigid_body);
        move_object(clock, obj->rigid_body);
    };
}
//
// These kind objects will reside inside something call window_game_state.
// render(&BackBuffer.Game_State)
//
void render (std::vector<object*>*objects_group, Camera* chosen_camera){    
    //for(int i = 0; i < (int)objects_group->size()-1; i++){
    for(objects* const &obj: *objects_group){
        obj->graphic_->shader->use();
        obj->graphic_->shader->setMat4("projection", chosen_camera[i]->projection); 
        obj->graphic_->shader->setMat4("view", chosen_camera[i]->view); 
        
        Draw(obj->graphic_->mesh, obj->graphic_->shader);
    };
    glUseProgram(0);
}
//===================================================================

class object{
private:
    std::string name;
    graphic_property graphic_;
    rigid_body* body;
public:
    void set_rigid_body();
    void move();
}
// In group:
// How to manage these vertex's data efficiently
// when ever we load small mesh
// Init: Set environment light, entity's pos, load texture, mesh: indices, vertices
//(pos, texcoord, normal, tangent, bitangent, boneid[4], weight[4])
// how do we RENDER: bind VAO -> use shader -> set uniform(camera's pos, entity's pos, light) -> draw element
// Think about this in group
