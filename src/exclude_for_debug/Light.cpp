/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Cao Khai $
   $Notice: (C) Copyright 2024 by Cao Khai, Inc. All Rights Reserved. $
   ======================================================================== */

#include "Light.h"

void setup_pointlight(global_light* envir_light){
    glm::vec3 pointlight_Pos[] =
        {
            glm::vec3(-0.2f, -1.0f, -0.3f),
            glm::vec3(0.05f, 0.05f, 0.05f),
            glm::vec3(0.4f, 0.4f, 0.4f),
            glm::vec3(0.5f, 0.5f, 0.5f)
        };

    envir_light->Point_Lights[0].position = glm::vec3( 0.7f,  0.2f,  2.0f);
    envir_light->Point_Lights[0].ambient = glm::vec3( 0.05f, 0.05f, 0.05f);
    envir_light->Point_Lights[0].diffuse = glm::vec3( 0.8f, 0.8f, 0.8f);
    envir_light->Point_Lights[0].specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    envir_light->Point_Lights[0].constant = 1.0f;
    envir_light->Point_Lights[0].linearTerm = 0.09f;
    envir_light->Point_Lights[0].quadraticTerm = 0.032f;
          
    envir_light->Point_Lights[1].position = glm::vec3(2.3f, -3.3f, -4.0f);    
    envir_light->Point_Lights[1].ambient = glm::vec3(0.05f, 0.05f, 0.05f);        
    envir_light->Point_Lights[1].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);           
    envir_light->Point_Lights[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);          
    envir_light->Point_Lights[1].constant = 1.0f;                     
    envir_light->Point_Lights[1].linearTerm = 0.09f;                  
    envir_light->Point_Lights[1].quadraticTerm = 0.032f;
}

void set_environmental_light(B_shader_program* shader, global_light* envir_light, Camera* camera){

    shader->use();
    setup_pointlight(envir_light);    

    shader->setFloat("material.shininess", 32.0f);

    shader->setVec3("lightPos", 10.0f, 5.0f, -3.3f);

    shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    
    std::string light_name;
    
    for(int i = 0; i < 2; i++){
        light_name = "pointLight_Pos[]";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].position);
        light_name = "pointLights[].diffuse";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].diffuse);
        light_name = "pointLights[].ambient";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].ambient);
        light_name = "pointLights[].specular";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setVec3(light_name.c_str(), envir_light->Point_Lights[i].specular);

        light_name = "pointLights[].constant";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 1.0f);
        light_name = "pointLights[].linearTerm";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.09f);
        light_name = "pointLights[].quadraticTerm";
        light_name.insert(light_name.cbegin() + light_name.find_first_of('[') + 1, '0'+i);
        shader->setFloat(light_name.c_str(), 0.032f);
    }
    glUseProgram(0);
};

void Set_environmental_light_(B_shader_program* shader, global_light* envir_light, Camera* camera){
    bool success = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
    assert(success);
    set_environmental_light(shader, envir_light, camera);
};
