/* ========================================================================
   $file: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "psuedo.h"

std::vector<Entities*>*World_Entities_Trackers;
struct Entities_Structure{
    std::vector<rigid_body*>all_entities;
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
    // actually skill is just some set of animation that deal specific damage;
    skills skill;
    int stregth;

    int Heath_Bar;
//
    steerer driver;
};

// Handshake cross check for collision between entities and entities with background

// TileMap contain array of positions in xz plane
// wherein x is analogous to y and z is similar to x in 2D coordinates
//

//===========================================================================
//NOTE: THIS BLOCK IS IN THE ATTEMPT OF CREATING ROOM/WORLD LIGHT
// We light up billboard/flat card using the environment/global light set up
// So We apply one big light source we call sun. And everyobjects have to apply
// its feature and change its look based on its position or intensity.

// Light
// May be loop over all of shader to apply this evironment 
   /*
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
*/
//========================PROFILER================================
// we got clock, we need to show them on opengl
// Casey store all pixel's data of one frame in large buffer and once pass them
// all to and it still worked.
//
//========================PROFILER================================
// TODO: How to apply material property to shader for drawing.
//===========================================================================

// Build world including group of mesh chunks.

// update :
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

// chunk of meshes
//======================LIGHT_PART==========================

struct basic_light_specs{
    glm::vec3 ambient;
    glm::vec3 specular;
    glm::vec3 diffuse;
};

struct dir_light_specs{
    general_light basic_specs;
    glm::vec3 direction;
};

struct attenuation_specs{
    // For attenuation (Point Light)
    float constant;
    float linearTerm;
    float quadraticTerm;
};

struct spot_light_specs{
    // For spotlight effect
    // spotlight area defining angle(Phi) maybe with the different name such as cutoff
    float CutOff;
    // Now the smooth/soft edge effect
    float OuterCutOff;
};

struct general_light{
    // Basic specs
    basic_light_specs basic_specs;
    glm::vec3 direction;
    // For attenuation (Point Light)
    attenuation_specs attenuation;
    // For spotlight effect
    // spotlight area defining angle(Phi) maybe with the different name such as cutoff
    spot_light_specs spot_specs;
};
//============================================================
// light based on normal map
struct environment_map{
    bitmap* LOD[4];
}
//
struct Enviromental_Element{
    // But how can this affect the output texture of the shader.
    environment_map env_map[3];
    std::vector<general_light*>*light_group;    
};
//============================================================
// Think about set this light group shrewly
//
void set_light(glm::vec3* position, std::vector<general_light*>* light_group){
    light_group->basic_specs =;
    light_group->direction =;
    light_group->attenuation =;
    light_group->spot_specs =;
}

// Manually set light here.
void turn_on_light(std::vector<general_light*>* light_group){
    // so we have to manually set it here
    // Is there anyway to automatic this one;
}
//======================LIGHT_PART==========================


//======================MESH_PART==========================
// So this can replace class function member effectively
// inside redering_platform.h/or graphic_api.h
static struct basic_shape_vertices_data{
    // triangle;
// This is for 2D game/effects
    static const float PlaneVertices[] = {
        // positions
        // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
        // x,    y,     z
         1.0f, -1.0f, 1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 
        -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f,  1.0f, 1.0f, 0.0f, 1.0f,                   
         1.0f,  1.0f, 1.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 1.0f, 1.0f, 0.0f
      };

    static const unsigned int planeIndices[] =  {
//Even though the vertex 1, 0 will be reused but we have to feed them name for opengl just like this
                    0, 1, 2, 2, 4, 0
     };
    
    // polygon
    float polygon[] = {};
    int polygon_indices[] = {};

    //cube
    float cube[] = {
     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
     -0.5f,  0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
                         
     -0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
     -0.5f, -0.5f,  0.5f,                         
                         
     -0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f, -0.5f,
     -0.5f, -0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
                         
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
                         
     -0.5f, -0.5f, -0.5f,
      0.5f, -0.5f, -0.5f,
      0.5f, -0.5f,  0.5f,
      0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f,  0.5f,
     -0.5f, -0.5f, -0.5f,
                         
     -0.5f,  0.5f, -0.5f,
      0.5f,  0.5f, -0.5f,
      0.5f,  0.5f,  0.5f,
      0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f,  0.5f,
     -0.5f,  0.5f, -0.5f
    };

    int cube_indices[] = {
        0, 1, 2, 2, 4, 0,//0
        6, 7, 8, 7, 6, 11,
        12, 13, 14, 13, 12, 17,//2
        18, 19, 20, 20, 22, 18,
        24, 25, 26, 25, 24, 29,//4
        30, 31, 32, 32, 34, 30
    };

    //tile-liked shape
    float tile[] = {// vertex                    TextCoords
      //BACK FACE
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
       // LEFT FACE                       
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
    int tile_indices[] = {
       0, 1, 2, 2, 4, 0,//0
       6, 7, 8, 7, 6, 11,
       12, 13, 14, 13, 12, 17,//2
       18, 19, 20, 20, 22, 18,
       24, 25, 26, 25, 24, 29,//4
       30, 31, 32, 32, 34, 30
    };
}vertices_collection;

// Consider using imposter to replace actual sphere
// Group of mesh mean group of asset and vertice data(from simple like triangle to complex like cube, cylinder or the whole model vertex)

struct Mesh{
    //Use this whenever I done the dynamic array
    Vertex primitive_data;//cube, plane
    unsigned int VAO;
}

//===========================================================
// PURPOSE: Create a mechanism that draw multiple object of scene using
//instancing method and available asset.
// In terms of graphics
// enviromental elements is just light

// How can I make sure that all the shaders draw the same object
struct texture_group{
// This one will be hack for 2D game performance
    std::string name;
    unsigned int normal_map;
    unsigned int emission_map;
    unsigned int diffused_map;  
    unsigned int specular_map;  
};
//
// How we do layer of effect on the same object
// How to first prototype the graphic property
class graphic_property{
private:
    // This will be geometry collection
    B_shader_program* basic_light_shader;
    // This is for drawing effect like fog, etc...
    B_shader_program* post_effect_shader;
    //unsigned int* Texture_Group; //dynamic array case
    std::vector<texture_group*> matched_texture_collection;
    std::vector<Mesh*>Mesh_Group;
public:
//  Mesh* mesh;
    update_(clock_set* clock);
    B_shader_program* get_shader(return shader);
    // model path is optional
    object(char *media_folder_path = nullptr, char *light_shader_name_ = nullptr)
        : name{name} {
//??
        std::string shader_name = light_shader_name_;
        //search inside the folder for matched source for shader
        basic_light_shader = new B_shader_program(shader_name+".vs", shader_name+".fs", shader_path);       
        // load file? or set them up manually???
        // draw a map
        mesh = new Mesh();
        setupMesh(mesh);
        // model if possible
    }
};

// We can replay game by thanks to this struct
struct game_state{
    void* BitmapMemory;
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    int BitmapMemorySize;
// One pit of performance to use this one
    // be careful
    std::vector<C_Model*>Model_Collection;    
};

void set_mesh_data(const char* data_path);

// ====================== Map constructing ===================================
typedef uint8 entity_type
// We then bind single texture/simple model
// to specific object id
#define static_object 0
#define moving_object 1

// The passable unit have to be aligned with each other
struct unit_specs{
    bool32 passable;
    glm::vec3 position;    
    rigid_body* body;
    int8 MeshID;
};

struct simple_volume_map{
    // one is mesh type, the other is the position;
    int8* map_content;// replace this with array of map_unit_specs
    std::vector<unit_specs*>map_content;
    size_t map_size;
    
    // Volumme/Room 3D size in world space
    uint16 height;
    uint16 breadth;
    uint16 length;
};

// We spawn/randomize new map everytime we change room
// Think about this is the volume/room not the mere flat ground
//
// How to relatively define map size based one world
// first we have to know how big is the unit cube size to compare to the world
// current ground size is 1(x0.375)x30(x0.5)x30(x0.5 )
//
//So I decide the volume will be 50x50x50
//
#define ROOM_HEIGHT (uint8)50
#define ROOM_BREADTH (uint8)50
#define ROOM_LENGTH (uint8)50
// ZII(Zero Initializtion) is good
void init_volume_map(simple_map* map, std::vector<Mesh*>*Mesh_Group){
    map->height = ROOM_HEIGHT;
    map->breadth = ROOM_BREADTH;
    map->length = ROOM_LENGTH;
    map->map_size = (size_t)(2 * map->height * map->breadth * map->length)+1;

#if TEST_DYNAMICALLY_ALLOCATION
    map->map_content = (unsigned int*)VirtualAlloc(map->map_content, map_size);
#else
    map->map_content.reserve(map->map_size);
}

// We need a pre-created Texture group
void sketch_map(simple_map* map, Mesh* mesh_group){
    srand(time(NULL));
    // rational map sketcher here.
    // 1. first thing first we need to decide where can the character where is not
    int x = 0;
    int y = 0;
    int h = 0;
    //for(int y = 0; y < map->height; y++){
        //for(int x = 0; x < map->breadth; x++){
            //for(int z = 0; z < map->length; z++)
    uint8 Block_Object_Count = (uint8)((float)map->map_size * 0.2f);
    for(size_t int i = 0; i < map->size; i++)
    {
                //// we do every single cube here which have the size of 1,1,1
                //
                // have to be more specificly rational about this one
                // ground first then up
                // what to store OMG we just need to store the mesh ID
        // need to be more rational, can not let it randome like this
        x++;

        if(x == map->w -1 ){
            x -= map->w - 1;
            z++;
        };

        if(z == map->l-1)
            h++;

        // All right. First simple approach is we make the majority of room
        // passable then sprinkle few block objects on the way.
/*
#if TEST_DYNAMICALLY_ALLOCATION
        {
        *map->map_content++ = rand()%(mesh_group->size()-1);
        if(block_object_count > 0)
        *map->map_content++ = rand()%1;
        if(*map->map_content == 0)
            block_object_count--;
        }
#else
*/
        //{
         map->map_content[i] = rand()%(mesh_group->size()-1);
        if(block_object_count > 0)
         map->map_content[i] = rand()%1;
        if(*map->map_content == 0)
            block_object_count--;         
        //}

    }
 //}        
    //};
}

// ====================== Map constructing ===========================

//===============LOOP_RUNNING_THEM=====================
// In Big inititalization : Init OpenGL
//                          Turn On Light
//                          Init Object: mesh, rigid body
//

void init_graphic (Win32_OffScreen_Buffer* BackBuffer, std::vector<object*>*object_group, std::vector<general_light*> light_group){
    // set light
    // where is the proper place for this light group: backbuffer or object group
    InitOpenGl(BackBuffer);
    // Light here
    // we need to set this one for every shader we have.
    turn_on_light(light_group);
    set_light_for_shader();
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

void object::set_rigid_body(glm::vec3* init_pos){
    Init_Entity_Specs(body);
}

void graphic_property::set_mesh_data(const char* data_path = nullptr, char* vertices data){
    // data can be loaded from text file!!;
    setupMesh(this->mesh);
    // For skinning
    SetVertexBoneData(Vertex* vertex, int boneID, float weight){
        ;
    };
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

void update(std::vector<rigid_body*>* entities_group,     std::vector<map_unit_specs*>map_content, clock_set* clock){
// check for collision
    move_object(main_character);
    for(map_unit_specs* const &obj: map_content){
        obj->rigid_body->update(clock);
// or
        // This is O(n²) problems
// TODO: Make this one check out the collision on map

        if(!obj->passable)
            check_collision(obj->rigid_body, main_character);

        move_object(clock, obj->rigid_body);
    };
}

// =====================================================
void render_in_group (Graphic_Properties* Graphic, simple_volume_map* world_map, Camera* chosen_camera){    
    // we can use instance
    // first feed shader with mesh data (Store with VAO)
    // Then update the relative position of the obj(with world, or just mere screen space with text)
    // Then use each brush(shader) in brush_set to draw object
    Graphic->shader[i]->setMat4("projection", chosen_camera->projection); 
    Graphic->shader[i]->setMat4("view", chosen_camera->view);
    size_t i = 0;
    //So with the catatonic object we just pass its position as an unchanged
    //layout data/ InstanceID
    while(i < world_map->size){
// Each shader represent for one layer of effect at least.
        if(!world_map->map_content[i]){
            //By modding for that dimension we always have a number in its range;
            // I think I done interpret the index to the entities postion in world
            // space
            glm::vec3 position = {(float)i%world_map->w, (i>(world_map->w*world_map->l))?(float)(i/(world_map->w*world_map->h)):0.0f, i>world_map->w?(i/world_map->w)%world_map->l:0.0f};
            // now we decide how to add matched id in Graphic object;
            // replace i with some thing
            glBindVertexArray(Graphics->mesh_group[map->map_content[i]].VAO);
// Also decide this one
            Graphic->shader[i]->use();
            // Postion may be we use i * w * l * h
            Graphic->shader[i]->setVec3("Postion", position);
            Draw(obj->graphic_->mesh, obj->graphic_->shader[i]);
        }
    i+=2;
    };
    // Then draw post effect here.
    glUseProgram(0);
}
//===================================================================
// In group:
// How to manage these vertex's data efficiently
// when ever we load small mesh
// we can sample out the function
// Init: Set environment light, entity's pos, load texture, mesh: indices, vertices
//(pos, texcoord, normal, tangent, bitangent, boneid[4], weight[4])
// how do we RENDER: bind VAO -> use shader -> set uniform(camera's pos, entity's
//pos, light) -> draw element
// Think about this in group
// these are in one struct call game_state
 //
// These kind objects will reside inside something call window_game_state.
// render(&BackBuffer.Game_State)

//  ========================= Rigid Body =============================
class object{
private:
    std::string name;
    rigid_body* body;
public:
    void set_rigid_body();
    void move();
}
