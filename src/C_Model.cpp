/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai(Casey Muratori's disciple) $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */
#include "C_Model.h"

using namespace std;

void DDraw(Model_* model, GLuint* programID){
    for(unsigned int i = 0; i < model->meshes.size(); i++){
        Draw(&model->meshes[i], programID);
    }
}

void loadModel_(Model_* model, string path){
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
    aiProcess_CalcTangentSpace       |
    aiProcess_Triangulate            |
    aiProcess_JoinIdenticalVertices  |
    aiProcess_SortByPType);    
    //texture_file.C_str()="C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_t/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.png";

    //const char* path_= "C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_t/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.png";
    //aiString texture_file_ = path_;
    //aiMaterial* material = scene->mMaterials[0];
    //material->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texture_file_);

    //if(mTextures_ != NULL){
       //printf("Texture ID:%d\n", scene->GetEmbeddedTexture(texture_file_.c_str()));
    //} else {
        //printf("Embedded texture is NULL\n");
        //cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<endl;
    //};


    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        cout<<"ERROR::ASSIMP::"<<importer.GetErrorString()<<endl;
    }

    model->directory = path.substr(0, path.find_last_of('/'));
// NODE
    processNode(model, scene->mRootNode, scene);
// MESH
    for(unsigned int i = 0; i < model->meshes.size(); i++){
        setupMesh(&model->meshes[i]);
    }
// MATERIAL Inside mesh
}

void processNode(Model_* model, aiNode* node, const aiScene* scene){
    // process all the node'scene meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        model->meshes.push_back(processMesh(model, mesh, scene));
    }
    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++){
        processNode(model, node->mChildren[i], scene);
    }
}

Mesh processMesh(Model_* model, aiMesh* mesh, const aiScene* scene){
    vector<unsigned int>indices;
    vector<Texture>textures;
    vector<Vertex>verticles;

    Vertex vertex;

    for(unsigned int i = 0; i < mesh->mNumVertices; i++){
        glm::vec3 vector;
        // Verticle is position
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        if(mesh->mTextureCoords[0]){
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;            
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        verticles.push_back(vertex);
        // Process VERTEX Position, Normal, Texure Coordinates
    }

    // Process INDICES(order of mesh vertex(which contain Position, Norm, TexCoords))
    for(unsigned int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        };
    }

            // Texture is simple a type which recall texture from given path
    
    //if(mesh->mMaterialIndex > 0){
            // Process MATERIAL
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // Texture is simple a type which recall texture from given path
        vector<Texture> diffuseMaps = loadMaterialTextures(model, material, 
                                                           aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        vector<Texture> specularMaps = loadMaterialTextures(model, material, 
                                                            aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());    

        //} else {
            //printf("Somehow mesh have no material that means no texture; index: %d\n", mesh->mMaterialIndex);
        //}

        return Mesh(verticles, indices, textures);
}

vector<Texture> loadMaterialTextures(Model_* model, aiMaterial* mat, aiTextureType type, string typeName){
    vector<Texture>textures ;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){

        aiString str;
        printf("Texture path is: %s\n", str.C_Str());
        mat->GetTexture(type, i, &str);
        // IF mat is null
        // manually load the texture to mat from png right here
        bool skip = false;
        for(unsigned int j = 0; j <model->loaded_textures.size(); j++){
            if(!std::strcmp(model->loaded_textures[j].path.data(), str.C_Str())){
                model->loaded_textures.push_back(model->loaded_textures[j]);
                skip = true;
                break;
            }
        };
        if(!skip){
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), model->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            model->loaded_textures.push_back(texture);
        }
    };
    return textures;
}

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma){
    string filename = string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    stbi_set_flip_vertically_on_load(true);
    // This used stbi_load to load image
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    
    
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    return textureID;    
}
