/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animator.h"

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

