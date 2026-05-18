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
    //Animation() = default;
    Animation(const char* animationPath = nullptr, Model_* model = nullptr);
    ~Animation(){};
    
    inline float GetTicksPerSecond(){return m_TicksPerSecond;};
    inline float GetDuration(){return m_Duration;};
    inline std::unordered_map<std::string, Bone_Info>* GetBoneIDMap() {return m_Bone_InfoMap;};
    
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
    std::vector<Bone>* GetBoneArray(){return &m_Bones;};
    AssimpNodeData* getRootNode(){return &m_RootNode;};
    
private:
    float m_Duration;
    int m_TicksPerSecond;
    AssimpNodeData m_RootNode;
    // Bone is a class type while Bone_Info is just a struct contain id and
    // offset matrix
    std::vector<Bone> m_Bones;
    std::unordered_map<std::string, Bone_Info>* m_Bone_InfoMap;
};

void showUniformVarValuePerVertex(GLuint* UBO = nullptr, GLuint* programeId = nullptr, Mesh* mesh = nullptr, bool32 showIndices = false,  bool32 showPos = false, bool32 showTexCoords = false,  bool32 showBoneIds = false, bool32 showWeights = false, bool32 showFinalBoneMatrices = false);

// Class wrapper
//extern "C" __declspec(dllexport) Animation* __cdecl CreateAniClass(const char* animationPath, Model_ *model);
//typedef Animation* (__cdecl *AniClassSpawner) (const char *, Model_ *);
//
//extern "C" __declspec(dllexport) void __cdecl DestroysAniClass(Animation *ani);
//typedef void (__cdecl *AniClassSlainer) (Animation* );
//
//extern "C" __declspec(dllexport) void __cdecl ShowInfo(GLuint* UBO, GLuint* programeId, Mesh* mesh, bool32 showIndices, bool32 showPos, bool32 showTexCoords, bool32 showBoneIds, bool32 showWeights, bool32 showFinalBoneMatrices);
//
//typedef void (__cdecl *ShowInfo_) (GLuint*, GLuint*, Mesh*, bool32, bool32, bool32, bool32, bool32, bool32);
//

#define ANIMATION_H
#endif

