#if !defined(PSUEDO_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

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

    int Health_Bar;
    steerer driver;
};

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
//======================LIGHT_PART==========================
//
//============================================================
//
//======================MESH_PART==========================
// So this can replace class function member effectively
// inside redering_platform.h/or graphic_api.h
typedef mesh_name uint8

enum primitive_shape{
    //TRIANGLE = (mesh_name)0,
    PLANE = (mesh_name)0,
    POLYGON = (mesh_name)1,
    CUBE_SHAPE = (mesh_name)2
};

enum face_type_rhs{
    UP = 0,
    DOWN = 1,
    FRONT = 2,
    BACK = 3,
    RIGHT = 4,
    LEFT = 5
};
//set primitives for mass drawing here
struct vertex{
    glm::vec3 pos;
};

struct triangle{
    face_type_rhs face;
    vertex vertices[3];
    float size;
};
//plane is polygon(for the sake of fully information)
struct plane{
    face_type_rhs face;
    float size;
    vertex vertices[6];
    index indices[6];
    // Should put it in here
    unsigned int texture;
};

struct cube{
    vertex vertices[24];    
    index indices[24];
    float size;
    //unsigned int texture;
};

struct mesh_shape_data_pointers{
    float* vertices_data;
    unsigned int* indices;
};

//======================MESH_PART==========================
//
//===========================================================
// PURPOSE: Create a mechanism that draw multiple object of scene using
//instancing method and available asset.
// In terms of graphics
// enviromental elements is just light

struct Mesh{
    //Use this whenever I done the dynamic array
    mesh_shape_data_pointers data_pointer;
    mesh_name name;
    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;
};
// How can I make sure that all the shaders draw the same object
struct texture_group{
// This one will be hack for 2D game performance
    unsigned int normal_map;
    unsigned int emission_map;
    unsigned int diffused_map;  
    unsigned int specular_map;  
};
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


// GROUP_EVERYTHING_UP
//
//

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
    //std::vector<texture_group*> matched_texture_collection;
    std::vector<Mesh>Mesh_Group;
    std::vector<vertices_data_structure>* Vertices_Data;    
public:
    update_(clock_set* clock);
    B_shader_program* get_shader(return shader);
    // model path is optional
    object(char *media_folder_path = nullptr, char *light_shader_name_ = nullptr)
        : name{name} {
//??
        std::string shader_name = light_shader_name_;
        //search inside the folder for matched source for shader
        basic_light_shader = new B_shader_program(shader_name+".vs", shader_name+".fs", shader_path);       
    }
};
// ====================== Map constructing ===================================

typedef uint8 entity_type
// We then bind single texture/simple model
// to specific object id
#define static_object 0
#define moving_object 1

typedef graphic_attribute_type uint8
#define ()_light_source map_attribute_type(0)/
        
#define light_shone_entity map_attribute_type(1)
#define light_source map_attribute_type(0)
// The passable unit have to be aligned with each other
struct unit_specs{
    int8 MeshID;
    bool32 passable;
    // consider removing this one.
    glm::vec3 position;    
    rigid_body* body;
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

#define PSUEDO_H
#endif
