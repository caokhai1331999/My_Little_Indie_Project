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

    envir_light->Point_Lights[0].position = glm::vec3(7.0f,  5.0f,  3.0f);
    envir_light->Point_Lights[0].ambient = glm::vec3( 0.05f, 0.05f, 0.05f);
    envir_light->Point_Lights[0].diffuse = glm::vec3( 0.8f, 0.8f, 0.8f);
    envir_light->Point_Lights[0].specular = glm::vec3( 1.0f, 1.0f, 1.0f);
    envir_light->Point_Lights[0].constant = 1.0f;
    envir_light->Point_Lights[0].linearTerm = 0.09f;
    envir_light->Point_Lights[0].quadraticTerm = 0.032f;
          
    envir_light->Point_Lights[1].position = glm::vec3(-5.0f, 5.0f, 3.0f);    
    envir_light->Point_Lights[1].ambient = glm::vec3(0.05f, 0.05f, 0.05f);        
    envir_light->Point_Lights[1].diffuse = glm::vec3(0.8f, 0.8f, 0.8f);           
    envir_light->Point_Lights[1].specular = glm::vec3(1.0f, 1.0f, 1.0f);          
    envir_light->Point_Lights[1].constant = 1.0f;                     
    envir_light->Point_Lights[1].linearTerm = 0.09f;                  
    envir_light->Point_Lights[1].quadraticTerm = 0.032f;

    envir_light->dirLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    envir_light->dirLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    envir_light->dirLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
}

void set_environmental_light(B_shader_program* shader, global_light* envir_light, Camera* camera){

    shader->use();

    std::string test_name {shader->GetProgramName()};
    shader->setFloat("material.shininess", 25.0f);

    glm::vec3 lightPos (-4.0f, 10.0f, -15.0f);
        shader->setVec3("lightPos", lightPos);

        shader->setVec3("dirLight.ambient", envir_light->dirLight.ambient);
        shader->setVec3("dirLight.diffuse", envir_light->dirLight.diffuse);
        shader->setVec3("dirLight.specular", envir_light->dirLight.specular);

    
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

void setup_pointlight_(global_light* envir_light){
    setup_pointlight(envir_light);
}

void Set_environmental_light_(B_shader_program* shader, global_light* envir_light, Camera* camera){
    bool success = gladLoadGLLoader((GLADloadproc)wglGetProcAddress);
    assert(success);
    set_environmental_light(shader, envir_light, camera);
};

void Jump(glm::mat4* object_matrix, float jump_time, float DelayedRatio, glm::vec3 moving_vector){
    if(Is_Jumping)
    float jump_time = 0.0f;
          float distance = 2.0f;
          //float endpoint_in_z = dancing_vampire_core[2][3] + distance;
          moving_vector = glm::vec3(0.0f, 2.0f, -5.0f);

              if(moving_vector.y > 0.0f){
              object_matrix = glm::translate(dancing_vampire_core, moving_vector);
              printf("Curren vampire matrix: %s\n", glm::to_string(dancing_vampire_core).c_str());
              moving_vector.y = moving_vector.y * glm::cos(30.0f * jump_time);
              moving_vector = moving_vector * DelayedRatio;
              jump_time += 0.02f;
    *object_matrix = glm::translate(*object_matrix, glm::sin(30.0f/jump_time) * glm::vec3(0.0f, 2.0f * DelayedRatio, -5.0f * DelayedRatio));
}
}

void Move_(glm::mat4* object_matrix, float delta_time, float DelayedRatio){
    Move(object_matrix, delta_time, DelayedRatio);
}
              
