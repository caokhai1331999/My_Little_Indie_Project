#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;

struct Material{

// Ambient will be the same even with change in input
   sampler2D texture_diffused1;
   sampler2D texture_specular1;

   vec3 ambient;
   vec3 diffuse;
   vec3 specular;

   float shininess;
};

uniform Material material;

void main(){
  if(texture(material.texture_diffused1, TexCoords).rgb!=vec3(0.0f)){
  FragColor = vec4(texture(material.texture_diffused1, TexCoords).rgb, 1.0f);
}else{
  FragColor = vec4(0.4f, 0.23f, 0.3f, 0.7f);
}
};