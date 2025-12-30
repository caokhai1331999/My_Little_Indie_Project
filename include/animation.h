#if !defined(ANIMATION_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <sstream>
#include <map>
#include <vector>

#include <assimp_glm_helpers.h>

#include "C_Mesh.h"

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
            aiVector3D aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
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

class Animation{
public:
    Animation() = default;

    Animation(char* animationPath = nullptr, Model_* model){
        // assert throw out the error when 0 is the value
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        // Still don't understand this part
        assert(scene && scene->mRootNode);
        auto animation = scene->m_Animations[0];
        m_Duration = animation->m_Duration;
        m_TicksPerSecond = animation->mTicksPerSecond;
        ReadHierarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBone(animation, *model);
    };

    ~Animation(){};


    
    inline float GetTicksPerSecond(){return m_TicksPerSecond;};
    inline float GetDuration(){return m_Duration;};
    inline const std::map<std::string, BoneInfo>& GetBoneIDMap(){return m_BoneInfoMap;};
    
// Functions
    void ConstructBone(); 
    void Update();
/*For interpolation each key then the whole*/

    // Get normalized value for Lerp and Slerp
    float GetScaleFactor(float animationTime, float lastkeyTime, float nextkeyTime);

// Part that finish the animation process
    Bone* FindBone(const std::string& name);
    void ReadMissingBone(const aiAnimation* animation, const Model* model);
    void ReadHierarchyData(const  AssimpNodeData& dest, const aiNode* src);

private:
    float m_Duration;
    int m_TicksPerSecond;
    std::vector<Bone> m_Bones;
    AssimpNodeData m_RootNode;
    std::map<std::string, BoneInfo> m_BoneInfoMap;
};

//NOTE: Now the animator class

class Animator{

public:
    void updateAnimationTime(const Animation& animation, float dt);
    void playAnimation(Animation* pAnimation);
    void calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
    std::vector<glm::mat4>getFinalBoneMatrices(){return finalBoneMatrices};
private:
    std::vector<glm::mat4>finalBoneMatrices;
    //Current Animation
    Animation* m_currentAnimation;
    float m_currentTime;
    float m_deltaTime;
};

#define ANIMATION_H
#endif
