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

static bool first_diffuse_time = true;
static bool first_specular_time = true;
static bool first_time = true;

class Model_{
//private:
public:
    // Model data
    vector<Mesh>meshes;
    std::string directory;
    std::string Texturedirectory;
    vector<Texture>loaded_textures;
    std::unordered_map<std::string, Bone_Info>* m_BoneInfoMap;
    int m_BoneCounter;
    bool gammaCorrection;

    Model_(const char *path = nullptr, bool gamma = false)
        : gammaCorrection(gamma) {
      m_BoneCounter = 0;
      m_BoneInfoMap = nullptr;
      m_BoneInfoMap = new std::unordered_map<std::string, Bone_Info>;
    };

    void ExtractBoneWeightForVertices(const aiMesh* mesh, std::vector<Vertex>&vertices);

    Mesh processMesh(const aiMesh* mesh, const aiScene* scene);
    std::string* GetModelDir(){return &directory;};
// About skeletal animation
    std::unordered_map<std::string, Bone_Info>*GetBoneInfoMap(){return m_BoneInfoMap;};
    int GetBoneCount() { return m_BoneCounter;};
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
unsigned int TextureFromMemory(const aiScene* scene, const string &directory, bool gamma = false, aiString* path = nullptr);
void loadModel_(Model_*model, string path);
void processNode(Model_* model, aiNode* node, const aiScene* scene);
vector <Texture>loadMaterialTextures(Model_* model, aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);


void Draw(Mesh* mesh = nullptr, GLuint* programID = nullptr);
void DDraw(Model_* model = nullptr, GLuint* programID = nullptr);
#endif
