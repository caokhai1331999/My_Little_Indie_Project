/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "Bone.h"


int Bone::GetPositionIndex(float animationTime){
    for(int index = 0; index < mNumPositions - 1; index++){
        // why animationTime < m_Positions[index].timestamp
        if(animationTime < m_Positions[index + 1].timestamp){
            return index;
        }
        assert(0);
    }
}

int Bone::GetRotationIndex(float animationTime){
    for(int index = 0; index < mNumRotations - 1; index++){
        if(animationTime < m_Rotations[index + 1].timestamp){
            return index;
        }
        assert(0);
    }
}

int Bone::GetScalingIndex(float animationTime){
    for(int index = 0; index < mNumRotations - 1; index++){
        if(animationTime < m_Rotations[index + 1].timestamp){
            return index;
        }
        assert(0);
    }
}

void Bone::Update(float animationTime){
    glm::mat4 Translation = InterpolatePosition(animationTime);
    glm::mat4 Rotation = InterpolateRotation(animationTime);
    glm::mat4 Scaling = InterpolateScaling(animationTime);

    m_LocalTransform = Translation * Rotation * Scaling;
};

// Get normalized value for Lerp and Slerp 
float GetScaleFactor(float animationTime, float lastkeyTime, float nextkeyTime){
    float scaleFactor = 0.0f;
    float midWayLength = animationTime - lastkeyTime;
    float timeDiff = nextkeyTime - lastkeyTime;

    scaleFactor = midWayLength/timeDiff;

    return scaleFactor;
};

// Figure out which Position key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolatePosition(float animationTime){

    if(mNumPositions == 1){
        return glm::translate(glm::mat4(1.0f), m_Positions[0].Position);
    }
    
    int pt0index = GetPositionIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, m_Positions[pt0index].timestamp, m_Positions[pt1index].timestamp);

    glm:vec3 finalPosition = glm::mix(m_Positions[pt0index].Position, m_Positions[pt1index].Position, scaleFactor);

    glm::mat4 finalTransformMatrix = glm::translate(glm::mat4(1.0f), finalPosition);

    return finalTransformMatrix;
};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolateScaling(float animationTime){

    if(mNumScalings == 1){
        return glm::scale(glm::mat4(1.0f), m_KeyScales[0].Scale);
    }

    int pt0index = GetScalingIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, m_KeyScales[pt0index].timestamp, m_KeyScales[pt1index].timestamp);

    glm:vec3 finalScale = glm::mix(m_KeyScales[pt0index].Scale, m_KeyScales[pt1index].Scale, scaleFactor);

    return glm::scale(glm::mat4(1.0f), finalScale);    
};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolateRotation(float animationTime){
    if(mNumRotations == 1){
        auto rotation = m_Rotations[0].Orientation;
        glm::toMat4(rotation);
    }

    int pt0index = GetRotationIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, m_Rotations[pt0index].timestamp, m_Rotations[pt1index].timestamp);;

    glm::quat finalRotation = glm::Lerp(animationTime, Rotations[pt0index].orientaion, Rotations[pt0index].orientaion, scaleFactor)

    return glm::toMat4(finalRotation);
};

