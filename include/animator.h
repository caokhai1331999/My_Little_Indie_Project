#if !defined(ANIMATOR_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "animation.h"

// structure of function type return type of original function (*) (argument type)
//typedef Animation* (* Animationn) (char*, class Model_*);

class  Animator{
public:
    Animator(Animation *animation = nullptr):m_currentAnimation(animation) {
      m_currentTime = 0.0f;
      m_deltaTime = 0.0f;

      m_currentAnimation = animation;
      finalBoneMatrices.reserve(100);
      finalBoneMatrices.resize(100);
      //NOTE: Init finalBoneMatrices first
      for(int i = 0; i < 100; i++);
      finalBoneMatrices.push_back(glm::mat4(1.0f));
      };

      ~Animator(){
        delete m_currentAnimation;
        m_currentAnimation = nullptr;
        };

        // Getters

        // Setters
    void SetDeltaTime(real64 Time);
    void AddDeltaTime(real64 Time);

    void updateAnimationTime(real64 dt);

    void playAnimation(Animation* pAnimation);
    void calculateBoneTransform(const AssimpNodeData* node, glm::mat4* parentTransform);

    std::vector<glm::mat4>*getFinalBoneMatrices(){return &finalBoneMatrices;};

private:
    //Current Animation
    std::vector<glm::mat4>finalBoneMatrices;
    Animation* m_currentAnimation;
    real64 m_currentTime;
    real64 m_deltaTime;
};

// Class wrapper
extern "C" __declspec(dllexport) Animator* CreateAnimatorClass(Animation *animation);
extern "C" __declspec(dllexport) void DestroyAnimatorClass(Animator *ani);
extern "C" __declspec(dllexport) void updateAnimationTime_(Animator *ani, real64 dt);

typedef Animator* (__cdecl *AniUserClassSpawner) (Animation*);
typedef void (__cdecl *AniUserClassSlayer) (Animator*);
typedef void (__cdecl *AniTimeUpdater) (Animator*, real64);

/*
#pragma comment(linker, "/export:CreateAnimatorClass")
#pragma comment(linker, "/export:DestroyAnimatorClass")
#pragma comment(linker, "/export:updateAnimationTime_")
*/
#define ANIMATOR_H
#endif

