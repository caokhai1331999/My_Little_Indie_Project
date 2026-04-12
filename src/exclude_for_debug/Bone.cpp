/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Bone.h"


int Bone::GetPositionIndex(const float* animationTime){

    //for(int index = 0; index < mNumPositions - 1; index++){
    for(int index = 0; index < mNumPositions; index++){
        // why animationTime < m_Positions[index].timestamp
        if((*animationTime) < m_Positions[index].timestamp){
            return index;
        }
    }
    printf("mNumPositions is %d and the last bone timestamp is %f, animation time %f\n", mNumPositions, m_Positions[mNumPositions - 1].timestamp, *animationTime);
    // So the assert happen while there is no animationTime less than any timestamp;
    //assert(0);
}

int Bone::GetRotationIndex(const float* animationTime){
    //for(int index = 0; index < mNumRotations - 1; index++){
    for(int index = 0; index < mNumRotations; index++){
        if((*animationTime) < m_Rotations[index].timestamp)
            return index;
    }
        assert(0);
    //This one track whether the index is invalid or not!!!
}

int Bone::GetScalingIndex(const float* animationTime){
    //for(int index = 0; index < mNumScalings - 1; index++){
    for(int index = 0; index < mNumScalings; index++){
        if((*animationTime) < m_KeyScales[index].timestamp){
            return index;
        }
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
    int pt0index = GetPositionIndex(animationTime);
    int pt1index = pt0index + 1;
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
        int pt0index = GetScalingIndex(animationTime);
        int pt1index = pt0index + 1;
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
        int pt0index = GetRotationIndex(animationTime);

        //if (pt0index < 0) {
        //assert(0);
        //};

        int pt1index = pt0index + 1;
        float scaleFactor = GetScaleFactor(animationTime, &(m_Rotations[pt0index].timestamp), &(m_Rotations[pt1index].timestamp));

        glm::quat finalRotation = glm::slerp(m_Rotations[pt0index].Orientation, m_Rotations[pt1index].Orientation, scaleFactor);

        finalRotation = glm::normalize(finalRotation);
        
        return glm::toMat4(finalRotation);   

};

