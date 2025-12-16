/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animation.h"

float GetScaleFactor(float animationTime, float lastkeyTime, float nextkeyTime){
    float scaleFactor = 0.0f
    float midWayLength = animationTime - lastkeyTime;
    float timeDiff = nextkeyTime - lastkeyTime;

    scaleFactor = midWayLength/timeDiff;

    return scaleFactor;
};

glm::mat4 InterpolatePosition(Bone* bone = nullptr, float animationTime = 0.0f){
    int pt0index = bone->m_Positions[bone->GetPositionIndex(animationTime)];
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, bone->m_Positions[pt0index].timestamp, bone->m_Positions[pt1index].timestamp);

glm:vec3 finalPosition = glm::mix(bone->m_Positions[pt0index].Position, bone->m_Positions[pt1index].Position, scaleFactor);

    glm::mat4 finalTransformMatrix = glm::translate(glm::mat4(1.0f), finalPosition);

    return finalTransformMatrix;
};

glm::mat4 InterpolateScaling(Bone* bone = nullptr, float animationTime = 0.0f);
glm::mat4 InterpolateRotation(Bone* bone = nullptr, float animationTime = 0.0f);
