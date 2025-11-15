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

struct Model_{
    // Model data
    vector<Mesh>meshes;
    string directory;
    vector<Texture>loaded_textures;
    bool gammaCorrection;
    Model_(const char *path = nullptr, bool gamma = false):gammaCorrection(gamma){
    };    
};

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);
void loadModel_(Model_*model, string path);
void processNode(Model_* model, aiNode* node, const aiScene* scene);
Mesh processMesh(Model_* model, aiMesh* mesh, const aiScene* scene);
vector <Texture>loadMaterialTextures(Model_* model, aiMaterial* mat, aiTextureType type, string typeName);
void DDraw(Model_* model = nullptr, GLuint* programID = nullptr);
#endif
