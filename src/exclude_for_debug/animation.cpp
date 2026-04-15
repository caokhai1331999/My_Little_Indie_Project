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
  const aiScene *scene = importer.ReadFile(animationPath, aiProcess_CalcTangentSpace | aiProcess_Triangulate);

  //| aiProcess_LimitBoneWeights
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
                      });//lambda to find out address of bone that have the same name of given name
  
  
  if(iter == m_Bones.end()){
        return nullptr;
    //return address of member that iter is pointing to
    }else {return &(*iter);}
};

// NOTE: Focus on this
void Animation::ReadMissingBone(const aiAnimation* animation, Model_* model){
// So the mNumChannels is the number of bone
    int size = animation->mNumChannels;

    m_Bones.reserve(100);
    m_Bone_InfoMap->reserve(100);
    //m_Bones.resize(size);

    // Get these properties from model var
    std::unordered_map<std::string, Bone_Info>* boneInfoMapForAni;
    boneInfoMapForAni = new std::unordered_map<std::string, Bone_Info>;
    boneInfoMapForAni->reserve(100);

    std::unordered_map<std::string, Bone_Info>* ModelboneInfoMapClone;

    ModelboneInfoMapClone = model->GetBoneInfoMap();
    unsigned int ExtraBoneCount = model->GetBoneCount();

    // Not any node is affected by this animation we have to choose between them
    
    //Reading channels (bone engaged in an animation and keyframes)
// BUGs likely to be in here
    for(int i = 0; i < size; i++){
        //TODO: Debug here
        aiNodeAnim *channel = animation->mChannels[i];
        assert(channel);
        std::string boneName = channel->mNodeName.data;

            // Why we have to reapply the boneId

//Bone is compensated right here it find the bone just in case it exist
//of the boneInfo map
        if((*ModelboneInfoMapClone).find(boneName) == (*ModelboneInfoMapClone).end()){
            Bone_Info newbone;
            newbone.id = ExtraBoneCount;
            newbone.offset = glm::mat4(1.0f);

            (*boneInfoMapForAni)[boneName] = newbone;
            ExtraBoneCount++;
        }else{
            //This bracket will show bug which indicate the mismatch between
            //boneInfo_map and m_bones
            (*boneInfoMapForAni)[boneName] = (*ModelboneInfoMapClone)[boneName];
            //Add bone if channel Name exist in boneInfo_Map
            m_Bones.push_back(Bone(channel->mNodeName.data, (*boneInfoMapForAni)[channel->mNodeName.data].id, channel));
        }
//The ID have to be matched with the give channel
        //printf("Add bone data to animation container\n");
        }
    m_Bone_InfoMap = boneInfoMapForAni;
};

void Animation::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src){
    assert(src);
    
    dest.name = src->mName.data;
    dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);

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
