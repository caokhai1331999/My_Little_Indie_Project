/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <iostream>
#include <unordered_map>
#include "animator.h"

bool32 showbefore = false;

void Animator::updateAnimationTime(real64* dt){
    real64* m_deltaTime = dt;
    if(m_currentAnimation){
        m_currentTime += (*m_deltaTime) * m_currentAnimation->GetTicksPerSecond();
        m_currentTime = fmod((float)m_currentTime, m_currentAnimation->GetDuration());
        //// calculate bone transform here;
        glm::mat4 parentTransform = glm::mat4(1.0f);
        //printf("test to see code changed once again\n");
//BUGs here//============================================
        calculateBoneTransform(m_currentAnimation->getRootNode(), &parentTransform);
    };
//============================================
};

void Animator::playAnimation(Animation* pAnimation){
    m_currentAnimation = pAnimation;
    m_currentTime = 0.0f;
};

void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4* parentTransform){

    const std::string& nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    //FIND BONE PRODUCE NULL ptr
    Bone* bone = m_currentAnimation->FindBone(nodeName);

    //if(nodeName.size() > 0){
        //printf("Bone name:%s\n", nodeName.c_str());
    //}

    if(bone){
        bone->Update(&m_currentTime);
        nodeTransform = bone->GetLocalTransformation();
    }
    //

    glm::mat4 globalTransform = nodeTransform * (*parentTransform);

    //Find missing bones and do the same
    std::unordered_map<std::string, Bone_Info> *boneInfoMap =
        m_currentAnimation->GetBoneIDMap();

    char Buffer[100] = {};
    //std::string offset_content = glm::to_string((*boneInfoMap)["Hips"].offset);
    //sprintf(Buffer, "BoneInfo offset Matrix at Hips is: %s",offset_content.c_str()) ;
     //OutputDebugStringA(Buffer);
     //std::string test_name = "Hips";
    //
     
    // WORKING====
    // BUG HERE =================================================
    if((*boneInfoMap).find(nodeName) != (*boneInfoMap).end()){
        int index = (*boneInfoMap)[nodeName].id;
        glm::mat4 offset = (*boneInfoMap)[nodeName].offset;
        finalBoneMatrices[index] = globalTransform * offset;
     } else {
        sprintf(Buffer, "There is no bone called %s inside the boneInfoMap\n", nodeName.c_str());
        OutputDebugStringA(Buffer);
    }
// BUG HERE =================================================
        //if (!showbefore) {
          //std::cout << "finalBoneMatrix at index 0 "
                    //<< glm::to_string(finalBoneMatrices[0])
                    //<< std::endl;
          //showbefore = true;
          //}
          // NOTE: can not print out finalbonematrices
          
           //Global Transform now is used as parent transform
          if ((int)node->children.size() > 0) {
            for (int i = 0; i < node->children.size(); i++) {
              Animator::calculateBoneTransform(&node->children[i],
                                               &globalTransform);
            }
          }
};

Animator* CreateAnimatorClass(Animation *animation) {
  return new Animator(animation);
};

void DestroyAnimatorClass(Animator *ani) {
    delete ani;
}

void updateAnimationTime_(Animator *ani, real64* dt) {
  ani->updateAnimationTime(dt);
}

void PlayAni_ (Animator* ani, Animation* animation){
    ani->playAnimation(animation);
}
