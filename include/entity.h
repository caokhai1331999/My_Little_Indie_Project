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

typedef bool motion_state;
typedef bool motion_type;
typedef int direction_;
typedef int motion;

// Must be clockwise

// This one is righthanded coor sys
/*
enum {
FORWARD,
LEFT,
BACKWARD,
RIGHT,
UP,
DOWN,
NONE
};
*/

// This one is lefthanded coor sys
enum {
BACKWARD,
LEFT,
FORWARD,
RIGHT,
UP,
DOWN,
NONE
};

#define complex (motion_type)1
#define basic (motion_type)0

#define moving (motion_state)1
#define IDLE (motion_type)0

// \ is + in macro language
#define MOVING_(x)\
    moving+x

#define JUMPING_FORWARD (motion)5//(complex_motion)
#define JUMPING_BACKWARD (motion)6//(complex_motion)
#define FALLING         (motion)7//(complex_motion)
// how can gravity affect object

struct object_motion_state_group{
    motion basic_move;
    motion fancy_move;
};

struct motion_spec{
    float base_veclocity;
    float acceleration;
    float veclocity;

    float base_jump_v;
    float jump_v;
    float jump_a;

    float falling_v;
    
    object_motion_state_group current_states;
    object_motion_state_group previous_states;
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
    direction_ current_face;
    direction_ previous_face;
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
