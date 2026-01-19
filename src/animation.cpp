
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animation.h"
#include <unordered_map>

Bone* Animation::FindBone(const std::string& name){
  auto iter = find_if(m_Bones.begin(), m_Bones.end(),
                      [&](const Bone& Bone)
                      {
                          return *(Bone.GetBoneName()) == name; //line 15
                      });//lambda to findout address of bone that have the same name of given name
  
  
  if(iter == m_Bones.end()){
        return nullptr;
    //return address of member that iter is pointing to
    }else {return &(*iter);}
};

void Animation::ReadMissingBone(const aiAnimation* animation, Model_* model){
    int size = animation->mNumChannels;
// Get these properties from model var
    std::unordered_map<std::string, Bone_Info>* boneInfoMap;
    boneInfoMap = model->GetBoneInfoMap();
    int boneCount = model->GetBoneCount();

    //Reading channels (bone engaged in an animation and keyframes)

    for(int i = 0; i < size; i++){
        aiNodeAnim* channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if((*boneInfoMap).find(boneName) == (*boneInfoMap).end()){
            (*boneInfoMap)[boneName].id = boneCount;
            (boneCount)++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data, (*boneInfoMap)[channel->mNodeName.data].id, channel));
    }
    m_Bone_InfoMap = (*boneInfoMap);
};

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src){
    assert(src);

    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.children.resize(src->mNumChildren);
    // as told this line prevent children vector from reallocating after an
    // element is added(which is believed would speed things up)
    dest.children.reserve(src->mNumChildren);
    for(int i = 0; i < src->mNumChildren; i++){
        AssimpNodeData child;
        ReadHierarchyData(child, src->mChildren[i]);
        dest.children.push_back(child);
    };
};

