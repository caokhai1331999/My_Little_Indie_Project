#if !defined(GL_BRIDGE_PLATFORM_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "handmade.h"
// move initopengl here
#typedef Mesh_Type uint8

#define Triangle_Mesh (Mesh_Type)0
#define Polygon_Mesh (Mesh_Type)1
#define Plane_Mesh (Mesh_Type)2
#define Cube_Mesh (Mesh_Type)3
#define Tile_Mesh (Mesh_Type)4

#define ROOM_HEIGHT (uint8)50
#define ROOM_BREADTH (uint8)50
#define ROOM_LENGTH (uint8)50

struct texture_group{
// This one will be hack for 2D game performance
    std::string name;
    unsigned int normal_map;
//
    unsigned int emission_map;
    unsigned int diffused_map;  
    unsigned int specular_map;  
};

struct Basic_Mesh{
    // Use this whenever I done the dynamic array
    // For the first time vertex is kind of too much to fillout need just need the
    // Position this time - plus how to phrase the position junk per vetex one
    // Vertex primitive_data;//cube, plane
    Mesh_Type MeshID;
    texture_group mesh_textures;
    // So we store the shape's vertices's data to VAOs array for shader to get it
    // The layout () in is taken from data bound with that VAO
    unsigned int VAO;
};

struct simple_volume_map{
    // one is mesh type, the other is the position;
    int8* map_content;
    size_t map_size;
    
    // Volumme/Room 3D size in world space
    uint16 height;
    uint16 breadth;
    uint16 length;
};

#typedef shader_type int8
#define basic_light (shader_type)0
#define post_effect (shader_type)1

class graphic_property{
private:
    // This will be geometry collection
    B_shader_program* basic_light_shader;
    // This is for drawing effect like fog, etc...
    B_shader_program* post_effect_shader;
    //unsigned int* Texture_Group; //dynamic array case
    std::vector<texture_group*> matched_texture_collection;
    std::vector<Basic_Mesh*>Mesh_Group;
public:
//    Mesh* mesh;
    // we don't need this one
 //update_(clock_set* clock);
    external B_shader_program* get_light_shader(shader_type type){
        if(type = basic_light){
            return basic_light_shader;
        }else if (type = post_effect){
            return post_effect_shader;
        }else{
            return nullptr;
        }
    };
    // model path is optional
    object(char *media_folder_path = nullptr, char *light_shader_name_ = nullptr)
        : name{name} {
//??
        std::string shader_name = light_shader_name_;
        std::string shader_path = assets_folder_path;
        //search inside the folder for matched source for shader
        basic_light_shader = new B_shader_program(shader_name+".vs", shader_name+".fs", shader_path);       
        // load file? or set them up manually???
        // draw a map
        mesh = new Mesh();
        setupMesh(mesh);
        // model if possible
    }
};

void init_volume_map(simple_map* map = nullptr, std::vector<Mesh*>*Mesh_Group = nullptr);
void sketch_map(simple_map* map = nullptr, Mesh* mesh_group = nullptr);
void render_in_group (Graphic_Properties* Graphic = nullptr, simple_volume_map* world_map = nullptr, Camera* chosen_camera = nullptr);


#define GL_BRIDGE_PLATFORM_H
#endif
