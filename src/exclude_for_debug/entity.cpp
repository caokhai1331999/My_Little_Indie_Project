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

    if(object->object_speed.current_states.basic_move != IDLE && (object->object_speed.previous_states.basic_move == object->object_speed.current_states.basic_move)){
        CalcNewV(delta_t, &object->object_speed.veclocity, &object->object_speed.acceleration);
    }else{
        object->object_speed.veclocity = object->object_speed.base_veclocity;
    }

    object->move(delta_t);
}

void CalcNewPos(float delta_time, rigid_body* object){
    float value;
    //add a little friction(opposed force here)
   //
    switch(object->object_speed.current_states.basic_move){
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

    if(object->position[3][1] < -15.5f){
        if(object->object_speed.current_states.fancy_move == JUMPING_FORWARD || object->object_speed.current_states.fancy_move == JUMPING_BACKWARD){
            object->object_speed.previous_states.fancy_move = object->object_speed.current_states.fancy_move;
            object->object_speed.current_states.fancy_move = FALLING;
        }
    }else{
        CalcNewV(delta_time, &object->object_speed.jump_v, &object->object_speed.jump_a);
    }

    
    switch(object->object_speed.current_states.fancy_move){
        case JUMPING_FORWARD:
            Jump(delta_time, object);
            ApplyGravity(delta_time, object);
            break;

        case JUMPING_BACKWARD:
            Jump(delta_time, object);
            ApplyGravity(delta_time, object);
            break;

        case FALLING:
            ApplyGravity(delta_time, object);
            ApplyMomentum(delta_time, object);
            break;            

        default :
            printf("Gravity is being applied\n");
            ApplyGravity(delta_time, object);
            break;
    }

    if(object->position[3][1] >= 0.0f && object->object_speed.current_states.fancy_move == FALLING){
        object->object_speed.previous_states = object->object_speed.current_states;
        object->object_speed.current_states.fancy_move = IDLE;
        object->object_speed.jump_v = object->object_speed.base_jump_v;
        object->object_speed.falling_v = 0.0f;
    }

    printf("object current position is %s\n", glm::to_string(object->position).c_str());
    //for (float& axis const: *vector){
        //if(axis != 0,0f)
            //object_pos[3][i] = based_a_v_Pos_calc(object.acceleration, object.veclocity, object_pos[3][i], delta_time);
        //i++;
    //};
}

void ApplyGravity(float delta_t, rigid_body* object){
    if(object->position[3][1] < 0){
        object->object_speed.falling_v += 9.8f * delta_t;         
        object->position[3][1] = based_a_v_Pos_calc(9.8f, object->object_speed.falling_v, object->position[3][1], delta_t);
    }
}

void ApplyMomentum(float delta_t, rigid_body* object){
    if(object->object_speed.jump_v > 1.0f)
        object->object_speed.jump_v -= 0.5f;
    if(object->object_speed.jump_v < 0.0f)
        object->object_speed.jump_v += 0.5f;

    if(object->object_speed.previous_states.fancy_move == JUMPING_FORWARD){
        object->position[3][2] = based_a_v_Pos_calc(0.0f, -object->object_speed.jump_v, object->position[3][2], delta_t);
    }else if(object->object_speed.previous_states.fancy_move == JUMPING_BACKWARD){
        object->position[3][2] = based_a_v_Pos_calc(0.0f, object->object_speed.jump_v, object->position[3][2], delta_t);
    }

};

void Jump(float delta_t, rigid_body* object){
    object->position[3][1] = based_a_v_Pos_calc(-object->object_speed.jump_a, -object->object_speed.jump_v, object->position[3][1], delta_t);
    if(object->object_speed.current_states.fancy_move == JUMPING_FORWARD){
          object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.jump_a * 0.4f, -object->object_speed.jump_v * 0.7, object->position[3][2], delta_t);
    }else if(object->object_speed.current_states.fancy_move == JUMPING_BACKWARD){
          object->position[3][2] = based_a_v_Pos_calc(object->object_speed.jump_a * 0.4f, object->object_speed.jump_v * 0.7, object->position[3][2], delta_t);
    }
}

void CalcNewV(float FrameTime, float* veclo, float* accel){;
    (*veclo) +=  (*accel) * FrameTime;
};
