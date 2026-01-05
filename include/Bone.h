#if !defined(BONE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

/*For Bone*/
#include <list>
#include <sstream>
#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL

#include <assimp_glm_helpers.h>

#include "C_Model.h"

struct KeyPosition{
    glm::vec3 Position;
    float timestamp;
};

struct KeyRotation{
    glm::quat Orientation;
    float timestamp;
};

struct KeyScale{
    glm::vec3 Scale;
    float timestamp;
};


struct AssimpNodeData{
    glm::mat4 transformation;
    string name;
    int childrenCount;
    std::vector<AssimpNodeData>children;
};

class Bone{

private:

    std::vector<KeyPosition>m_Positions;
    std::vector<KeyRotation>m_Rotations;
    std::vector<KeyScale>m_KeyScales;

    int mNumPositions;
    int mNumRotations;
    int mNumScalings;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    int m_ID;

    // Read keyframe from aiNodeAnim
    Bone(const std::string& name, int ID, const aiNodeAnim* channel):m_Name(name), m_ID(ID), m_LocalTransform(1.0f){
        //NOTE: In turn we copy KeyPostion, KeyRotation and KeyScale data from aiNodeAnim to Bone

        //First build up POSITION of Bone first
        int m_NumPositions = channel->mNumPositionKeys;
        for(int positionIndex = 0; positionIndex < m_NumPositions; positionIndex++){
            aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = channel->mPositionKeys[positionIndex].mTime;

            KeyPosition Data;
            Data.Position = AssimpGLMHelpers::GetGLMVec(aiPosition);
            Data.timestamp = timeStamp;
            m_Positions.push_back(Data);
        }

        //Then the Rotation
        int m_NumRotations = channel->mNumRotationKeys;
        for(int rotationIndex = 0; rotationIndex < mNumRotations; rotationIndex++){
            aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = channel->mRotationKeys[rotationIndex].mTime;

            KeyRotation Data;
            Data.Orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
            Data.timestamp = timeStamp;

            m_Rotations.push_back(Data);
        };

        // Finally the Scale
        int m_NumScalings = channel->mNumScalingKeys;
        for(int scalingIndex = 0; scalingIndex < m_NumScalings; scalingIndex++){

            aiVector3D scale = channel->mScalingKeys[scalingIndex].mValue;
            float timestamp = channel->mScalingKeys[scalingIndex].mTime;

            KeyScale Data;
            Data.Scale = AssimpGLMHelpers::GetGLMVec(scale);
            Data.timestamp = timestamp;
            m_KeyScales.push_back(Data);
        };
    }
        /* Interpolate b/w translation, rotations and scalings based on the current time and prepare for the final transform matrices by combining all key transformation*/
public:
    glm::mat4 GetLocalTransformation(){return m_LocalTransform;};
        std::string GetBoneName(){return m_Name;};
    int GetBoneID(){return m_ID;};
        void Update(float animationTime);
        /*Return current index on mKeyPositions and interpolate it based on current animation time*/
        int GetPositionIndex(float animationTime);
        /*Return current index on mRotations and interpolate it based on current animation time*/
        int GetRotationIndex(float animationTime);
        /*Return current index on mScalings and interpolate it based on current animation time*/
        int GetScalingIndex(float animationTime);

        // insert bone
        glm::mat4 InterpolatePosition(float animationTime = 0.0f);
        glm::mat4 InterpolateScaling(float animationTime = 0.0f);
        glm::mat4 InterpolateRotation(float animationTime = 0.0f);        
};


#define BONE_H
#endif
