/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Bone.h"


unsigned int Bone::GetPositionIndex(const float* animationTime){

 for(unsigned int index = 0; index < mNumPositions - 1; index++){
        // why animationTime < m_Positions[index].timestamp
        if((*animationTime) < m_Positions[index].timestamp){
            return index;
        }

        //if(index == mNumPositions - 2){
            //printf("mNumPositions is %d and the second last bone timestamp is %f, animation time %f\n", mNumPositions - 2, m_Positions[mNumPositions - 2].timestamp, *animationTime);
//
            //if((*animationTime) < m_Positions[index].timestamp){
                //printf("The comparing operation worked, the animation time did perceived to be less than the second last Position timestamp.\n");
            //};
        //}
    }

 if((*animationTime) > m_Positions[mNumPositions - 2].timestamp && (*animationTime) < m_Positions[mNumPositions - 1].timestamp)
 {
     return mNumPositions - 2;
 }
    //If do this way the last frame index will get out of the bound
    //printf("mNumPositions is %d and the last bone timestamp is %f, animation time %f\n", mNumPositions, m_Positions[mNumPositions - 1].timestamp, *animationTime);
    // So the assert happen while there is no animationTime less than any timestamp;
    assert(0);
}

unsigned int Bone::GetRotationIndex(const float* animationTime){
    for(unsigned int index = 0; index < mNumRotations - 1 ; index++){
        if((*animationTime) < m_Rotations[index].timestamp)
            return index;
    }

    if((*animationTime) > m_Rotations[mNumRotations - 2].timestamp && (*animationTime) < m_Rotations[mNumRotations - 1].timestamp)
    {
        return mNumRotations - 2;
    }    
        assert(0);
    //This one track whether the index is invalid or not!!!
}

unsigned int Bone::GetScalingIndex(const float* animationTime){
    for(unsigned int index = 0; index < mNumScalings - 1; index++){
        if((*animationTime) < m_KeyScales[index].timestamp){
            return index;
        }
    }

    if((*animationTime) > m_KeyScales[mNumScalings - 2].timestamp && (*animationTime) < m_KeyScales[mNumScalings - 1].timestamp)
    {
        return mNumScalings - 2;
    }

    assert(0);
}

void Bone::Update(const float* animationTime){

    glm::mat4 Translation = InterpolatePosition(animationTime);
    glm::mat4 Rotation = InterpolateRotation(animationTime);
    glm::mat4 Scaling = InterpolateScaling(animationTime);

    m_LocalTransform = Translation * Rotation * Scaling;
};

// Get normalized value for Lerp and Slerp 
float GetScaleFactor(const float* animationTime, float* lastkeyTime, float* nextkeyTime){

    float scaleFactor = 0.0f;

    float timeDiff = (*nextkeyTime) - (*lastkeyTime);
    float midWayLength = (*animationTime) - (*lastkeyTime);

    scaleFactor = midWayLength/timeDiff;

    return scaleFactor;
};

// Figure out which Position key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolatePosition(const float* animationTime){

    if(mNumPositions == 1){
        return glm::translate(glm::mat4(1.0f), m_Positions[0].Position);
    } else if (mNumPositions == 0){
        return glm::mat4(1.0f);
    }

    unsigned int pt0index = GetPositionIndex(animationTime);
    unsigned int pt1index = 0;

    if(pt0index < GetNumPosition() - 1)
    pt1index = pt0index + 1;

    float scaleFactor = GetScaleFactor(animationTime, &(m_Positions[pt0index].timestamp), &(m_Positions[pt1index].timestamp));

    glm::vec3 finalPosition = glm::mix(m_Positions[pt0index].Position, m_Positions[pt1index].Position, scaleFactor);

    glm::mat4 finalTransformMatrix = glm::translate(glm::mat4(1.0f), finalPosition);

    return finalTransformMatrix;
};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolateScaling(const float* animationTime){

    if(mNumScalings == 1){
        return glm::scale(glm::mat4(1.0f), m_KeyScales[0].Scale);
    } else if (mNumScalings == 0){
        return glm::mat4(1.0f);
    } 
        unsigned int pt0index = GetScalingIndex(animationTime);
        unsigned int pt1index  = 0;;

        if(pt0index < mNumScalings - 1)
        pt1index = pt0index + 1;

        float scaleFactor = GetScaleFactor(animationTime, &(m_KeyScales[pt0index].timestamp), &(m_KeyScales[pt1index].timestamp));

        glm::vec3 finalScale = glm::mix(m_KeyScales[pt0index].Scale, m_KeyScales[pt1index].Scale, scaleFactor);

        return glm::scale(glm::mat4(1.0f), finalScale);       

};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolateRotation(const float* animationTime){

    if(mNumRotations == 1){
        auto rotation = m_Rotations[0].Orientation;
        return glm::toMat4(rotation);
    } else if (mNumRotations == 0){
        return glm::mat4(1.0f);
    } 
        // NOTE: things were tracked to go wrong from here
        unsigned int pt0index = GetRotationIndex(animationTime);

        //if (pt0index < 0) {
        //assert(0);
        //};
        unsigned int pt1index = 0;

        if(pt0index < mNumRotations - 1)
        pt1index = pt0index + 1;

        float scaleFactor = GetScaleFactor(animationTime, &(m_Rotations[pt0index].timestamp), &(m_Rotations[pt1index].timestamp));

        glm::fquat finalRotation = glm::slerp(m_Rotations[pt0index].Orientation, m_Rotations[pt1index].Orientation, scaleFactor);

        finalRotation = glm::normalize(finalRotation);
        
        return glm::toMat4(finalRotation);   

};

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

bool32 __cdecl check_collision_wrapper(space_box* box1, space_box* box2){
    return check_collision(box1, box2);  
};


std::string* load_bin_map(const char* name){
    std::string map_name = name;
    std::fstream bin_map {map_name, bin_map.in};

    //std::string str;
    //this is an array of pointer
    std::string* map_content;
    char cursor;
    map_content->reserve(120);
    if(bin_map.is_open()){
        //bin_map.read(&(*map_content)[0], 120);
        int loop_count = 0;
        while(bin_map >> cursor){
            (*map_content)[loop_count] = cursor;
            loop_count++;
        };
        if(bin_map.eof()){
            //printf("Succeed read end of file, loop counted:%d\n", loop_count);
            printf("Succeed read end of file,\n");
        }
        printf("Succeed loading map content\n");
    }else{
        printf("Failed loading map content\n");
    } 
    return map_content;
}


std::string* __load_bin_map_wrapper(const char* name){
    return load_bin_map(name);
}
