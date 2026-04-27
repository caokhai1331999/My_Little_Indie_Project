#if !defined(BONE_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include <assimp_glm_helpers.h>

#include "C_Model.h"

struct KeyPosition{
    glm::vec3 Position;
    float timestamp;
};

struct KeyRotation{
    glm::fquat Orientation;
    float timestamp;
};

struct KeyScale{
    glm::vec3 Scale;
    float timestamp;
};

struct AssimpNodeData{
    glm::mat4 transformation;
    std::string name;
    std::vector<AssimpNodeData>children;
};
// Test zone=====================
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
        size = *size_; 
        
        position = glm::mat4(1.0f);
        position = glm::translate(position, (*first_pos));

        collide_list = new std::vector<collided_space>;
        collide_list->reserve(10);
    }
//NOTE: How to define direction for both box
    // question is do we need to store it on one general list or each object space box list
    // animation for each motions;
};
// Test zone=====================
class Bone{

private:

    std::vector<KeyPosition>m_Positions;
    std::vector<KeyRotation>m_Rotations;
    std::vector<KeyScale>m_KeyScales;

    unsigned int mNumPositions;
    unsigned int mNumRotations;
    unsigned int mNumScalings;

    glm::mat4 m_LocalTransform;
    std::string m_Name;
    unsigned int m_ID;

public:
    // Read keyframe from aiNodeAnim
    Bone(){};
    Bone(const std::string& name, unsigned int ID, const aiNodeAnim* channel):m_Name(name), m_ID(ID), m_LocalTransform(1.0f){
        //NOTE: In turn we copy KeyPostion, KeyRotation and KeyScale data from aiNodeAnim to Bone

        //First build up POSITION of Bone first
        mNumPositions = channel->mNumPositionKeys;

        for(unsigned int positionIndex = 0; positionIndex < mNumPositions; positionIndex++){
            aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
            float timeStamp = channel->mPositionKeys[positionIndex].mTime;

            KeyPosition Data;
            Data.Position = AssimpGLMHelpers::GetGLMVec(aiPosition);
            Data.timestamp = timeStamp;
            m_Positions.push_back(Data);
        }

        //Then the Rotation
        mNumRotations = channel->mNumRotationKeys;
        for(unsigned int rotationIndex = 0; rotationIndex < mNumRotations; rotationIndex++){
            aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
            float timeStamp = channel->mRotationKeys[rotationIndex].mTime;

            KeyRotation Data;
            Data.Orientation = AssimpGLMHelpers::GetGLMQuat(aiOrientation);
            Data.timestamp = timeStamp;

            m_Rotations.push_back(Data);
        };

        // Finally the Scale
        mNumScalings = channel->mNumScalingKeys;
        for(unsigned int scalingIndex = 0; scalingIndex < mNumScalings; scalingIndex++){

            aiVector3D scale = channel->mScalingKeys[scalingIndex].mValue;
            float timestamp = channel->mScalingKeys[scalingIndex].mTime;

            KeyScale Data;
            Data.Scale = AssimpGLMHelpers::GetGLMVec(scale);
            Data.timestamp = timestamp;
            m_KeyScales.push_back(Data);
        };
    }
        /* Interpolate b/w translation, rotations and scalings based on the current time and prepare for the final transform matrices by combining all key transformation*/    
  glm::mat4 GetLocalTransformation(){return m_LocalTransform;};
        const std::string* GetBoneName() const {return &m_Name;};
    int GetBoneID(){return m_ID;};
        void Update(const float* animationTime);
        /*Return current index on mKeyPositions and interpolate it based on current animation time*/
        unsigned int GetPositionIndex(const float* animationTime);
        /*Return current index on mRotations and interpolate it based on current animation time*/
        unsigned int GetRotationIndex(const float* animationTime);
        /*Return current index on mScalings and interpolate it based on current animation time*/
        unsigned int GetScalingIndex(const float* animationTime);

        // insert bone
        glm::mat4 InterpolatePosition(const float* animationTime);
        glm::mat4 InterpolateScaling(const float* animationTime);
        glm::mat4 InterpolateRotation(const float* animationTime);        

    unsigned int GetNumPosition(){return mNumPositions;};
    float GetPositionTimeStamp(int index = 0){
        return m_Positions[index].timestamp;
    }
};


bool32 check_collision(space_box* box1 = nullptr, space_box* box2 = nullptr);

extern "C" __declspec(dllexport) bool32 __cdecl check_collision_wrapper(space_box* box1 = nullptr, space_box* box2 = nullptr);
//declare a function pointer;
typedef bool32 (__cdecl *check_collision_) (space_box*, space_box*);


#define BONE_H
#endif
