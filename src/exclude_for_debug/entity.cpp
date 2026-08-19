/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "entity.h"

//-----------------For_Debugging-------------------------
texture_group load_textures_in_folder(File_Manager* folder_looker, const char* folder_path_){
    // NOTE: first of all, all we get from the cFileName is just the name of the folder not the path;
    File_Manager texture_looker;
    std::string folder_path = folder_path_;
    std::string file_path;

    int width, height, nrComponents;
    GLenum format;

    std::string name;
    texture_group textures_of_folder = {};

    file_path = folder_path + "/*";
    texture_looker.handle = FindFirstFileA(file_path.c_str(), &texture_looker.find_data);
    bool32 load_succeeded = true;
    //if(texture_looker.handle != INVALID_HANDLE_VALUE){
    //(GetLastError() != ERROR_NO_MORE_FILES) && 
        while(load_succeeded){
            if(!(texture_looker.find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)){
                file_path.clear();
                file_path = folder_path + "/" + texture_looker.find_data.cFileName;
                //now search for file
                if((!(texture_looker.find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) && (string_contain(&file_path, "."))){
                    if(string_contain(&file_path, "normal")){
                        // load file contain here;
                        // move next
                        unsigned char* normal_map_content = (unsigned char*)stbi_load(file_path.c_str(), &width, &height, &nrComponents, 0);

                        if (nrComponents == 1)
                            format = GL_RED;
                        else if (nrComponents == 3)
                            format = GL_RGB;
                        else if (nrComponents == 4)
                            format = GL_RGBA;

                        glGenTextures(1, &textures_of_folder.normal_map);
                        glBindTexture(GL_TEXTURE_2D, textures_of_folder.normal_map);
                        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, normal_map_content);
                        stbi_image_free(normal_map_content);
                    }

// so the argv[0] is the name of program or the path containing it.

                    else if (string_contain(&file_path, "diffuse"))
                        /* if file's name contain "diffuse" in name*/
                    {
                        // load file contain here;
                        unsigned char* diffuse_map_content = (unsigned char*)stbi_load(file_path.c_str(), &width, &height, &nrComponents, 0);

                        if (nrComponents == 1)
                            format = GL_RED;
                        else if (nrComponents == 3)
                            format = GL_RGB;
                        else if (nrComponents == 4)
                            format = GL_RGBA;

                        glGenTextures(1, &textures_of_folder.diffused_map);
                        glBindTexture(GL_TEXTURE_2D, textures_of_folder.diffused_map);
                        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, diffuse_map_content);
                        stbi_image_free(diffuse_map_content);
                    }

                    else if(string_contain(&file_path, "specular")){
                        // load file contain here;
                        unsigned char* specular_map_content = (unsigned char*)stbi_load(file_path.c_str(), &width, &height, &nrComponents, 0);

                        if (nrComponents == 1)
                            format = GL_RED;
                        else if (nrComponents == 3)
                            format = GL_RGB;
                        else if (nrComponents == 4)
                            format = GL_RGBA;

                        glGenTextures(1, &textures_of_folder.specular_map);
                        glBindTexture(GL_TEXTURE_2D, textures_of_folder.specular_map);
                        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, specular_map_content);
                        stbi_image_free(specular_map_content);
                    }
                }
            }
            // TODO: Remember to free stb_image load data;
            load_succeeded = FindNextFileA(texture_looker.handle, &texture_looker.find_data);
        }
    //} else {
        //printf("Error: %d\n", GetLastError());
    //}
    FindClose(texture_looker.handle);
        // draw out name;
    //textures_of_folder.name = file_path.substr(file_path.find_last_of('/'), file_path.size() - file_path.find_last_of('/')).c_str();
    textures_of_folder.name = file_path.c_str();
    return textures_of_folder;
}

local_persist void Load_Textures_for_OpenGL(std::vector<texture_group>* texture_collection, const char* media_folder_path){
// Recursively loop over the folder and load group of textures here
    //graphic_obj->texture_collection.reserve(10); 
    texture_collection->reserve(10); 

    File_Manager folder_looker;
    std::string folder_path;

    // we just need to feed the folder path for stb_image to load texture's data
// Loop through all of the media folder for textures.
    PVOID OldValue = NULL;
    //Wow64DisableWow64FsRedirection(&OldValue);
    folder_path = media_folder_path;
    folder_path += "/*";
    folder_looker.handle = FindFirstFileA(folder_path.c_str(), &folder_looker.find_data);
    // which function I have to use to mark whether the maker reach the end of folder
    bool32 load_succeeded = true;
    if(folder_looker.handle != INVALID_HANDLE_VALUE){
        while(load_succeeded/*reach the end of folder*/ ){
            //stb_image(temp_image_content, path)
            // i
            // Win32 way        
                // TODO: Put a null check flag hee
                folder_path.clear();
            folder_path = media_folder_path;
            folder_path = folder_path + "/" + folder_looker.find_data.cFileName;

            if((folder_looker.find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (!string_contain(new string(folder_looker.find_data.cFileName),".")))
             texture_collection->push_back(load_textures_in_folder(&folder_looker, folder_path.c_str()));

            load_succeeded = FindNextFileA(folder_looker.handle, &folder_looker.find_data);
//This cause thread thread violation NOTE: Be careful
            //if(!load_succeeded)
                //printf("Error: %s\n", loadCurrentErr());
        }
        //Wow64RevertWow64FsRedirection(OldValue);
    }
    FindClose(folder_looker.handle);
}
// change graphic_property* graphic_obj to vector of texture_group
extern "C" __declspec(dllexport) void Load_Textures_for_OpenGL_(Platform_Properties* Game_Platform, std::vector<texture_group>* texture_collection, const char* media_folder_path){
    ReloadGLFunction(Game_Platform);
    Load_Textures_for_OpenGL(texture_collection, media_folder_path);
};

//-----------------For_Debugging-------------------------

void rigid_body::move(float delta_t){

    if(this->current_face != this->previous_face){
        OrientFace(this);
    }

    glm::vec3 UpAxis = glm::vec3(this->position[1]);
    glm::vec3 xAxis = glm::vec3(this->position[0]);
    glm::vec3 zAxis = glm::vec3(this->position[2]);

    printf("object current position is %s\n", glm::to_string(this->position).c_str());
    printf("object current up axis is %s\n", glm::to_string(UpAxis).c_str());
    printf("object current x axis is %s\n", glm::to_string(xAxis).c_str());
    printf("object current z axis is %s\n", glm::to_string(zAxis).c_str());    
    
    CalcNewPos(delta_t, this);
}

void move_object(float delta_t, rigid_body* object){

    if(object->object_speed.current_states != IDLE && (object->object_speed.previous_states == object->object_speed.current_states)){
        CalcNewV(delta_t, &object->object_speed.veclocity, &object->object_speed.acceleration);
    }else{
        object->object_speed.veclocity = object->object_speed.base_veclocity;
    }

    object->move(delta_t);
}

static void  Orient_Around_Y(glm::mat4* matrix, float angle){
// We forgot the Scale factor here
    //1st col
    (*matrix)[0][0] = (float)glm::cos(glm::radians(angle))/100;
    (*matrix)[0][1] = 0.0f;
    (*matrix)[0][2] = (float)-glm::sin(glm::radians(angle))/100;
    //2nd col
    (*matrix)[1][0] = 0.0f;
    (*matrix)[1][1] = (float)1/100;
    (*matrix)[1][2] = 0.0f;
    //3rd col
    (*matrix)[2][0] = (float)glm::sin(glm::radians(angle))/100;
    (*matrix)[2][1] = 0.0f;
    (*matrix)[2][2] = (float)glm::cos(glm::radians(angle))/100;

    //*matrix = glm::rotate(*matrix, glm::radians(angle), glm::vec3(0, 1, 0));
};

static void OrientFace(rigid_body* object){
    float deg ;
    if(object->current_face != object->previous_face){
         //deg = 90.0f * -float(object->current_face - object->previous_face);
         deg = 90.0f * float(object->current_face);
        if(deg > 360.0f)
            deg -= 360.0f;
        if(deg < -360.0f)
            deg += 360.0f;
    }else{
        deg = 0;
    }
    Orient_Around_Y(&object->position, deg);
}

static void CalcNewPos(float delta_time, rigid_body* object) {
    float value;
    //add a little friction(opposed force here)
   //
    switch(object->object_speed.current_states){
        case WALK_FORWARD:
            object->position[3][2] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][2], delta_time);            
            break;
        case WALK_BACKWARD:
            object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.acceleration, -object->object_speed.veclocity, object->position[3][2], delta_time);
            break;
        case WALK_RIGHT:
            object->position[3][0] = based_a_v_Pos_calc(object->object_speed.acceleration, object->object_speed.veclocity, object->position[3][0], delta_time);            
            break;
        case WALK_LEFT:
            object->position[3][0] = based_a_v_Pos_calc(-object->object_speed.acceleration, -object->object_speed.veclocity, object->position[3][0], delta_time);
            break;
        //default:
            //printf("still don't what to put into this defaut case\n");
            //break;
    };

    if(abs(object->position[3][2] - object->pre_pos.z) >= 3.0f){
        printf("distance traveled until falling:%f\n", abs(object->position[3][2] - object->pre_pos.z));
        if(object->object_speed.current_states == JUMP_FORWARD || object->object_speed.current_states == JUMP_BACKWARD){
            object->object_speed.previous_states = object->object_speed.current_states;
            object->object_speed.current_states = FALLING;

            test_vampire_motion.pre_pos = glm::vec3(test_vampire_motion.position[3]);
        }
    }else{
        CalcNewV(delta_time, &object->object_speed.jump_v, &object->object_speed.jump_a);
    }

    
    switch(object->object_speed.current_states){
        case JUMP_FORWARD:
            Jump(delta_time, object);
            ApplyGravity(delta_time, object);
            break;

        case JUMP_BACKWARD:
            Jump(delta_time, object);
            ApplyGravity(delta_time, object);
            break;

        case FALLING:
            ApplyGravity(delta_time, object);
            ApplyMomentum(delta_time, object);
            break;            

        default :
            printf("Gravity is being applied\n");
            ApplyGravity(delta_time, object);
            break;
    }

    if(object->position[3][1] >= 0.0f && object->object_speed.current_states == FALLING){
        object->object_speed.previous_states = object->object_speed.current_states;
        object->object_speed.current_states = IDLE;
        object->object_speed.jump_v = object->object_speed.base_jump_v;
        object->object_speed.falling_v = 0.0f;
    }

    printf("object current position is %s\n", glm::to_string(object->position).c_str());

    //for (float& axis const: *vector){
        //if(axis != 0,0f)
            //object_pos[3][i] = based_a_v_Pos_calc(object.acceleration, object.veclocity, object_pos[3][i], delta_time);
        //i++;
    //};
}

static void  ApplyGravity(float delta_t, rigid_body* object){
    if(object->position[3][1] < 0){
        object->object_speed.falling_v += 9.8f * delta_t;         
        object->position[3][1] = based_a_v_Pos_calc(9.8f, object->object_speed.falling_v, object->position[3][1], delta_t);
    }
}

static void ApplyMomentum(float delta_t, rigid_body* object){
    if(object->object_speed.jump_v > 1.0f)
        object->object_speed.jump_v -= 0.5f;
    if(object->object_speed.jump_v < 0.0f)
        object->object_speed.jump_v += 0.5f;

    if(object->object_speed.previous_states == JUMP_FORWARD){
        object->position[3][2] = based_a_v_Pos_calc(0.0f, -object->object_speed.jump_v, object->position[3][2], delta_t);
    }else if(object->object_speed.previous_states == JUMP_BACKWARD){
        object->position[3][2] = based_a_v_Pos_calc(0.0f, object->object_speed.jump_v, object->position[3][2], delta_t);
    }

};

static void Jump(float delta_t, rigid_body* object){

    object->position[3][1] = based_a_v_Pos_calc(-object->object_speed.jump_a, -object->object_speed.jump_v, object->position[3][1], delta_t);
    if(object->object_speed.current_states == JUMP_FORWARD){
          object->position[3][2] = based_a_v_Pos_calc(-object->object_speed.jump_a * 0.4f, -object->object_speed.jump_v * 0.7, object->position[3][2], delta_t);
    }else if(object->object_speed.current_states == JUMP_BACKWARD){
          object->position[3][2] = based_a_v_Pos_calc(object->object_speed.jump_a * 0.4f, object->object_speed.jump_v * 0.7, object->position[3][2], delta_t);
    }
}

static void CalcNewV(float FrameTime, float* veclo, float* accel){;
    (*veclo) +=  (*accel) * FrameTime;
};
