#if !defined(PHYSICS_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <assimp_glm_helpers.h>
#include "handmade.h"

struct collided_space{
// How to store direction
    glm::vec3 space;
    //glm::ivec3 direction;
    collided_space(glm::vec3 sp):space{sp}{};
};


struct space_box{
    // We have to spawn the unique id for every object in order not to confuse with other one
    glm::mat4 position;
    glm::vec3 size;

    std::vector<collided_space>* collide_list;

    space_box(glm::vec3* first_pos = nullptr, glm::vec3* size_ = nullptr){
        if(size_ != nullptr)
        size = *size_; 
        
        position = glm::mat4(1.0f);
        if(first_pos != NULL){
            position = glm::translate(position, (*first_pos));
        } else {
            position = glm::translate(position, glm::vec3(0.0f));
        }

        collide_list = new std::vector<collided_space>;
        collide_list->reserve(10);
    }
//NOTE: How to define direction for both box
    // question is do we need to store it on one general list or each object space box list
    // animation for each motions;
};

bool32 check_collision(space_box* box1 = nullptr, space_box* box2 = nullptr);
void GetBackToPosBeforeHit(glm::mat4* position = nullptr, const glm::vec3* collided_box = nullptr);
#define PHYSICS_H
#endif
