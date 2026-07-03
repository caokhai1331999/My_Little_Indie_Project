#if !defined(C_MODEL_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#define C_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "C_Mesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <time.h>

static bool first_diffuse_time = true;
static bool first_specular_time = true;
global_variable bool first_normal_time = true;
static bool first_time = true;

class Model_{
//private:
public:
    // Model data
    //std::vector<Mesh>meshes;
    std::vector<Mesh*>meshes;
    int number_of_meshes;
    std::string directory;
    std::string Texturedirectory;
    vector<Texture>loaded_textures;
    std::unordered_map<std::string, Bone_Info>* m_BoneInfoMap;
    unsigned int m_BoneCounter;
    bool gammaCorrection;
    std::string name;
    
    Model_(bool gamma = false, std::string* model_name = nullptr)
            : gammaCorrection(gamma), name(*model_name) {
      m_BoneCounter = 0;
      m_BoneInfoMap = nullptr;
      m_BoneInfoMap = new std::unordered_map<std::string, Bone_Info>;
    };

    void ExtractBoneWeightForVertices(const aiMesh* mesh, std::vector<Vertex>&vertices);

    //Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
    Mesh* processMesh(const aiMesh* mesh, const aiScene* scene);
    std::string* GetModelDir(){return &directory;};
// About skeletal animation
    std::unordered_map<std::string, Bone_Info>*GetBoneInfoMap(){return m_BoneInfoMap;};
    unsigned int GetBoneCount() { return m_BoneCounter;};
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
unsigned int TextureFromMemory(const aiScene* scene, const string &directory, bool gamma = false, aiString* path = nullptr);
void Count_Meshes (const aiNode* Node = nullptr, int total_meshes_number = 0);
void loadModel_(Model_*model, string path);
void processNode(Model_* model, aiNode* node, const aiScene* scene);
vector <Texture>loadMaterialTextures(Model_* model, aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);

void DDraw(Model_* model = nullptr, GLuint* programID = nullptr);
#endif
