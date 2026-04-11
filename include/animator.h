#if !defined(ANIMATOR_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <glm/gtx/compatibility.hpp> 
#include "animation.h"

#include <iostream>
#include <unordered_map>


// structure of function type return type of original function (*) (argument type)
//typedef Animation* (* Animationn) (char*, class Model_*);

class Animator{
public:
    Animator(Animation *animation = nullptr):m_currentAnimation(animation) {
      UBO = 0;        

      m_currentTime = 0.0f;
      m_deltaTime = 0.0f;

      m_currentAnimation = animation;
      finalBoneMatrices.reserve(52);
      finalBoneMatrices.resize(52);

      g_iGlobalMatricesBindingIndex = 1;

      globalInverseBoneTransform = glm::mat4(1.0f);
      //NOTE: Init finalBoneMatrices first

      //for(int i = 0; i < 100; i++);
      //finalBoneMatrices.push_back(glm::mat4(1.0f));
      //};
    }
      ~Animator(){
        delete m_currentAnimation;
        m_currentAnimation = nullptr;
        };

    glm::mat4 GetGlobalInverse(){return globalInverseBoneTransform;};
    void SetGlobalInverse(const glm::mat4* Trans){
        globalInverseBoneTransform = (*Trans);};
    
    void updateAnimationTime(const float* dt);

    void playAnimation(Animation* pAnimation);
    void calculateBoneTransform(const AssimpNodeData* node = nullptr, glm::mat4* parentTransform = nullptr);

    void setupUBO(GLuint * ProgramID = nullptr);
    void updateUBOData();
    float GetCurrentTime(){return m_currentTime;};
    
    //float* getFinalBoneMatrices(){return &(*finalBoneMatrices.data())[0][0];};
    std::vector<glm::mat4>* getFinalBoneMatrices(){return &finalBoneMatrices;};

private:
    //Check out the uniform buffer
    unsigned int UBO;
    GLuint g_iGlobalMatricesBindingIndex;
    glm::mat4 globalInverseBoneTransform;
    //Current Animation
    std::vector<glm::mat4>finalBoneMatrices;
    Animation* m_currentAnimation;

    float m_currentTime;
    float m_deltaTime;
};

// Class wrapper
extern "C" __declspec(dllexport) Animator* CreateAnimatorClass(Animation *animation);
extern "C" __declspec(dllexport) void DestroyAnimatorClass(Animator *ani);
extern "C" __declspec(dllexport) void updateAnimationTime_(Animator *ani, const float* dt);
extern "C" __declspec(dllexport) void PlayAni_(Animator* ani, Animation* animation);

extern "C" __declspec(dllexport) void setupUBO_(Animator *ani, GLuint* programID);
extern "C" __declspec(dllexport) void updateUBOData_(Animator *ani);

//Assign new name for function here
typedef Animator* (__cdecl *AniUserClassSpawner) (Animation*);
typedef void (__cdecl *AniUserClassSlayer) (Animator*);
typedef void (__cdecl *AniTimeUpdater) (Animator*, const float*);
typedef void (*PlayAni__)(Animator*, Animation*);
typedef void (*setUpUBO__)(Animator*, GLuint*);
typedef void (*updateUBOData__)(Animator*);

/*
#pragma comment(linker, "/export:CreateAnimatorClass")
#pragma comment(linker, "/export:DestroyAnimatorClass")
#pragma comment(linker, "/export:updateAnimationTime_")
*/
#define ANIMATOR_H
#endif

