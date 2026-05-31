/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "entity.h"

void rigid_body::move(float delta_t){
    CalcNewPos(delta_t, this);
}

void move_object(float delta_t, rigid_body* object){
    object->move(delta_t);
}

void CalcNewPos(float delta_time, rigid_body* object){
    float value;
    switch(object->object_speed.motion_states.basic_move){
        case MOVING_FORWARD:
            object->position[3][2] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][2], delta_time);            
            break;
        case MOVING_BACKWARD:
            object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.acceleration, -object->object_speed.veclocity, object->position[3][2], delta_time);
            break;
        case MOVING_RIGHT:
            object->position[3][0] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][0], delta_time);            
            break;
        case MOVING_LEFT:
            object->position[3][0] = based_a_v_Pos_calc(-object->object_speed.acceleration, -object->object_speed.veclocity, object->position[3][0], delta_time);
            break;
        //default:
            //printf("still don't what to put into this defaut case\n");
            //break;
    };

    if(object->position[3][1] < -4.5f){
        if(object->object_speed.motion_states.fancy_move == JUMPING)
            object->object_speed.motion_states.fancy_move = FALLING;
    }
    
    switch(object->object_speed.motion_states.fancy_move){
        case JUMPING:
            Jump(delta_time, object);
            ApplyGravity(delta_time, object);
            break;

        case FALLING:
            ApplyMomentum(delta_time, object);
            ApplyGravity(delta_time, object);
            break;            

        default :
            printf("Gravity is being applied\n");
            ApplyGravity(delta_time, object);
            break;
    }

    if(object->position[3][1] >= 0.0f && object->object_speed.motion_states.fancy_move == FALLING)
        object->object_speed.motion_states.fancy_move = IDLE;

    printf("object current position is %s\n", glm::to_string(object->position).c_str());
    //for (float& axis const: *vector){
        //if(axis != 0,0f)
            //object_pos[3][i] = based_a_v_Pos_calc(object.acceleration, object.veclocity, object_pos[3][i], delta_time);
        //i++;
    //};
}

void ApplyGravity(float delta_t, rigid_body* object){
    if(object->position[3][1] < 0)
        object->position[3][1] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][1], delta_t);
}

void ApplyMomentum(float delta_t, rigid_body* object){
    object->position[3][2] = based_a_v_Pos_calc(0.0f, -object->object_speed.jump_v, object->position[3][2], delta_t);
};

void Jump(float delta_t, rigid_body* object){
          object->position[3][1] = based_a_v_Pos_calc(-object->object_speed.jump_a, -object->object_speed.jump_v, object->position[3][1], delta_t);
          object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.jump_a, -object->object_speed.jump_v, object->position[3][2], delta_t);
}
