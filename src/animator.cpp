/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "animator.h"

bool32 showbefore = false;

void Animator::updateAnimationTime(const float* dt){
    float* m_deltaTime = new float;
    *m_deltaTime = *dt;
    if(m_currentAnimation){
        m_currentTime += (*m_deltaTime) * m_currentAnimation->GetTicksPerSecond();

        m_currentTime = fmod((float)m_currentTime, m_currentAnimation->GetDuration());
        //// calculate bone transform here;
        glm::mat4 parentTransform = glm::mat4(1.0f);
        //printf("test to see code changed once again\n");
//BUGs here//============================================

        //glm::mat4 globalInverseTransform = glm::inverse(m_currentAnimation->getRootNode()->transformation);
        glm::mat4 globalInverseTransform = glm::inverse (m_currentAnimation->getRootNode()->transformation);

        auto TempBoneInfo = m_currentAnimation->GetBoneIDMap();
        
        reserveBoneMatrices((int)TempBoneInfo->size());
        
        SetGlobalInverse(&globalInverseTransform);
        calculateBoneTransform(m_currentAnimation->getRootNode(), &parentTransform);
    };
//============================================
};

void Animator::playAnimation(Animation* pAnimation){
    Animation* currentAni = GetCurrentAnimation();

    float new_threshold = GetCurrentTime() - currentAni->GetDuration();
    SetCurrentTime(&new_threshold);

    //finalBoneMatrices.clear();
    m_currentAnimation = pAnimation;
    //m_currentTime = 0.0f;
};

inline void Animator::calculateBoneTransform(const AssimpNodeData* node, glm::mat4* parentTransform, glm::mat4* GlobalInverse){
 //first fetch Transform from node or matched bone(needed to be updated) existed
    const std::string& nodeName = node->name;
    glm::mat4 nodeTransform = node->transformation;

    //FIND BONE PRODUCE NULL ptr
    Bone* bone = m_currentAnimation->FindBone(nodeName);

    //if(nodeName.size() > 0){
        //printf("Bone name:%s\n", nodeName.c_str());
    //}

    if(bone){
        //Issues happened here
        bone->Update(&m_currentTime);
        nodeTransform = bone->GetLocalTransformation();
    }
    ////

    glm::mat4 globalTransform = (*parentTransform) * nodeTransform;

    //Find missing bones and do the same
    // Bone_Info include id and offset matrix of the bone
    std::unordered_map<std::string, Bone_Info> *boneInfoMap =         m_currentAnimation->GetBoneIDMap();

    //char Buffer[100] = {};
    //std::string global_inverse = glm::to_string(GetGlobalInverse());
    //sprintf(Buffer, "Global Inverse Transform matrix is: %s",global_inverse.c_str()) ;
     //OutputDebugStringA(Buffer);
////
     //std::string test_name = "Hips";
    // WORKING====
    // BUG HERE =================================================
    glm::mat4 globalInverse = GetGlobalInverse();
    if((*boneInfoMap).find(nodeName) != (*boneInfoMap).end()){
        //The boneId represent the index of boneInfo in map
        unsigned int index = (*boneInfoMap)[nodeName].id;
        glm::mat4 offset = (*boneInfoMap)[nodeName].offset;

        //finalBoneMatrices[index] =  inverseIsFinite ? globalInverse * globalTransform * offset: globalTransform * offset;
        finalBoneMatrices[index] =  globalInverse * globalTransform * offset;
    }
    //else {
        //sprintf(Buffer, "There is no bone called %s inside the boneInfoMap\n", nodeName.c_str());
        //OutputDebugStringA(Buffer);
    //}
// BUG HERE =================================================
        //if (!showbefore) {
          //std::cout << "finalBoneMatrix at index 0 "
                    //<< glm::to_string(finalBoneMatrices[0])
                    //<< std::endl;
          //showbefore = true;
          //}
          // NOTE: can not print out finalbonematrices
          
           //Global Transform now is used as parent transform
          if (node->children.size() > 0) {
            for (int i = 0; i < node->children.size(); i++)
            { calculateBoneTransform(&node->children[i],                                              &globalTransform, &globalInverse);
            }
          }
};

void Animator::setupUBO(GLuint* ProgramID){
    // Gen, bind, bufferdata UBO container

    wglMakeCurrent(NULL, NULL);
    HGLRC GLcontext = wglGetCurrentContext();
// We have to get the wglGetopenglextention again so this approaching way is no so
    // viable
    
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)* 52, NULL, GL_STREAM_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //// get Uniform location from ProgramID
    GLint boneMatricIndex = glGetUniformBlockIndex(*ProgramID, "finalBone");
    // bind that location to global binding point (using glUniformBlock binding)
    glUniformBlockBinding(*ProgramID, boneMatricIndex, g_iGlobalMatricesBindingIndex);
    // then bind the binding point to UBO using glBindbufferrange
    glBindBufferRange(GL_UNIFORM_BUFFER, g_iGlobalMatricesBindingIndex, UBO, 0, sizeof(glm::mat4)* 52);
};

void Animator::updateUBOData(){
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4)* 52, &finalBoneMatrices[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
};

/*

Animator* CreateAnimatorClass(Animation *animation) {
  return new Animator(animation);
};

void DestroyAnimatorClass(Animator *ani) {
    delete ani;
}

void updateAnimationTime_(Animator *ani, const float* dt) {
    ani->updateAnimationTime(dt);
}

void PlayAni_ (Animator* ani, Animation* animation){
    ani->playAnimation(animation);
}

void setupUBO_(Animator* ani, GLuint* programeID){
    ani->setupUBO(programeID);  
}

void updateUBOData_(Animator* ani){
    ani->updateUBOData();
}

 */
