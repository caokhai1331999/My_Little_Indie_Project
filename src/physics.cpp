/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "physics.h"

bool32 check_collision(space_box* box1, space_box* box2){
    //AABB BBAA
    //assume that the position point is in the center of the box
    bool32 collided = false;

    glm::vec3 spaceforBox1;
    //ON X-axis
    if (((box1->position[3][0] < box2->position[3][0])&&(box1->position[3][0] + box1->size.x > box2->position[3][0])) || ((box2->position[3][0] < box1->position[3][0])&&(box2->position[3][0] + box2->size.x > box1->position[3][0]))){
        //implement here
        spaceforBox1.x = box1->position[3][0] - box2->position[3][0];
        !collided?collided = true:collided=false;     
    }
    //ON y-axis
    else if (((box1->position[3][1] < box2->position[3][1])&&(box1->position[3][1] + box1->size.y > box2->position[3][1])) || ((box2->position[3][1] < box1->position[3][1])&&(box2->position[3][1] + box2->size.y > box1->position[3][1]))){
        spaceforBox1.y = box1->position[3][1] - box1->position[3][1];
        !collided?collided = true:collided=false;        
    }
    //ON z-axis
    else if (((box1->position[3][2] < box2->position[3][2])&&(box1->position[3][2] + box1->size.z > box2->position[3][2])) || ((box2->position[3][2] < box1->position[3][2])&&(box2->position[3][2] + box2->size.z > box1->position[3][2]))){
        spaceforBox1.z = box1->position[3][2] - box1->position[3][2];
        !collided?collided = true:collided=false;        
    }

    box2->collide_list->push_back(collided_space(spaceforBox1));
    glm::vec3 spaceforBox2 = glm::vec3(-1 * spaceforBox1.x, -1 * spaceforBox1.y, -1 * spaceforBox1.z);
    box1->collide_list->push_back(collided_space(spaceforBox2));

    return collided;
};
