#if !defined(C_MODEL_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#define C_MODEL_H

#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "assimp_glm_helpers.h"
#include "C_Mesh.h"

class Model_{
//private:
public:
    // Model data
    vector<Mesh>meshes;
    std::string directory;
    std::string Texturedirectory;
    vector<Texture>loaded_textures;
    std::map<std::string, Bone_Info>* m_BoneInfoMap;
    int m_BoneCounter;
    bool gammaCorrection;

    Model_(const char *path = nullptr, bool gamma = false)
        : gammaCorrection(gamma) {
      m_BoneCounter = 0;
      m_BoneInfoMap = nullptr;
      m_BoneInfoMap = new std::map<std::string, Bone_Info>;
    };

    void SetVertexBoneData(Vertex* vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
    Mesh processMesh(Model_* model, aiMesh* mesh, const aiScene* scene);
    string* GetModelDir(){return &directory;};
// About skeletal animation
    std::map<std::string, Bone_Info>*GetBoneInfoMap(){return m_BoneInfoMap;};
    int GetBoneCount() { return m_BoneCounter;};
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
unsigned int TextureFromMemory(const aiScene* scene, const string &directory, bool gamma = false, aiString* path = nullptr);
void loadModel_(Model_*model, string path);
void processNode(Model_* model, aiNode* node, const aiScene* scene);
vector <Texture>loadMaterialTextures(Model_* model, aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene);
void SetVertexBoneDataToDefault(Vertex* vertex);

void Draw(Mesh* mesh = nullptr, GLuint* programID = nullptr);

void DDraw(Model_* model = nullptr, GLuint* programID = nullptr);
#endif
