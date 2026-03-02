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

class Animator{
public:
    Animator(Animation *animation = nullptr):m_currentAnimation(animation) {
      m_currentTime = 0.0f;
      m_deltaTime = 0.0f;
      };
      ~Animator(){
        delete m_currentAnimation;
        m_currentAnimation = nullptr;
        };

    void updateAnimationTime(float dt);
    void playAnimation(Animation* pAnimation);
    void calculateBoneTransform(const AssimpNodeData* node, glm::mat4* parentTransform);

    std::vector<glm::mat4>*getFinalBoneMatrices(){return &finalBoneMatrices;};

private:
    std::vector<glm::mat4>finalBoneMatrices;
    //Current Animation
    Animation* m_currentAnimation;
    float m_currentTime;
    float m_deltaTime;
};


// Class wrapper
extern "C" __declspec(dllexport) Animator* __cdecl CreateAnimatorClass(Animation *animation);
extern "C" __declspec(dllexport) void __cdecl DestroyAnimatorClass(Animator *ani);
extern "C" __declspec(dllexport) void updateAnimationTime_(Animator *ani, float dt);

typedef Animator* (__cdecl *AniUserClassSpawner) (Animation*);
typedef void (__cdecl *AniUserClassSlayer) (Animator*);
typedef void (__cdecl *AniTimeUpdater) (Animator*, float);

#define ANIMATOR_H
#endif

