/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animation.h"
// Get normalized value for Lerp and Slerp 
float GetScaleFactor(float animationTime, float lastkeyTime, float nextkeyTime){
    float scaleFactor = 0.0f
    float midWayLength = animationTime - lastkeyTime;
    float timeDiff = nextkeyTime - lastkeyTime;

    scaleFactor = midWayLength/timeDiff;

    return scaleFactor;
};

// Figure out which Postion key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 InterpolatePosition(Bone* bone = nullptr, float animationTime = 0.0f){

    if(Bone->m_NumPosition == 1)
        return glm::translate(glm::mat4(1.0f), Bone->m_Positions[0].Postion);
    
    int pt0index = bone->GetPositionIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, bone->m_Positions[pt0index].timestamp, bone->m_Positions[pt1index].timestamp);

glm:vec3 finalPosition = glm::mix(bone->m_Positions[pt0index].Position, bone->m_Positions[pt1index].Position, scaleFactor);

    glm::mat4 finalTransformMatrix = glm::translate(glm::mat4(1.0f), finalPosition);

    return finalTransformMatrix;
};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 InterpolateScaling(Bone* bone = nullptr, float animationTime = 0.0f){

    if(Bone->m_NumScalings == 1)
        return glm::scale(glm::mat4(1.0f), Bone->m_Scales[0].Scale);

    int pt0index = bone->GetScalingIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, bone->m_Scales[pt0index].timestamp, bone->m_Scales[pt1index].timestamp);

    glm:vec3 finalScale = glm::mix(bone->m_Scales[pt0index].Scale, bone->m_Scales[pt1index].Scale, scaleFactor);

    return glm::scale(glm::mat4(1.0f), finalScale);    
};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 InterpolateRotation(Bone* bone = nullptr, float animationTime = 0.0f){
    if(Bone->m_NumRotations == 1){
        auto rotation = Bone->m_Rotations[0].orientation;
        glm::toMat4(rotation);
    }

    int pt0index = bone->GetRotationIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, bone->m_Rotations[pt0index].timestamp, bone->m_Rotations[pt1index].timestamp);;

    glm::quat finalRotation = glm::Lerp(animationTime, bone->Rotations[pt0index].orientaion, bone->Rotations[pt0index].orientaion, scaleFactor)

    return glm::toMat4(finalRotation);
};
