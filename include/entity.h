#if !defined(ENTITY_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "Tile.h"
#include "physics.h"
// Must be clockwise

// This one is righthanded coor sys
/*
enum FACE_DIRECTION {
FORWARD,
LEFT,
BACKWARD,
RIGHT,
UP,
DOWN,
NONE
};
*/
typedef uint8 direction;
// This one is lefthanded coor sys
enum FACE_DIRECTION {
    BACKWARD = (direction)0,
    LEFT = (direction)1,
    FORWARD = (direction)2,
    RIGHT = (direction)3,
    UP = (direction)4,
    DOWN = (direction)5,
    NONE = (direction)6
};

typedef uint8 moving_type;
typedef moving_type basic_moving_type;
typedef moving_type complex_moving_type;


//-----------------For_Debugging-------------------------

struct File_Manager{
    WIN32_FIND_DATA find_data;
    HANDLE handle;
};

struct texture_group{
// This one will be hack for 2D game performance
    const char* name;

    unsigned int normal_map;
    unsigned int diffused_map;  
    unsigned int specular_map;
    unsigned int emission_map;
};

local_persist texture_group load_textures_in_folder(File_Manager* folder_looker = nullptr, const char* folder_path_ = nullptr);
local_persist void Load_Textures_for_OpenGL(std::vector<texture_group>* texture_collection, const char* media_folder_path);
extern "C" __declspec(dllexport) void Load_Textures_for_OpenGL_(Platform_Properties* Game_Platform, std::vector<texture_group>* texture_collection, const char* media_folder_path = nullptr);
typedef void (*Load_Textures_for_OpenGL__) (Platform_Properties*, std::vector<texture_group>* , const char*);
//-----------------For_Debugging-------------------------;


enum BASIC_MOVE{
    WALKING = basic_moving_type(1)
};

enum COMPLEX_MOVE{
    JUMPING = complex_moving_type(2),
    //FALLING = complex_moving_type(10)
};

#define DIRECTION_(x) \
    x ## _BACKWARD, \
    x ## _LEFT, \
    x ## _FORWARD, \
    x ## _RIGHT, \
    x ## _UP, \
    x ## _DOWN

enum MOVING_STATE {
    IDLE = basic_moving_type(0),
    DIRECTION_(WALK),
    DIRECTION_(JUMP),
    FALLING
};

// \ is + in macro language
// structure auto define the value of ordered member
    //(complex_motion)
// how can gravity affect object
struct motion_spec{
    float base_veclocity;
    float acceleration;
    float veclocity;

    float base_jump_v;
    float jump_v;
    float jump_a;

    float falling_v;
    
    MOVING_STATE current_states;
    MOVING_STATE previous_states;
};

//void apply_gravity(float mass);

class rigid_body{
//private:
    //space_box box_;
// For rigid body
public:
    float mass;
    glm::mat4 position;
    glm::vec3 pre_pos;
    FACE_DIRECTION current_face;
    FACE_DIRECTION previous_face;
    motion_spec object_speed;
    space_box box_;
    void move(float delta_t);
};

static float based_a_v_Pos_calc(float a, float v, float p, float delta_t){
    float a_ = a - (0.5*v);
    return ((a_/2)*std::pow(delta_t,2))+(v*delta_t)+p;
};

global_variable rigid_body test_vampire_motion = {};

void Init_Entity_Specs(rigid_body* entity = nullptr){
    entity->position = glm::mat4(1.0f);
    entity->position = glm::scale(entity->position,glm::vec3( 0.01f));
    entity->position = glm::translate(entity->position, glm::vec3(-2.0f, 0.0f, 0.0f));

    // update position
    entity->object_speed.base_veclocity = 3.0f;
    entity->object_speed.acceleration = 2.0f;
    entity->object_speed.veclocity = 3.0f;
    
    entity->object_speed.base_jump_v = 6.0f;
    entity->object_speed.jump_a = 14.0f;
    entity->object_speed.jump_v = 6.0f;
    
    entity->object_speed.falling_v = 0.0f;
    
    entity->current_face = FORWARD;

};

static void CalcNewPos(float FrameTime = 0, rigid_body* object = nullptr);
static void OrientFace(rigid_body* object = nullptr);
static void Orient_Around_Y(glm::mat4* matrix = nullptr, float angle = 0);
static void CalcNewV(float FrameTime = 0,  float* veclo = nullptr, float* accel = nullptr);
static void ApplyGravity(float delta_t, rigid_body* object);
static void ApplyMomentum(float delta_t, rigid_body* object);
static void CalcMomentumSpeed(float FrameTime = 0,  rigid_body* object = nullptr);
static void Jump(float delta_t, rigid_body* object);
// Need to have fomular for this.

extern "C" __declspec(dllexport) void move_object(float delta_t, rigid_body* object);
typedef void (*move_object_)(float, rigid_body*);

#define ENTITY_H
#endif
