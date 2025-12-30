/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animation.h"

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
    float scaleFactor = 0.0f
    float midWayLength = animationTime - lastkeyTime;
    float timeDiff = nextkeyTime - lastkeyTime;

    scaleFactor = midWayLength/timeDiff;

    return scaleFactor;
};

// Figure out which Postion key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolatePosition(float animationTime){

    if(m_NumPosition == 1)
        return glm::translate(glm::mat4(1.0f), m_Positions[0].Postion);
    
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

    if(m_NumScalings == 1)
        return glm::scale(glm::mat4(1.0f), m_Scales[0].Scale);

    int pt0index = GetScalingIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, m_Scales[pt0index].timestamp, m_Scales[pt1index].timestamp);

    glm:vec3 finalScale = glm::mix(m_Scales[pt0index].Scale, m_Scales[pt1index].Scale, scaleFactor);

    return glm::scale(glm::mat4(1.0f), finalScale);    
};

// Figure out which Scale key to interpolation b/w interpolate and return the
// final matrix
glm::mat4 Bone::InterpolateRotation(float animationTime){
    if(m_NumRotations == 1){
        auto rotation = m_Rotations[0].orientation;
        glm::toMat4(rotation);
    }

    int pt0index = GetRotationIndex(animationTime);
    int pt1index = pt0index + 1;
    float scaleFactor = GetScaleFactor(animationTime, m_Rotations[pt0index].timestamp, m_Rotations[pt1index].timestamp);;

    glm::quat finalRotation = glm::Lerp(animationTime, Rotations[pt0index].orientaion, Rotations[pt0index].orientaion, scaleFactor)

    return glm::toMat4(finalRotation);
};

Bone* Animation::FindBone(const std::string& name){
    auto iter = find_if(m_Bone.begin(), m_Bone.end(), [&](const Bone& Bone){return Bone.GetBoneName() == name;});//lambda to findout address of bone that have the same name of given name
    if(iter == m_Bones.end())return nullptr;
    //return address of member that iter is pointing to
    else return &(*iter);
};

void Animation::ReadMissingBone(const aiAnimation* animation, const Model* model){
    int size = animation->mNumChannels;
// Get these properties from model var
    std::map<std::string, BoneInfo> boneInfoMap = model->GetBoneInfoMap();
    int& boneCount = model->GetBoneCount();

    //Reading channels (bone engaged in an animation and keyframes)

    for(int i = 0; i < size; i++){
        Channel* channel = animation->mChannels[i];
        std::string boneName = animation->mNodeName.data;

        if(boneInfoMap.find(boneName) == boneInfoMap.end()){
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }
    m_BoneInfoMap = boneInfoMap;
};

void Animation::ReadHierarchyData(const AssimpNodeData& dest, const aiNode* src){
    assert(src);

    Dest.name = src->mName.Data;
    Dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    Dest.childrenCount = src->mChildrenCount;

    for(int i = 0; i < src->mChildrenCount; i++){
        AssimpNodeData child = new AssimpNodeData;
        ReadHierarchyData(child, src->mChildren[i]);
        dest.children.push_back(child);
    };
};


void Animatior::updateAnimationTime(const Animation& animation, float dt){
    m_deltaTime = dt;
    if(currentAnimation){
        m_currentTime += m_deltaTime;
        m_currentTime = fmod(m_currentTime, m_currentAnimation->GetDuration());
        //calculate bone transform here;
    };
};

void Animatior::playAnimation(Animation* pAnimation){
    m_currentAnimation = pAnimation;
    m_currentTime = 0.0f;
};



void Animatior::calculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform){

    std::string nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = m_currentAnimation->FindBone(nodeName);

    if(bone){
        Update(m_CurrentTime);
        nodeTransform = GetLocalTransformation();
    }
    
    glm::mat4 globalTransform = nodeTransform * parentTransform;
    //Find missing bones and do the same
    std::map<std::string, BoneInfo> boneInfoMap = m_currentAnimation->getBoneIDMap();
//WORKING====
    if(boneInfoMap.find(nodeName) != boneInfoMap.end()){
        int index =boneInfoMap[nodeName].id;
        offset = boneInfoMap[nodeName].offset;
        finalBoneMatrices[index] = globalTransform * offset;
    }

    for(int i = 0; i < node->ChildrenCount; i++){
        // Global Transform now is used as parent transform
        Animatior::calculateBoneTransform(&node->children[i], globalTransform);
    }
};
