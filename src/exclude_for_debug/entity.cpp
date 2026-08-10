/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "entity.h"

void rigid_body::move(float delta_t){

    if(this->current_face != this->previous_face){
        OrientFace(this);
    }

    glm::vec3 UpAxis = glm::vec3(this->position[1]);
    glm::vec3 xAxis = glm::vec3(this->position[0]);
    glm::vec3 zAxis = glm::vec3(this->position[2]);

    printf("object current position is %s\n", glm::to_string(this->position).c_str());
    printf("object current up axis is %s\n", glm::to_string(UpAxis).c_str());
    printf("object current x axis is %s\n", glm::to_string(xAxis).c_str());
    printf("object current z axis is %s\n", glm::to_string(zAxis).c_str());    
    
    CalcNewPos(delta_t, this);
}

void move_object(float delta_t, rigid_body* object){

    if(object->object_speed.current_states != IDLE && (object->object_speed.previous_states == object->object_speed.current_states)){
        CalcNewV(delta_t, &object->object_speed.veclocity, &object->object_speed.acceleration);
    }else{
        object->object_speed.veclocity = object->object_speed.base_veclocity;
    }

    object->move(delta_t);
}

static void  Orient_Around_Y(glm::mat4* matrix, float angle){
// We forgot the Scale factor here
    //1st col
    (*matrix)[0][0] = (float)glm::cos(glm::radians(angle))/100;
    (*matrix)[0][1] = 0.0f;
    (*matrix)[0][2] = (float)-glm::sin(glm::radians(angle))/100;
    //2nd col
    (*matrix)[1][0] = 0.0f;
    (*matrix)[1][1] = (float)1/100;
    (*matrix)[1][2] = 0.0f;
    //3rd col
    (*matrix)[2][0] = (float)glm::sin(glm::radians(angle))/100;
    (*matrix)[2][1] = 0.0f;
    (*matrix)[2][2] = (float)glm::cos(glm::radians(angle))/100;

    //*matrix = glm::rotate(*matrix, glm::radians(angle), glm::vec3(0, 1, 0));
};

static void OrientFace(rigid_body* object){
    float deg ;
    if(object->current_face != object->previous_face){
         //deg = 90.0f * -float(object->current_face - object->previous_face);
         deg = 90.0f * float(object->current_face);
        if(deg > 360.0f)
            deg -= 360.0f;
        if(deg < -360.0f)
            deg += 360.0f;
    }else{
        deg = 0;
    }
    Orient_Around_Y(&object->position, deg);
}

static void CalcNewPos(float delta_time, rigid_body* object) {
    float value;
    //add a little friction(opposed force here)
   //
    switch(object->object_speed.current_states){
        case WALK_FORWARD:
            object->position[3][2] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][2], delta_time);            
            break;
        case WALK_BACKWARD:
            object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.acceleration, -object->object_speed.veclocity, object->position[3][2], delta_time);
            break;
        case WALK_RIGHT:
            object->position[3][0] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][0], delta_time);            
            break;
        case WALK_LEFT:
            object->position[3][0] = based_a_v_Pos_calc(-object->object_speed.acceleration, -object->object_speed.veclocity, object->position[3][0], delta_time);
            break;
        //default:
            //printf("still don't what to put into this defaut case\n");
            //break;
    };

    if(abs(object->position[3][2] - object->pre_pos.z) >= 3.0f){
        printf("distance traveled until falling:%f\n", abs(object->position[3][2] - object->pre_pos.z));
        if(object->object_speed.current_states == JUMP_FORWARD || object->object_speed.current_states == JUMP_BACKWARD){
            object->object_speed.previous_states = object->object_speed.current_states;
            object->object_speed.current_states = FALLING;

            test_vampire_motion.pre_pos = glm::vec3(test_vampire_motion.position[3]);
        }
    }else{
        CalcNewV(delta_time, &object->object_speed.jump_v, &object->object_speed.jump_a);
    }

    
    switch(object->object_speed.current_states){
        case JUMP_FORWARD:
            Jump(delta_time, object);
            ApplyGravity(delta_time, object);
            break;

        case JUMP_BACKWARD:
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

    if(object->position[3][1] >= 0.0f && object->object_speed.current_states == FALLING){
        object->object_speed.previous_states = object->object_speed.current_states;
        object->object_speed.current_states = IDLE;
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

static void  ApplyGravity(float delta_t, rigid_body* object){
    if(object->position[3][1] < 0){
        object->object_speed.falling_v += 9.8f * delta_t;         
        object->position[3][1] = based_a_v_Pos_calc(9.8f, object->object_speed.falling_v, object->position[3][1], delta_t);
    }
}

static void ApplyMomentum(float delta_t, rigid_body* object){
    if(object->object_speed.jump_v > 1.0f)
        object->object_speed.jump_v -= 0.5f;
    if(object->object_speed.jump_v < 0.0f)
        object->object_speed.jump_v += 0.5f;

    if(object->object_speed.previous_states == JUMP_FORWARD){
        object->position[3][2] = based_a_v_Pos_calc(0.0f, -object->object_speed.jump_v, object->position[3][2], delta_t);
    }else if(object->object_speed.previous_states == JUMP_BACKWARD){
        object->position[3][2] = based_a_v_Pos_calc(0.0f, object->object_speed.jump_v, object->position[3][2], delta_t);
    }

};

static void Jump(float delta_t, rigid_body* object){

    object->position[3][1] = based_a_v_Pos_calc(-object->object_speed.jump_a, -object->object_speed.jump_v, object->position[3][1], delta_t);
    if(object->object_speed.current_states == JUMP_FORWARD){
          object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.jump_a * 0.4f, -object->object_speed.jump_v * 0.7, object->position[3][2], delta_t);
    }else if(object->object_speed.current_states == JUMP_BACKWARD){
          object->position[3][2] = based_a_v_Pos_calc(object->object_speed.jump_a * 0.4f, object->object_speed.jump_v * 0.7, object->position[3][2], delta_t);
    }
}

static void CalcNewV(float FrameTime, float* veclo, float* accel){;
    (*veclo) +=  (*accel) * FrameTime;
};
