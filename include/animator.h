#if !defined(ANIMATOR_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "animation.h"

class Animator{

public:
    Animator(Animation *animation = nullptr):m_currentAnimation(animation) {
      m_currentTime = 0.0f;
      m_deltaTime = 0.0f;
      };
    ~Animator();

    void updateAnimationTime(const Animation& animation, float dt);
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


#define ANIMATOR_H
#endif
