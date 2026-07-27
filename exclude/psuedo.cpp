/* ========================================================================
   $file: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "psuedo.h"

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
;
//===========================================================================
//NOTE: THIS BLOCK IS IN THE ATTEMPT OF CREATING DYNAMIC LIGHT

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
// Casey store all pixel's data of one frame in large buffer and pass them all to
// GL context
//
//========================PROFILER================================
// TODO: How to apply material property to shader for drawing.
//===========================================================================

// Build world including group of mesh chunks.
//Compute normal map to make surface looked less fake
// How to arrange it
global_variable Tile* World = nullptr;

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
// inside redering_platform.h
struct basic_shape_vertices_pos{
    // triangle;
    unsigned int triangle[] ={};
    unsigned int triangle_indices[] ={};
    // polygon
    unsigned int polygon[] ={};
    unsigned int polygon_indices[] ={};
    //cylinder
    unsigned int cube[] ={};
    unsigned int cube_indices[] ={};
    //
};
// Consider using imposter to replace actual sphere
// Group of mesh mean group of asset and vertice data(from simple like triangle to complex like cube, cylinder or the whole model vertex)
struct Mesh{
    //Use this whenever I done the dynamic array
    Vertex primitive_data;//cube, plane
    unsigned int textures[3];
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
    unsigned int normal_map;
    unsigned int emission_map;
    unsigned int diffused_map;  
    unsigned int specular_map;  
};
//
// How we do layer of effect on the same object

class graphic_property{
private:
    // This will be geometry collection
    B_shader_program* basic_light_shader;
    B_shader_program* post_effect_shader;

    //unsigned int* Texture_Group; //dynamic array case
    std::vector<unsigned int*> texture_Group;
    std::vector<Mesh*>Mesh_Group;
public:
//    Mesh* mesh;
    update_(clock_set* clock);
    B_shader_program* get_shader(return shader);
    // model path is optional
    object(char *folder_path = nullptr)
        : name{name} {
//??
        //search inside the folder for matched source for shader
        program = new B_shader_program(shader_name+".vs", shader_name+".fs", shader_path);
        
        // load file? or set them up manually???
        // draw a map
        mesh = new Mesh();
        setupMesh(mesh);
        // model if possible
    }
};

struct game_state{
    void* BitmapMemory;
    int BitmapWidth;
    int BitmapHeight;
    int Pitch;
    int BitmapMemorySize;

    std::vector<C_Model*>Model_Collection;    
};

void set_mesh_data(const char* data_path);

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

char* randomize_entitiespos(){
    char[width * height] map_content;
    int random_value;
    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            random_value = std::rand()%4 + 1;
            *map_content++  = random_value;
        }
    };
    return map_content;
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

// =====================================================
void render_in_group (Graphic_Properties* Graphic, uint8* world_map, Camera* chosen_camera){    
    //for(int i = 0; i < (int)objects_group->size()-1; i++){
    // we can use instance
    // first feed shader with mesh data (Store with VAO)
    // Then update the relative position of the obj(with world, or just mere screen space with text)
    // Then use each brush(shader) in brush_set to draw object

    for(objects* const &obj: *objects_group){
// Each shader represent for one layer of effect at least.
        glBindVertexArray(Graphics->VAO_Group);
        Graphic->shader[i]->use();
        Graphic->shader[i]->setMat4("projection", chosen_camera[i]->projection); 
        Graphic->shader[i]->setMat4("view", chosen_camera[i]->view);         
        Draw(obj->graphic_->mesh, obj->graphic_->shader);
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

 // ====================== Map constructing ===================================
typedef uint8 entity_type
// We then bind single texture/simple model
// to specific object id
#define static_object 0
#define moving_object 1

struct simple_volume_map{
    uint8* map_content;
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

void init_volume_map(simple_map* map, ){
    map->height = ROOM_HEIGHT;
    map->breadth = ROOM_BREADTH;
    map->length = ROOM_LENGTH;
    map->map_size = (size_t)(map->height * map->breadth * map->length);
}

// We need a pre-created Texture group
void sketch_map(simple_map* map){
    map->map_content = (char*)VirtualAlloc(map->map_content, map_size);
    for(int y = 0; y < map->height; y++){
        for(int x = 0; x < map->breadth; x++){
            for(int z = 0; z < map->length; z++){
                // we do every single cube here which have the size of 1,1,1
                //
                // have to be more specificly rational about this one
                // ground first then up
                *map->map_content++ = rand()%(last_texture_id - first_texture_id);
            }
        }        
    };
}

void Draw_Volume(simple_map* map, B_shader_program* shader, Camera* chosen_camera){
    for(size_t i = 0; i < map->size; i++){
        shader->setFloat((unsinged int)map->map_content[i]);
        shader
    };
};
// ====================== Map constructing ===========================

class object{
private:
    std::string name;
    rigid_body* body;
public:
    void set_rigid_body();
    void move();
}
