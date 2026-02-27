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
typedef Animation* (* AniClassSpawner) (char *, Model_ *);
typedef void (* AniClassSlainer) (Animation* );



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

typedef Animator* (* AniUserClassSpawner) (Animation*);

// Class wrapper
extern "C" __declspec(dllexport) Animator *CreateAnimatorClass(Animation *animation) {
  Animator *newAnimator = new Animator(animation);
  return newAnimator;
};

extern "C" __declspec(dllexport) void DestroyAnimatorClass(Animator *ani) {
    delete ani;
}

extern "C" __declspec(dllexport) void updateAnimationTimee(Animator *ani, float dt) {
    ani->updateAnimationTime(dt);
}

typedef void (* AniUserClassSlayer) (Animator*);
typedef void (* AniTimeUpdater) (Animator*, float);

#define ANIMATOR_H
#endif

