#include <unordered_map>
#if !defined(ANIMATION_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include <sstream>
#include <unordered_map>
#include <vector>

#include <assimp_glm_helpers.h>

#include "C_Mesh.h"
#include "Bone.h"
#include "C_Model.h"

class Animation{
public:
    Animation() = default;

    Animation(char* animationPath = nullptr, Model_* model = nullptr){
      // assert throw out the error when 0 is the value
        m_Bones.reserve(100);
        m_Bone_InfoMap.reserve(100);;
        
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
        // Still don't understand this part
        assert(scene && scene->mRootNode);
        auto animation = scene->mAnimations[0];
        m_Duration = animation->mDuration;
        m_TicksPerSecond = animation->mTicksPerSecond;
        ReadHierarchyData(m_RootNode, scene->mRootNode);
        ReadMissingBone(animation, model);

        };

    ~Animation(){};


    
    inline float GetTicksPerSecond(){return m_TicksPerSecond;};
    inline float GetDuration(){return m_Duration;};
    inline std::unordered_map<std::string, Bone_Info>* GetBoneIDMap() {return &m_Bone_InfoMap;};
    
// Functions
    void ConstructBone(); 
    void Update();
/*For interpolation each key then the whole*/

    // Get normalized value for Lerp and Slerp
    float GetScaleFactor(float animationTime, float lastkeyTime, float nextkeyTime);

// Part that finish the animation process
    Bone* FindBone(const std::string& name);
    void ReadMissingBone(const aiAnimation* animation, Model_* model);
    void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);

private:
    float m_Duration;
    int m_TicksPerSecond;
    AssimpNodeData m_RootNode;
    std::vector<Bone> m_Bones;
    std::unordered_map<std::string, Bone_Info> m_Bone_InfoMap;
};
#define ANIMATION_H
#endif
