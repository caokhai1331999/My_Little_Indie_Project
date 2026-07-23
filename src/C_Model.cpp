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
    bool32 vampire = false;
    if(strcmp(model->name.c_str(), "vampire") == 0){
        vampire = true;
        //printf("being drawn model: %s\n", model->name.c_str());
    }

    if(model != nullptr && programID != nullptr){
        if(model->meshes.size() > 1){
            for(unsigned int i = 0; i < model->meshes.size(); i++){
                 Draw(model->meshes[i], programID);
            }   
        } else {
            Draw(model->meshes[0], programID);
        }

    } else {
        printf("model or programID is NULL\n");
    }
}

internal void Count_Meshes (const aiNode* node, int* total_meshes_number){

    if(node->mNumMeshes > 0){
        (*total_meshes_number) += (int)node->mNumMeshes;
    }
    
    if(node->mNumChildren > 0){
        for(unsigned int i = 0; i < node->mNumChildren; i++){
            Count_Meshes(node->mChildren[i], total_meshes_number);
        }
    }
}


void loadModel_(Win32_OffScreen_Buffer* BackBuffer, Model_* model, string path){

    bool32 vampire = false;

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(path,
  aiProcess_JoinIdenticalVertices  |
  aiProcess_SortByPType|
  aiProcess_FindInvalidData|
  aiProcess_FindInstances|
  aiProcess_OptimizeMeshes|
  aiProcess_CalcTangentSpace       |
  aiProcess_Triangulate            );    

    
/*

*/    //texture_file.C_str()="C:/Users/klove/Documents/repos/GLFW2/Vulkan_Learning_t/build/source/stylised_terrain_tile_1011124259_texture_fbx/stylised_terrain_tile_1011124259_texture.png";

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

    std::string dir = path.substr(0, path.find_last_of('/'));
    model->directory = (char*)dir.c_str();
    if(strcmp(model->name.c_str(), "vampire") == 0){
        vampire = true;
        printf("being drawn model: %s which has %d materials in the scene\n", model->name.c_str(), scene->mNumMaterials);
        model->Texturedirectory = model->directory + "/for_vampire";
    }else{
        model->Texturedirectory = model->directory;
    }

// NODE
    //Count_Meshes(scene->mRootNode, &(model->number_of_meshes));
    processNode(BackBuffer, model, scene->mRootNode, scene);
// MESH
    if((int)model->meshes.size() > 1){
        for(unsigned int i = 0; i < model->meshes.size(); i++){
            setupMesh(model->meshes[i]);
        }
    }else{
            setupMesh(model->meshes[0]);
    }

// MATERIAL Inside mesh
}
// Watch out for this hierarchy
internal void processNode(Win32_OffScreen_Buffer* BackBuffer,  Model_* model, aiNode* node, const aiScene* scene){
    // process all the node'scene meshes (if any)
    aiMesh* mesh;
    Mesh* spawned_mesh;
    if(node->mNumMeshes > 0){
        for(unsigned int i = 0; i < node->mNumMeshes; i++){
            mesh = scene->mMeshes[node->mMeshes[i]];
            spawned_mesh = model->processMesh(BackBuffer, mesh, scene);
            //spawn_mesh = (Mesh*)push_size_(sizeof(*spawn_mesh), BackBuffer->state->memory_sentinal, BackBuffer->mutex);
            model->meshes.push_back(spawned_mesh);

            //model->meshes  = (mesh* )push_size_(sizeof(*spawned_mesh), &BackBuffer->state->memory_sentinal, BackBuffer->state->mutex);
            //CopyMemory(model->meshes, spawned_mesh, spawned_mesh->size);
            //Then how to access member of model->meshes.????
            //and how to iterate them to draw???
        }
    }

// then do the same for each of its children
    if(node->mNumChildren > 0){
        for(unsigned int i = 0; i < node->mNumChildren; i++){
            processNode(BackBuffer, model, node->mChildren[i], scene);
        }
    }
}

inline Mesh* Model_::processMesh(Win32_OffScreen_Buffer* BackBuffer, const aiMesh* mesh, const aiScene* scene){
    
    vector<unsigned int>indices;
    vector<Texture>textures;
    vector<Vertex>vertices;
//
  // unsigned int* indices;
  // Texture* textures;
  // Vertex* vertices;
    
// Current Change 02.07.26
    Vertex vertex = {};
    glm::vec2 vec = glm::vec2(0.0f);

    if(mesh->mNumVertices > 0){
        for(unsigned int i = 0; i < mesh->mNumVertices; i++){
            vertex = {};
            SetVertexBoneDataToDefault(&vertex);

            // For skeletal animation
            vertex.Position = AssimpGLMHelpers::GetGLMVec(mesh->mVertices[i]);
            vertex.Normal = AssimpGLMHelpers::GetGLMVec(mesh->mNormals[i]);
            vertex.Tangent = AssimpGLMHelpers::GetGLMVec(mesh->mTangents[i]);
        
            if(mesh->mTextureCoords[0]){
                vec = AssimpGLMHelpers::GetGLMVec(mesh->mTextureCoords[0][i]);
                vertex.TexCoords = vec;            
            } else {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }
            //
            //
            //
            //
            vertices.push_back(vertex);
            // Vertices = (Vertex*)push_size_(size_of(vertex), BackBuffer->game_state->memory_sentinel, BackBuffer->mutex);
            // CopyMemory(Vertices, &Vertex);
            // Process VERTEX Position, Normal, Texure Coordinates
        }
    }

    // Process INDICES(order of mesh vertex(which contain Position, Norm, TexCoords))
    aiFace face;
    if(mesh->mNumFaces > 0){
        for(unsigned int i = 0; i < mesh->mNumFaces; i++){
            face = mesh->mFaces[i];
            for(unsigned int j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[j]);
            };
        }
    }

            // Texture is simple a type which recall texture from given path
    
    //if(mesh->mMaterialIndex > 0){
        // Process MATERIAL
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // Texture is simple a type which recall texture from given path
        if(material != nullptr){
            vector<Texture> diffuseMaps = loadMaterialTextures(this, material, aiTextureType_DIFFUSE, "material.texture_diffused", scene);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            //printf("Loading diffuse texture to mesh\n");

            vector<Texture> specularMaps = loadMaterialTextures(this, material, aiTextureType_SPECULAR, "material.texture_specular", scene);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());    

            vector<Texture> normalMaps = loadMaterialTextures(this, material, aiTextureType_NORMALS, "material.texture_normal", scene);
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());    
            //printf("Loading specular texture to mesh\n");
            //Load diffuse and specular map to texture            
                  } else {
            printf("material is null\n");
        }
    //} else {
        //printf("Somehow mesh have no material that means no texture; index: %d\n", mesh->mMaterialIndex);
    //}
        Model_::ExtractBoneWeightForVertices(mesh, &vertices);
        //return Mesh(vertices, indices, textures);
        // This work
        begin_ticket_mutex(&BackBuffer->ticket);
        Mesh* OutPut = new Mesh(&vertices, indices, textures);
        end_ticket_mutex(&BackBuffer->ticket);

        return OutPut;
}

internal vector<Texture> loadMaterialTextures(Model_* model, aiMaterial* mat, aiTextureType type, string typeName, const aiScene* scene){
    vector<Texture>textures;
// Manually load normal texture here
    if(strcmp(typeName.c_str(), "material.texture_normal") == 0){

        if(first_normal_time){
            Texture texture_;
            // Now search for the file
            std::string filename_;
            std::string pattern = model->Texturedirectory+"/*normal*";
            intptr_t searchAgent = {};
            _finddata64i32_t Normal_Texture_File = {};

            searchAgent = _findfirst(pattern.c_str(), &Normal_Texture_File);
            if(searchAgent != -1L){
                printf("Succeed searching out the file in folder %s\n", model->directory.c_str());
                filename_ = Normal_Texture_File.name;
                texture_.id = TextureFromFile(filename_.c_str(), model->Texturedirectory);
                //printf("texture path is:%s\n",model->directory.c_str());
                texture_.type = typeName;
                std::string path_ = model->directory +"/"+ filename_;
                texture_.path = path_.c_str();
                textures.push_back(texture_);
                printf("Start loading texture from external file , model path is :%s \n",model->directory.c_str());
                //model->loaded_textures.push_back(texture_);                    
            } else {
                printf("Couldn't find out the file with that pattern: %s\n", pattern.c_str());
            }
            first_normal_time = !first_normal_time;
        }
 
    } else {
        aiString str;
        std::string strr;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++){
            strr = {str.C_Str()};
            if(mat->GetTexture(type, i, &str) == aiReturn_SUCCESS){
                //if(std::strcmp(str.C_Str(),"specular.jpg") == 0){
                //if(string_contain(&strr, "specular")) {
                    //if(first_specular_time){
                        //printf("Texture successfully retrieved %s\n", str.C_Str());
                        //first_specular_time = false;
                    //};
                //} else if(string_contain(&strr, "diffuse")) {
                    //if(first_diffuse_time){
                        //printf("Texture successfully retrieved %s\n", str.C_Str());
                        //first_diffuse_time = false;
                    //};                    
                //}
                /*    else if (string_contain(&strr, "normal")){                
                      }*/

            } else {
                //if(first_diffuse_time){
                printf("Texture not found or error occured\n");            
                //first_diffuse_time = false;
                //};
            };
            //printf("Texture path is: %s\n", str.C_Str());
            // IF mat is null
            // manually load the texture to mat from png right here
            bool skip = false;
            for(unsigned int j = 0; j <model->loaded_textures.size(); j++){

                if(first_time){
                    printf("Loading texture from model\n");
                    first_time = false;
                }

                // If the path length equal (the texture existed)
                if(!std::strcmp(model->loaded_textures[j].path.data(), str.C_Str())){
                    //model->loaded_textures.push_back(model->loaded_textures[j]);
                    textures.push_back(model->loaded_textures[j]);
                    skip = true;
                    break;
                }

            };
        
            if(!skip){
                Texture texture;
                //This is wrong in fbx case : How to fix this???
                if(scene->mNumTextures == 0){
                    texture.id = TextureFromFile(str.C_Str(), model->Texturedirectory);
                    //printf("texture path is:%s\n",model->directory.c_str());
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    printf("Start loading texture from external file , model Texture path is :%s\n",model->Texturedirectory.c_str());
                    model->loaded_textures.push_back(texture);
                }else {
                    //for(unsigned int i = 0; i < scene->mNumTextures; i++){

                    /*
                      NOTE: Try to assign scene texture to model container
                      haven't test it yet

                      Things is more complicated than I thought. Actually we load
                      embbedded one in wrong way. We have to load the texture
                      matched with the mat index with is produced by atoi
                      (mat->getTexture().data + 1)
                    */

                    printf("Start loading texture from embbedded texture, path is:%s\n",(char* )str.C_Str());
                    // Bug here
                    texture.id = TextureFromMemory(scene, model->directory, false, &str);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    model->loaded_textures.push_back(texture);
                };
                //=======================
            };                    
                   
        }
        // This part is responsible for loading image internally or from external file
        //}
    }
    return textures;        
}


//void Model_::ExtractBoneWeightForVertices(const aiMesh* mesh, std::vector<Vertex>&vertices){
inline void Model_::ExtractBoneWeightForVertices(const aiMesh* mesh, std::vector<Vertex>*vertices){

    std::unordered_map<std::string, Bone_Info>* mBoneInfoMap = this->m_BoneInfoMap;
    unsigned int boneID = -1;
    std::string boneName;
    Bone_Info newboneinfo ;

    if(mesh->mNumBones > 0)
    {    for(int boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++){
            boneID = -1;
            //What is mName
            boneName = mesh->mBones[boneIndex]->mName.C_Str();
//If no elements was found!!!

            if(mBoneInfoMap->find(boneName) == mBoneInfoMap->end()){
                newboneinfo = {};
// On the way of learning here
                boneID = m_BoneCounter;

                newboneinfo.id = m_BoneCounter;
                newboneinfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);

                (*mBoneInfoMap)[boneName] = newboneinfo;
                m_BoneCounter++;
            }else{
                boneID = (*mBoneInfoMap)[boneName].id;
            }

            assert(boneID != -1);
            //what exactly weights's type is
            aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
            int numWeights = mesh->mBones[boneIndex]->mNumWeights;

            int vertexId;
            float weight;

            for(int weightIndex = 0; weightIndex < numWeights; weightIndex++){
//NOTE:  How do they know that the vertexId is matched with the boneID
                vertexId = weights[weightIndex].mVertexId;
                weight = weights[weightIndex].mWeight;

                assert(vertexId <= vertices->size());
                SetVertexBoneData(&(*vertices)[vertexId], boneID, weight);
            }

        }
    }
    // assert check whether the argument is unequal to 0 or 
};



internal unsigned int TextureFromFile(const char *path, const string &directory, bool gamma){
  std::string filename = string(path);
  //May the file path is insufficient here
  filename = directory + '/' + filename;
    unsigned int textureID;
    //printf("texture file path: %s\n", filename.c_str());
    int width, height, nrComponents;

    //stbi_set_flip_vertically_on_load(true);
        // This used stbi_load to load image

    // TODO: find out why stbi load failed to load image data
    // and create alternative function if there is no way to fix this

     unsigned char *data = (unsigned char*)stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

     if (data)
        {
            glGenTextures(1, &textureID);

            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glActiveTexture(GL_TEXTURE0+textureID);

            printf("Texture ID: %d, load from:%s \n", textureID, filename.c_str());
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
            glActiveTexture(GL_TEXTURE0);            
            glBindTexture(GL_TEXTURE_2D, GL_TEXTURE0);            
        } else {
            std::cout << "Texture failed to load at path: " << filename.c_str() << std::endl;
            stbi_image_free(data);
        }
    return textureID;
}

internal unsigned int TextureFromMemory(const aiScene* scene, const string &directory, bool gamma, aiString* path){
    // IF the embedded texture is NULL
    //string filename = string(path->C_Str());
    //filename = directory + '/' + filename;

    //printf("Embedded texture index is %s\n", path->data);
    //char TextIndex[2] = {'*', (char)(textIndex + 48)};

    int textIndex = atoi((path->data)+1);
    int width, height, nrComponents;
    size_t size;
    unsigned int textureID;

    const aiTexture* tex = nullptr;
    tex = new aiTexture;

    //tex = scene->GetEmbeddedTexture(path->C_Str());
    tex = scene->mTextures[textIndex];

    bool switched = false;

    while(!tex){
        printf("Switch fetching texture from arry method\n");
        if(!switched){
            tex = scene->mTextures[textIndex];
            switched = true;
        }else{
            tex = scene->GetEmbeddedTexture(path->C_Str());
            switched = false;
        }
    }

    char* img = nullptr;    

    if(tex){
        printf("We got a texture\n");
        glGenTextures(1, &textureID);
        
        printf("Texture ID in memory texture loading: %d\n", textureID);
        stbi_set_flip_vertically_on_load(true);

        if(tex->pcData){
            //Wrong here

            if(tex->mHeight == 0){
                //NOTE: if the texture is compressed
                size = tex->mWidth;
                img = new char;
                // This is where thing went wrong
                img = (char*)stbi_load_from_memory((unsigned char*)tex->pcData , size, &width, &height, &nrComponents, 0);
                printf("image specs: w: %d, h:%d, size:%d\n", tex->mWidth, tex->mHeight, (int)size);

            }else{
                width = tex->mWidth;
                height = tex->mHeight;

                img = new char[4*width*height];
                memcpy(img ,tex->pcData , 4*width*height);

                nrComponents = 4;
            }

            if(img!=NULL)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;


                if(width%4 !=0){
                    printf("align pixels\n");
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                }

                glActiveTexture(GL_TEXTURE0+textureID);
                glBindTexture(GL_TEXTURE_2D, textureID);
                printf("width is: %d, height is:%d, nrComponent is:%d \n", width, height, nrComponents);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(img);
                img = nullptr;
                
            } else {
                printf("Got no data from Scene embedded texture\n");
            }
            //delete data;
            //data = nullptr;
        } else {
            printf("Data from aiTexture is NULL\n");
        }
    } else {
        printf("Got no embedded texture from Scene\n");
    }
    
    return textureID;
}
