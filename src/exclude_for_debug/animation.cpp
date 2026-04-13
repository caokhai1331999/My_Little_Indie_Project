/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "animation.h"

Animation::Animation(const char* animationPath, Model_ *model) {
  // assert throw out the error when 0 is the value

  Assimp::Importer importer;
  m_Bone_InfoMap = new std::unordered_map<std::string, Bone_Info>;
  const aiScene *scene = importer.ReadFile(animationPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_LimitBoneWeights);
  // Still don't understand this part
  // Now I understand this: This line check whether one of these two
  // the scene->mRootNode is NULL or not
  assert(scene && scene->mRootNode);

  // Bone construct based on scene here
  aiAnimation* animation = scene->mAnimations[0];

  m_Duration = animation->mDuration;
  m_TicksPerSecond = animation->mTicksPerSecond;

  ReadHierarchyData(this->m_RootNode, scene->mRootNode);
  ReadMissingBone(animation, model);
};


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

// NOTE: Focus on this
void Animation::ReadMissingBone(const aiAnimation* animation, Model_* model){
// So the mNumChannels is the number of bone
    int size = animation->mNumChannels;
    m_Bones.reserve(size);
    m_Bone_InfoMap->reserve(size);
    //m_Bones.resize(size);

    // Get these properties from model var
    std::unordered_map<std::string, Bone_Info>* boneInfoMap;

    boneInfoMap = model->GetBoneInfoMap();
    int boneCount = model->GetBoneCount();

    //Reading channels (bone engaged in an animation and keyframes)
// BUGs likely to be in here
    for(int i = 0; i < size; i++){
        //TODO: Debug here
        aiNodeAnim *channel = animation->mChannels[i];
        assert(channel);
        std::string boneName = channel->mNodeName.data;

            // Why we have to reapply the boneId
        //if((*boneInfoMap).find(boneName) == (*boneInfoMap).end()){
            //(*boneInfoMap)[boneName].id = boneCount;
            //(boneCount)++;
        //}

//Bone is compensated right here
        m_Bones.push_back(Bone(channel->mNodeName.data, (*boneInfoMap)[channel->mNodeName.data].id, channel));
        //printf("Add bone data to animation container\n");
        }
    m_Bone_InfoMap = boneInfoMap;
};

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src){
    assert(src);
    
    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);

    dest.children.resize(src->mNumChildren);
    dest.children.reserve(src->mNumChildren);
    // as told this line prevent children vector from reallocating after an
    // element is added(which is believed would speed things up)
    for(int i = 0; i < src->mNumChildren; i++){
        AssimpNodeData child;
        ReadHierarchyData(child, src->mChildren[i]);
        dest.children.push_back(child);
    };
};

Animation* __cdecl CreateAniClass(const char *animationPath, Model_ *model) {
    return new Animation(animationPath, model);
};

void __cdecl DestroysAniClass(Animation *ani) {
    delete ani;
}
