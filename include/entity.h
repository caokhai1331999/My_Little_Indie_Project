#if !defined(ENTITY_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "handmade.h"
#include "physics.h"

typedef int basic_motion;
typedef int complex_motion;

#define IDLE            (basic_motion)0//(basic_motion)
#define MOVING_FORWARD  (basic_motion)1//(basic_motion)
#define MOVING_BACKWARD (basic_motion)2//(basic_motion)
#define MOVING_RIGHT    (basic_motion)3//(basic_motion)
#define MOVING_LEFT     (basic_motion)4//(basic_motion)
#define JUMPING_FORWARD (complex_motion)5//(complex_motion)
#define JUMPING_BACKWARD (complex_motion)6//(complex_motion)
#define FALLING         (complex_motion)7//(complex_motion)
// how can gravity affect object

struct object_motion_state_group{
    basic_motion basic_move;
    complex_motion fancy_move;
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
    glm::mat4 position;
    float mass;
    motion_spec object_speed;
    void move(float delta_t);
};

float based_a_v_Pos_calc(float a, float v, float p, float delta_t){
    float a_ = a - (0.5*v);
    return ((a_/2)*std::pow(delta_t,2))+(v*delta_t)+p;
}

global_variable rigid_body test_vampire_motion = {};

void CalcNewPos(float FrameTime = 0, rigid_body* object = nullptr);
void CalcNewV(float FrameTime = 0,  float* veclo = nullptr, float* accel = nullptr);
void ApplyGravity(float delta_t, rigid_body* object);

void CalcMomentumSpeed(float FrameTime = 0,  rigid_body* object = nullptr);
void ApplyMomentum(float delta_t, rigid_body* object);
// Need to have fomular for this.
void Jump(float delta_t, rigid_body* object);

extern "C" __declspec(dllexport) void move_object(float delta_t, rigid_body* object);
typedef void (*move_object_)(float, rigid_body*);

#define ENTITY_H
#endif
