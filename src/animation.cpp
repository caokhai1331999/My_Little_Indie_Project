/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animation.h"

Bone* Animation::FindBone(const std::string& name){
    auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& Bone){return Bone.GetBoneName() == name;});//lambda to findout address of bone that have the same name of given name
    if(iter == m_Bones.end())return nullptr;
    //return address of member that iter is pointing to
    else return &(*iter);
};

void Animation::ReadMissingBone(const aiAnimation* animation, const Model* model){
    int size = animation->mNumChannels;
// Get these properties from model var
    std::map<std::string, Bone_Info> boneInfoMap = model->GetBoneInfoMap();
    int& boneCount = model->GetBoneCount();

    //Reading channels (bone engaged in an animation and keyframes)

    for(int i = 0; i < size; i++){
        Channel* channel = animation->mChannels[i];
        std::string boneName = animation->mNodeName.data;

        if(boneInfoMap.find(boneName) == boneInfoMap.end()){
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
    }
    m_BoneInfoMap = boneInfoMap;
};

void Animation::ReadHierarchyData(const AssimpNodeData& dest, const aiNode* src){
    assert(src);

    Dest.name = src->mName.Data;
    Dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    Dest.childrenCount = src->mChildrenCount;

    for(int i = 0; i < src->mChildrenCount; i++){
        AssimpNodeData child = new AssimpNodeData;
        ReadHierarchyData(child, src->mChildren[i]);
        dest.children.push_back(child);
    };
};

