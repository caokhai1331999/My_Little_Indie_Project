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
  const aiScene *scene = importer.ReadFile(animationPath,
  aiProcess_CalcTangentSpace |
  aiProcess_Triangulate);

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

    m_Bones.reserve(size);
    m_Bone_InfoMap->reserve(size);
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

//Animation* __cdecl CreateAniClass(const char *animationPath, Model_ *model) {
    //return new Animation(animationPath, model);
//};
//
//void __cdecl DestroysAniClass(Animation *ani) {
    //delete ani;
//}

void showUniformVarValuePerVertex(GLuint* UBO, GLuint* programeId, Mesh* mesh, bool32 showIndices, bool32 showPos, bool32 showTexCoords, bool32 showBoneIds, bool32 showWeights, bool32 showFinalBoneMatrices){
    
//We already bind the specific VAO before
    int VertexSizeforVec4 = MAX_BONE_INFLUENCE * 100;
    int VertexSizeforVec3 = 3 * 100;
    glBindVertexArray(mesh->VAO);

    GLint is_moving;
    std::string moving_flag = "is_moving";
    GLint location_ = -1;
    printf("moving flag location is %d\n", (int)location_);
    location_ = glGetUniformLocation((*programeId), (GLchar*)moving_flag.c_str());
    printf("programme id:%d\n", (*programeId));    
    if((int)location_ != -1){
        glGetUniformiv(*programeId, location_, &is_moving);
        printf("moving flag location is %d\n", location_);
        printf("moving flag is %s\n", is_moving==1?"true":"false");
    } else {
        printf("Moving_flag location is invalid\n");
    };

    for(int k = 0; k < 52; k++){

        if(showIndices){
            GLuint index;   
            glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, k*sizeof(GLuint), sizeof(GLuint), &index);
            printf("index of %d vertex is: %d\n", k, index);
        }
        
        if(showPos){
        glm::vec3 readbackPositions;        
                GLintptr offset = (k*sizeof(struct Vertex));
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::vec3), &readbackPositions);
                printf("Position ID %d:", k);
                std::cout<<glm::to_string(readbackPositions);
                printf("\n");
            }
        
        if(showTexCoords){
        glm::vec2 readbackTexCoords;        
                GLintptr offset = (k*sizeof(struct Vertex)) + offsetof(struct Vertex, TexCoords);
                glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(glm::vec2), &readbackTexCoords);
                printf("TexCoords ID %d:", k);
                std::cout<<glm::to_string(readbackTexCoords);
                printf("\n");
            }

        
    if(showBoneIds){
        int readbackBoneIDs[4];
        //readbackBoneIDs = new int (VertexSizeforVec4);
        if(readbackBoneIDs!=nullptr){
            GLintptr offset = (k*sizeof(Vertex)) + offsetof(struct Vertex, m_BoneIDs);
            glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(int)*MAX_BONE_INFLUENCE, readbackBoneIDs);
            printf("Bone IDs per vertex %d is: ", k);
            printf("[%d, %d, %d, %d] ", readbackBoneIDs[0], readbackBoneIDs[1], readbackBoneIDs[2], readbackBoneIDs[3]);
            printf("\n");
        }
    }

    if(showWeights){
        float readbackWeights[4];
        //readbackWeights = new float (VertexSizeforVec4);
        if(readbackWeights != nullptr){
            GLintptr offset = sizeof(Vertex)*k + offsetof(struct Vertex, m_Weights);
            glGetBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float)*MAX_BONE_INFLUENCE, readbackWeights);
            if(readbackWeights[0]!=0.0f){
                printf("Bone weights per vertex %d: ", k);
                printf("[%f, %f, %f, %f]", readbackWeights[0], readbackWeights[1], readbackWeights[2], readbackWeights[3]);
                printf("\n");
            } else {
                continue;   
            }                
        }
    }
 }

    if(showFinalBoneMatrices) {

        //GLfloat matVal[16];
        GLfloat* matVal_;
        std::string matrixName_;
        glUseProgram(*programeId);

        //char index[1];
        //char indexx[2];
        GLint maxUniforms;

        for(int k = 0; k < 52; k++){
            matrixName_ = "finalBone.finalBoneMatrices[]";


                //if(k<10){
                    //sprintf(index, "%d", k);
                    //matrixName_.insert(matrixName_.size()-1, index);
                //} else {
                    //sprintf(indexx, "%d", k);
                    //matrixName_.insert(matrixName_.size()-1, indexx);                                     
                //}
//
            //printf("matrix name: %s, programmeID: %d\n", matrixName_.c_str(), *programeId);

            //GLint matrixLocation = glGetUniformLocation(*programeId,                                       (GLchar*)matrixName_.c_str());

            //printf("location of current finalbone matrices: %d\n", matrixLocation);
            
//NOTE: This one is for uniform buffer
            GLint matrixLocation = glGetUniformBlockIndex(*programeId,                                       (GLchar*)matrixName_.c_str());
//
            glBindBuffer(GL_UNIFORM_BUFFER, *UBO);

// 2. Map the buffer memory
            void* ptr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat)*16*52, GL_MAP_READ_BIT);

// 3. Read data using the offset obtained earlier
            matVal_ = (float*)((char*)ptr + (k*sizeof(GLfloat)*16));
            glUnmapBuffer(GL_UNIFORM_BUFFER);
// Read *member1Data...

// 4. Unmap the buffer

            //if(matrixLocation != -1){
                //glGetUniformfv(*programeId, matrixLocation, matVal);

                if(matVal_[0]!=0.0f){
                    printf("\nBone Matrix index %d is:", k);
                    for(int i = 0 ; i < 16; i++){
                        if(i==0||i==4||i==8||i==12){
                            i==0?printf("["):printf(", [");
                        }
                        (i==0||i==4||i==8||i==12)?printf("%f", matVal_[i]):printf(", %f", matVal_[i]);
                        if(i==3||i==7||i==11||i==15){
                            printf("]");
                        }
                    };
                    printf("\n");                                 
                }
            //} else {
                //GLenum error = glGetError();
                //if(error == GL_INVALID_VALUE){
                    //printf("ProgramID is not a value generated by OPENGL\n");
                //} else if(error == GL_INVALID_OPERATION){
                    //printf("ProgramID is not an object or successfully linked\n");                            
                //};                
            //}

//So function failed to retrieved matrix from shader

        }
    }
};

/*
void __cdecl ShowInfo(GLuint* UBO, GLuint* programeId, Mesh* mesh, bool32 showIndices, bool32 showPos, bool32 showTexCoords, bool32 showBoneIds, bool32 showWeights, bool32 showFinalBoneMatrices){

    if(wglGetCurrentContext() != NULL){
        bool success = false;
        success = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
        assert(success);
        printf("Succeed reloading gl function pointers\n");        
    } else{
        printf("The gl context is NULL in this block\n");
    }

    showUniformVarValuePerVertex(UBO, programeId, mesh, showIndices, showPos, showTexCoords, showBoneIds, showWeights, showFinalBoneMatrices);
};
 */
