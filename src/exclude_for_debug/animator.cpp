/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <unordered_map>
#include "animator.h"

void Animator::updateAnimationTime(float dt){
    m_deltaTime = dt;
    if(m_currentAnimation){
        m_currentTime += m_deltaTime;
        m_currentTime = fmod(m_currentTime, m_currentAnimation->GetDuration());
        //// calculate bone transform here;
        glm::mat4 parentTransform = glm::mat4(1.0f);
        //printf("test to see code changed once again\n");
        calculateBoneTransform(m_currentAnimation->getRootNode(), &parentTransform);
        };
};

void Animator::playAnimation(Animation* pAnimation){
    m_currentAnimation = pAnimation;
    m_currentTime = 0.0f;
};

void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4* parentTransform){

    const std::string& nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    Bone* bone = m_currentAnimation->FindBone(nodeName);

    if(bone){
        bone->Update(m_currentTime);
        nodeTransform = bone->GetLocalTransformation();
    }
    
    glm::mat4 globalTransform = nodeTransform * (*parentTransform);
    //Find missing bones and do the same
    std::unordered_map<std::string, Bone_Info>* boneInfoMap = m_currentAnimation->GetBoneIDMap();
//WORKING====
    if((*boneInfoMap).find(nodeName) != (*boneInfoMap).end()){
        int index =(*boneInfoMap)[nodeName].id;
        glm::mat4 offset = (*boneInfoMap)[nodeName].offset;
        finalBoneMatrices[index] = globalTransform * offset;
    }

    for(int i = 0; i < node->children.size(); i++){
        // Global Transform now is used as parent transform
        Animator::calculateBoneTransform(&node->children[i], &globalTransform);
    }
};

Animator* CreateAnimatorClass(Animation *animation) {
  return new Animator(animation);
};

void DestroyAnimatorClass(Animator *ani) {
    delete ani;
}

void updateAnimationTime_(Animator *ani, float dt) {
  ani->updateAnimationTime(dt);
}

